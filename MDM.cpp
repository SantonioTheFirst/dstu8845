#include <cstdint>  // Подключаем типы фиксированной ширины (uint32_t, uint64_t) для точной работы с битовыми масками
#include <iostream> // Подключаем поток ввода-вывода для отображения результатов работы

// Константы этапа компиляции для жесткого контроля памяти
constexpr int TOTAL_BITS = 768; // Общее пространство: 512 бит ключа + 256 бит IV
constexpr int WORDS_COUNT = 12; // 768 бит / 64 бита в слове = ровно 12 слов типа uint64_t
constexpr int TOTAL_ROUNDS = 32; [span_1](start_span)// Количество раундов инициализации твоего шифра[span_1](end_span)
constexpr int MAX_CANDIDATES = 1000; [span_2](start_span)// Ограничение популяции кандидатов для алгоритма SlightlyGreedy[span_2](end_span)
constexpr int NEXT_GEN_BUFFER = 10000; [span_3](start_span)// Размер статического буфера для списка L_c до применения фактора alpha[span_3](end_span)

// Структура кандидата: теперь это чистая битовая маска 768-битного состояния
struct Candidate { // Объявление структуры кандидата
    uint64_t mask[WORDS_COUNT]; // 12 слов по 64 бита. Установленный бит (1) означает, что бит включен в исследуемое подмножество
    int score; [span_4](start_span)// Оценка кандидата: количество нулевых раундов инициализации подряд[span_4](end_span)
}; // Конец структуры

// Выравнивание глобальных массивов по границе 64 байт для оптимизации кэш-промахов L1
alignas(64) Candidate S_current[MAX_CANDIDATES]; // Массив кандидатов текущего поколения
alignas(64) Candidate S_next[NEXT_GEN_BUFFER]; // Массив кандидатов следующего поколения до сортировки и редукции

// Эмуляция 32-раундового потокового шифра. 
// Принимает 768-битное состояние, возвращает 32 бита вывода (по 1 биту на раунд)
uint32_t run_mock_cipher(const uint64_t* state) { // Функция принимает указатель на массив из 12 слов uint64_t
    uint32_t hash = 0x811c9dc5; // Базовое значение FNV-хеша для детерминированного перемешивания
    for (int i = 0; i < WORDS_COUNT; ++i) { // Итерируемся по всем 12 словам 64-битного состояния
        hash ^= static_cast<uint32_t>(state[i] & 0xFFFFFFFF); // Подмешиваем младшие 32 бита слова в хеш
        hash *= 0x01000193; // Умножаем на простое число FNV для диффузии
        hash ^= static_cast<uint32_t>(state[i] >> 32); // Подмешиваем старшие 32 бита слова в хеш
        hash *= 0x01000193; // Снова умножаем для завершения раунда перемешивания текущего слова
    } // Конец цикла по словам состояния
    return hash; [span_5](start_span)// Возвращаем 32-битный результат, симулирующий выход 32 раундов инициализации[span_5](end_span)
} // Конец функции эмуляции

// Функция оценки кандидата (MDM test). [span_6](start_span)[span_7](start_span)Вычисляет сигнатуру для подмножества, заданного маской[span_6](end_span)[span_7](end_span)
int evaluate_mdm(const uint64_t* mask) { // Принимает 768-битную маску выбранных бит
    uint64_t current_state[WORDS_COUNT] = {0}; // Выделяем на стеке 768-битное рабочее состояние и зануляем его
    uint32_t mdm_signature = run_mock_cipher(current_state); [span_8](start_span)// Инициализируем сигнатуру базовым вызовом (все нули)[span_8](end_span)
    
    int active_bits[128]; // Стек-буфер для хранения реальных индексов выбранных бит (для генерации кода Грея)
    int subset_size = 0; // Счетчик количества выбранных бит (размерность текущего куба)
    
    // Битовая магия для быстрого извлечения индексов из 768-битной маски
    for (int w = 0; w < WORDS_COUNT; ++w) { // Проходим по 12 словам маски
        uint64_t m = mask[w]; // Копируем слово во временную переменную, чтобы безопасно ее разрушать
        while (m != 0) { // Пока в слове остались единицы
            int bit_idx = __builtin_ctzll(m); // Находим позицию младшего установленного бита (Count Trailing Zeros)
            active_bits[subset_size++] = w * 64 + bit_idx; // Вычисляем глобальный индекс бита (0..767) и сохраняем
            m &= m - 1; // Сбрасываем младший установленный бит (быстрый переход к следующей единице)
        } // Конец цикла по битам одного слова
    } // Конец извлечения индексов
    
    if (subset_size == 0) return 0; // Если маска пуста, возвращаем 0 (куба нет)
    
    uint64_t total_iterations = 1ULL << subset_size; [span_9](start_span)// Объем куба: 2 в степени количества бит подмножества[span_9](end_span)
    
    // Проход по кубу с использованием кодов Грея (смена 1 бита за шаг)
    for (uint64_t i = 1; i < total_iterations; ++i) { // Начинаем с 1, так как состояние 0 уже учтено
        int flip_idx = __builtin_ctzll(i); // Находим какой бит кода Грея нужно инвертировать на этом шаге
        int bit_to_flip = active_bits[flip_idx]; // Получаем глобальный индекс шифра (0..767) из нашего массива
        
        int word_idx = bit_to_flip / 64; // Определяем, в каком из 12 слов находится этот бит
        int bit_pos = bit_to_flip % 64; // Определяем позицию бита (0..63) внутри этого слова
        
        current_state[word_idx] ^= (1ULL << bit_pos); // Инвертируем ровно один бит состояния
        mdm_signature ^= run_mock_cipher(current_state); [span_10](start_span)// Вызываем шифр и XOR-им выход с общей сигнатурой[span_10](end_span)
    } // Конец прохода по кубу
    
    if (mdm_signature == 0) return TOTAL_ROUNDS; // Если все биты нули, возвращаем максимум (32 раунда)
    
    [span_11](start_span)// Считаем подряд идущие нули начиная с младшего бита (соответствует нулевому раунду)[span_11](end_span)
    return __builtin_ctz(mdm_signature); // Возвращаем длину непрерывной серии нулей в сигнатуре
} // Конец функции оценки

// Итеративный ленивый генератор сочетаний (n из k)
bool next_combination(int* indices, int n, int k) { // Принимает индексы, размер пула (n) и сколько нужно выбрать (k)
    int i = k - 1; // Начинаем с самого правого элемента массива индексов
    while (i >= 0 && indices[i] == n - k + i) { // Идем влево, пока элементы достигли своего максимума
        i--; // Сдвигаем указатель левее
    } // Конец цикла поиска элемента для инкремента
    
    if (i < 0) return false; // Если все индексы на максимуме, комбинации закончились
    
    indices[i]++; // Увеличиваем найденный элемент
    for (int j = i + 1; j < k; ++j) { // Проходим по всем элементам правее инкрементированного
        indices[j] = indices[j - 1] + 1; // Выстраиваем их строго по возрастанию (+1 от предыдущего)
    } // Конец восстановления порядка хвоста
    
    return true; // Комбинация сгенерирована
} // Конец функции генератора

[span_12](start_span)// Алгоритм FindBest: ищет лучшие биты для добавления к текущей маске[span_12](end_span)
void find_best(const Candidate& current_cand, int k_keep, int n_add, Candidate* out_top, int& out_count) { // Параметры: текущий кандидат, сколько оставить, сколько бит добавить
    int available_bits[TOTAL_BITS]; // Стек-буфер для глобальных индексов бит, которых еще нет в маске
    int avail_count = 0; // Счетчик доступных бит
    
    // Извлекаем индексы нулей (свободных бит) из текущей маски кандидата
    for (int w = 0; w < WORDS_COUNT; ++w) { // Проходим по всем 12 словам маски
        uint64_t inv_mask = ~current_cand.mask[w]; // Инвертируем маску: теперь 1 означает "бит свободен"
        uint64_t m = inv_mask; // Копируем инвертированную маску для разрушающего чтения
        while (m != 0) { // Пока есть свободные биты
            int bit_pos = __builtin_ctzll(m); // Находим позицию свободного бита
            available_bits[avail_count++] = w * 64 + bit_pos; // Записываем глобальный индекс (0..767)
            m &= m - 1; // Сбрасываем обработанный бит
        } // Конец цикла по свободным битам слова
    } // Конец сбора доступных бит
    
    int indices[128]; // Буфер для генератора комбинаций (индексы внутри массива available_bits)
    for (int i = 0; i < n_add; ++i) { // Задаем начальную комбинацию
        indices[i] = i; // 0, 1, 2...
    } // Конец начальной инициализации
    
    out_count = 0; // Обнуляем счетчик найденных кандидатов
    
    [span_13](start_span)do { // Начинаем цикл тестирования комбинаций[span_13](end_span)
        Candidate test_cand = current_cand; // Копируем маску родителя целиком
        
        for (int i = 0; i < n_add; ++i) { // Проходим по выбранным новым битам
            int global_bit_idx = available_bits[indices[i]]; // Достаем глобальный индекс бита
            int word_idx = global_bit_idx / 64; // Вычисляем слово
            int bit_pos = global_bit_idx % 64; // Вычисляем позицию в слове
            test_cand.mask[word_idx] |= (1ULL << bit_pos); // Устанавливаем бит в маске кандидата
        } // Конец установки новых бит
        
        test_cand.score = evaluate_mdm(test_cand.mask); // Вычисляем MDM сигнатуру и скор для новой маски
        
        // Логика добавления в топ-k (сортировка вставкой на лету)
        if (out_count < k_keep) { // Если топ еще не заполнен
            out_top[out_count++] = test_cand; // Добавляем в конец
            for (int i = out_count - 1; i > 0 && out_top[i].score > out_top[i - 1].score; --i) { // Всплытие пузырьком
                Candidate temp = out_top[i]; // Запоминаем текущий
                out_top[i] = out_top[i - 1]; // Сдвигаем верхний вниз
                out_top[i - 1] = temp; // Ставим текущий выше
            } // Конец всплытия
        } else if (test_cand.score > out_top[k_keep - 1].score) { // Если топ полон, но новый кандидат лучше худшего в топе
            out_top[k_keep - 1] = test_cand; // Заменяем худшего
            for (int i = k_keep - 1; i > 0 && out_top[i].score > out_top[i - 1].score; --i) { // Поднимаем его на нужное место
                Candidate temp = out_top[i]; // Временная переменная
                out_top[i] = out_top[i - 1]; // Сдвиг
                out_top[i - 1] = temp; // Установка
            } // Конец сортировки
        } // Конец логики поддержания топа
    } while (next_combination(indices, avail_count, n_add)); [span_14](start_span)// Берем следующую комбинацию, пока они есть[span_14](end_span)
} // Конец алгоритма FindBest

[span_15](start_span)// Алгоритм SlightlyGreedy: обобщенный жадный алгоритм[span_15](end_span)
[span_16](start_span)void slightly_greedy(const int* k_vec, const int* n_vec, const float* alpha_vec, int iterations) { // Принимает векторы настроек[span_16](end_span)
    int current_count = 1; // Стартуем с одного базового состояния
    for (int w = 0; w < WORDS_COUNT; ++w) S_current[0].mask[w] = 0; // Зануляем 768-битную маску первого кандидата (пустое множество)
    S_current[0].score = 0; // Начальный скор равен 0
    
    for (int step = 0; step < iterations; ++step) { // Цикл по заданному количеству итераций
        int next_count = 0; // Счетчик кандидатов следующего поколения до обрезки
        
        [span_17](start_span)for (int i = 0; i < current_count; ++i) { // Проходим по всем выжившим кандидатам текущего поколения[span_17](end_span)
            Candidate local_top[128]; // Буфер для приема лучших расширений от FindBest
            int local_count = 0; // Количество реально найденных
            
            find_best(S_current[i], k_vec[step], n_vec[step], local_top, local_count); [span_18](start_span)// Запрашиваем расширения[span_18](end_span)
            
            for (int j = 0; j < local_count; ++j) { // Перебираем результаты
                if (next_count < NEXT_GEN_BUFFER) { // Если глобальный буфер следующего поколения не переполнен
                    S_next[next_count++] = local_top[j]; // Копируем кандидата
                } // Конец проверки буфера
            } // Конец переноса
        } // Конец цикла обработки поколения
        
        [span_19](start_span)// Сортировка всего следующего поколения (сортировка вставкой, т.к. STL запрещен)[span_19](end_span)
        for (int i = 1; i < next_count; ++i) { // Начинаем со второго элемента
            Candidate key = S_next[i]; // Запоминаем текущий
            int j = i - 1; // Указываем на предыдущий
            while (j >= 0 && S_next[j].score < key.score) { // Ищем место для вставки (по убыванию скора)
                S_next[j + 1] = S_next[j]; // Сдвигаем меньшие элементы вправо
                j = j - 1; // Шагаем влево
            } // Конец поиска позиции
            S_next[j + 1] = key; // Вставляем
        } // Конец сортировки
        
        [span_20](start_span)// Редукция (отсечение хвоста списка) фактором альфа[span_20](end_span)
        int reduced_count = static_cast<int>(next_count * alpha_vec[step]); // Вычисляем новое количество
        if (reduced_count == 0 && next_count > 0) reduced_count = 1; // Оставляем хотя бы одного, чтобы алгоритм жил
        if (reduced_count > MAX_CANDIDATES) reduced_count = MAX_CANDIDATES; // Жесткий лимит по памяти
        
        current_count = reduced_count; // Обновляем размер пула
        for (int i = 0; i < current_count; ++i) { // Цикл копирования
            S_current[i] = S_next[i]; // Перенос структур из буфера в основной массив поколения
        } // Конец копирования
        
        std::cout << "Step " << step << " max zero rounds: " << S_current[0].score << "\n"; // Выводим лучший результат шага
    } // Конец итераций алгоритма
} // Конец алгоритма

int main() { // Точка входа
    const int k_vec[] = {5, 3, 2}; [span_21](start_span)// Настройки: сохранять 5, затем 3, затем 2 ветки на родителя[span_21](end_span)
    const int n_vec[] = {2, 1, 1}; [span_22](start_span)// Настройки: добавлять по 2 бита, затем по 1 биту[span_22](end_span)
    const float alpha_vec[] = {1.0f, 0.5f, 0.5f}; [span_23](start_span)// Настройки: редукция пула[span_23](end_span)
    
    slightly_greedy(k_vec, n_vec, alpha_vec, 3); // Запуск алгоритма
    
    return 0; // Успешное завершение
}
