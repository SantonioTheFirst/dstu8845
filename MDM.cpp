#include "strumok_optimized.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <omp.h>
#include <cstring>
#include <chrono>
#include <iomanip>

constexpr uint16_t TOTAL_BITS = 768;
constexpr uint8_t WORDS_KEY = 4;
constexpr uint8_t WORDS_IV = 4;
constexpr uint8_t WORDS_COUNT = WORDS_KEY + WORDS_IV; // 12 слов
constexpr uint8_t TARGET_CUBE_SIZE = 35; // Целевая размерность куба
constexpr uint8_t TOTAL_ROUNDS = 32;
constexpr uint32_t MAX_CANDIDATES = 10000; 

// Строгое выравнивание структур для предотвращения false sharing в L1/L2
struct alignas(64) Candidate {
    uint64_t mask[WORDS_COUNT]; 
    uint32_t score;
    uint32_t signature;
};

// Обертка состояния для корректного выравнивания
struct alignas(64) CipherState {
    uint64_t words[WORDS_COUNT];
};

// Выровненный счетчик для изоляции памяти между ядрами процессора
struct alignas(64) ThreadData {
    uint32_t count;
};

alignas(64) Candidate S_current[MAX_CANDIDATES];
alignas(64) Candidate S_next[MAX_CANDIDATES * 2];

// Вызов оптимизированной функции инициализации Струмка
inline uint32_t run_mock_cipher(const uint64_t* key, const uint64_t* iv) {
    dstu8845 cipher;
    cipher.dstu8845_256_fast(key, iv);
    return cipher.z_0;
}

// Линейный эвалюатор с кодом Грея
inline void evaluate_mdm(Candidate& cand) {
    CipherState current_state = {0}; 
    uint32_t mdm_signature = run_mock_cipher(current_state.words, current_state.words + WORDS_KEY);
    
    uint32_t active_bits[128];
    uint32_t subset_size = 0;
    
    for (uint32_t w = 0; w < WORDS_COUNT; ++w) {
        uint64_t m = cand.mask[w];
        while (m != 0) {
            active_bits[subset_size++] = w * 64 + __builtin_ctzll(m);
            m &= m - 1;
        }
    }
    
    if (subset_size == 0) { cand.score = 0; cand.signature = 0; return; }
    
    uint64_t total_iterations = 1ULL << subset_size;
    
    for (uint64_t i = 1; i < total_iterations; ++i) {
        uint32_t flip_idx = __builtin_ctzll(i);
        uint32_t bit_to_flip = active_bits[flip_idx];
        current_state.words[bit_to_flip / 64] ^= (1ULL << (bit_to_flip % 64));
        mdm_signature ^= run_mock_cipher(current_state.words, current_state.words + WORDS_KEY);
    }
    cand.signature = mdm_signature;
    cand.score = (mdm_signature == 0) ? TOTAL_ROUNDS : __builtin_ctz(mdm_signature);
}

// Поддержание топа (In-place сортировка вставками)
inline void update_top(Candidate* top_array, uint32_t& count, const Candidate& new_cand, uint32_t k_keep) {
    if (count < k_keep) {
        top_array[count++] = new_cand;
        for (int32_t i = count - 1; i > 0 && top_array[i].score > top_array[i - 1].score; --i) {
            Candidate temp = top_array[i]; top_array[i] = top_array[i - 1]; top_array[i - 1] = temp;
        }
    } else if (new_cand.score > top_array[k_keep - 1].score) {
        top_array[k_keep - 1] = new_cand;
        for (int32_t i = k_keep - 1; i > 0 && top_array[i].score > top_array[i - 1].score; --i) {
            Candidate temp = top_array[i]; top_array[i] = top_array[i - 1]; top_array[i - 1] = temp;
        }
    }
}

// Подсистема чекпоинтов
void save_checkpoint(uint32_t step, const Candidate* pool, uint32_t count) {
    char name[64]; sprintf(name, "ckpt_step_%u.bin", step);
    std::ofstream f(name, std::ios::binary);
    if (f.is_open()) {
        f.write(reinterpret_cast<const char*>(&count), sizeof(count));
        f.write(reinterpret_cast<const char*>(pool), count * sizeof(Candidate));
    }
}

bool load_checkpoint(uint32_t step, Candidate* pool, uint32_t& count) {
    char name[64]; sprintf(name, "ckpt_step_%u.bin", step);
    std::ifstream f(name, std::ios::binary);
    if (!f.is_open()) return false;
    f.read(reinterpret_cast<char*>(&count), sizeof(count));
    f.read(reinterpret_cast<char*>(pool), count * sizeof(Candidate));
    std::cout << "--> Resumed from " << name << " (" << count << " candidates)\n";
    return true;
}

inline char* append_uint32(char* buf, uint32_t val) {
    if (val == 0) { *buf++ = '0'; return buf; }
    char tmp[12]; int32_t i = 0;
    while (val > 0) { tmp[i++] = '0' + (val % 10); val /= 10; }
    while (i > 0) { *buf++ = tmp[--i]; }
    return buf;
}

// Запись пула в файл CSV с учетом времени шага
void dump_pool_to_csv(std::ofstream& csv_file, uint32_t step, const Candidate* pool, uint32_t count, double step_time) {
    static constexpr size_t BUF_SIZE = 10 * 1024 * 1024;
    static char write_buf[BUF_SIZE];
    char* ptr = write_buf;
    
    char time_str[32];
    int time_len = sprintf(time_str, "%.3f", step_time);
    
    for (uint32_t i = 0; i < count; ++i) {
        const auto& cand = pool[i];
        ptr = append_uint32(ptr, step); *ptr++ = ';';
        ptr = append_uint32(ptr, i + 1); *ptr++ = ';';
        ptr = append_uint32(ptr, cand.score); *ptr++ = ';';
        
        std::memcpy(ptr, time_str, time_len);
        ptr += time_len; *ptr++ = ';';
        
        int32_t shift = 28;
        while (shift >= 0) {
            uint32_t nibble = (cand.signature >> shift) & 0xF;
            *ptr++ = (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
            shift -= 4;
        }
        *ptr++ = ';';
        
        bool first = true;
        for (uint32_t w = 0; w < WORDS_COUNT; ++w) {
            uint64_t m = cand.mask[w];
            while (m != 0) {
                if (!first) *ptr++ = ' ';
                ptr = append_uint32(ptr, w * 64 + __builtin_ctzll(m));
                first = false; m &= m - 1;
            }
        }
        *ptr++ = '\n';
        
        if (ptr - write_buf > static_cast<ptrdiff_t>(BUF_SIZE - 4096)) {
            csv_file.write(write_buf, ptr - write_buf); ptr = write_buf;
        }
    }
    if (ptr > write_buf) csv_file.write(write_buf, ptr - write_buf);
}

// HPC: Двухуровневый безблокировочный поиск (Cube Attack Edition - поиск только по IV)
void find_best_parallel(const Candidate& current_cand, uint32_t k_keep, uint32_t n_add, Candidate* global_top, uint32_t& global_count, Candidate* all_threads_tops, ThreadData* thread_data) {
    uint32_t available_bits[TOTAL_BITS]; 
    uint32_t avail_count = 0;
    
    // ВАЖНО: Сбор свободных бит ТОЛЬКО из пространства IV (w начинается с WORDS_KEY)
    for (uint32_t w = WORDS_KEY; w < WORDS_COUNT; ++w) {
        uint64_t inv = ~current_cand.mask[w];
        while (inv != 0) { 
            available_bits[avail_count++] = w * 64 + __builtin_ctzll(inv); 
            inv &= inv - 1; 
        }
    }
    
    int num_threads = omp_get_max_threads();
    std::memset(thread_data, 0, num_threads * sizeof(ThreadData)); // Обнуляем счетчики потоков

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        Candidate* my_top = &all_threads_tops[tid * k_keep];
        uint32_t& my_count = thread_data[tid].count;

        if (n_add == 1) {
            #pragma omp for schedule(dynamic, 64) nowait
            for (uint32_t i = 0; i < avail_count; ++i) {
                Candidate test_cand = current_cand;
                uint32_t b1 = available_bits[i];
                test_cand.mask[b1 / 64] |= (1ULL << (b1 % 64));
                
                evaluate_mdm(test_cand);
                update_top(my_top, my_count, test_cand, k_keep);
            }
        } 
        else if (n_add == 2) {
            #pragma omp for schedule(dynamic, 16) nowait
            for (uint32_t i = 0; i < avail_count; ++i) {
                for (uint32_t j = i + 1; j < avail_count; ++j) {
                    Candidate test_cand = current_cand;
                    uint32_t b1 = available_bits[i]; 
                    uint32_t b2 = available_bits[j];
                    test_cand.mask[b1 / 64] |= (1ULL << (b1 % 64));
                    test_cand.mask[b2 / 64] |= (1ULL << (b2 % 64));
                    
                    evaluate_mdm(test_cand);
                    update_top(my_top, my_count, test_cand, k_keep);
                }
            }
        }
    }

    // Последовательное слияние локальных топов в глобальный без критических секций
    global_count = 0;
    for (int t = 0; t < num_threads; ++t) {
        Candidate* thread_sector = &all_threads_tops[t * k_keep];
        uint32_t t_count = thread_data[t].count;
        for (uint32_t i = 0; i < t_count; ++i) {
            update_top(global_top, global_count, thread_sector[i], k_keep);
        }
    }
}

// Основной цикл
void slightly_greedy(const uint32_t* k_vec, const uint32_t* n_vec, const uint32_t* limit_vec, uint32_t iterations) {
    std::ofstream csv_file("mdm_analytics.csv", std::ios::out | std::ios::binary | std::ios::app);
    if (csv_file.is_open()) {
        csv_file << "Step;Rank;Score;StepTime(s);Signature;Cube_Bits\n"; 
    }
    
    uint32_t current_count = 1;
    std::memset(&S_current[0], 0, sizeof(Candidate));
    
    uint32_t start_step = 0;
    for (int32_t s = iterations - 1; s >= 0; --s) {
        if (load_checkpoint(s, S_current, current_count)) { 
            start_step = s + 1; 
            break; 
        }
    }

    // Выделение памяти под структуры распараллеливания 1 раз, чтобы избежать задержек (low-latency)
    int max_threads = omp_get_max_threads();
    Candidate* all_threads_tops = new Candidate[max_threads * MAX_CANDIDATES];
    ThreadData* thread_data = new ThreadData[max_threads];

    for (uint32_t step = start_step; step < iterations; ++step) {
        auto step_start_time = std::chrono::high_resolution_clock::now();
        
        uint32_t next_count = 0;
        uint32_t k_keep = k_vec[step];
        
        Candidate* local_results = new Candidate[k_keep];
        
        for (uint32_t i = 0; i < current_count; ++i) {
            uint32_t returned_count = 0;
            // Передаем сырые указатели для исключения аллокаций внутри горячего алгоритма
            find_best_parallel(S_current[i], k_keep, n_vec[step], local_results, returned_count, all_threads_tops, thread_data);
            
            for (uint32_t j = 0; j < returned_count; ++j) {
                if (next_count < MAX_CANDIDATES * 2) {
                    S_next[next_count++] = local_results[j];
                }
            }
        }
        delete[] local_results;
        
        for (uint32_t i = 1; i < next_count; ++i) {
            Candidate key = S_next[i]; int32_t j = i - 1;
            while (j >= 0 && S_next[j].score < key.score) { S_next[j + 1] = S_next[j]; j--; }
            S_next[j + 1] = key;
        }
        
        uint32_t reduced_count = next_count;
        if (reduced_count > limit_vec[step]) reduced_count = limit_vec[step];
        if (reduced_count == 0 && next_count > 0) reduced_count = 1;
        if (reduced_count > MAX_CANDIDATES) reduced_count = MAX_CANDIDATES;
        
        current_count = reduced_count;
        std::memcpy(S_current, S_next, current_count * sizeof(Candidate));
        
        auto step_end_time = std::chrono::high_resolution_clock::now();
        double elapsed_seconds = std::chrono::duration<double>(step_end_time - step_start_time).count();
        
        std::cout << "Step " << step 
                  << " | Max Score: " << S_current[0].score 
                  << " | Pool size: " << current_count 
                  << " | Time: " << std::fixed << std::setprecision(3) << elapsed_seconds << " s\n";
        
        if (csv_file.is_open()) {
            dump_pool_to_csv(csv_file, step, S_current, current_count, elapsed_seconds);
        }
        save_checkpoint(step, S_current, current_count);
    }
    
    // Освобождение памяти
    delete[] all_threads_tops;
    delete[] thread_data;
    csv_file.close();
}

int main() {
    omp_set_num_threads(omp_get_max_threads());
    
    // Замена std::vector на стековые массивы 
    uint32_t k_vec[TARGET_CUBE_SIZE];
    uint32_t n_vec[TARGET_CUBE_SIZE];
    uint32_t limit_vec[TARGET_CUBE_SIZE];
    
    // Шаг 0: ищем пары (n = 2)
    n_vec[0] = 2;
    k_vec[0] = MAX_CANDIDATES;
    limit_vec[0] = MAX_CANDIDATES;

    // Шаг 1: ищем пары (n = 2)
    n_vec[1] = 2;
    k_vec[1] = 2000;
    limit_vec[1] = MAX_CANDIDATES;
    
    // Последующие шаги: наращиваем по 1 биту
    for (uint32_t i = 2; i < TARGET_CUBE_SIZE; ++i) {
        n_vec[i] = 1;
        k_vec[i] = 2000;
        limit_vec[i] = MAX_CANDIDATES;
    }
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::cout << "Starting SlightlyGreedy search using " << omp_get_max_threads() << " threads...\n";
    slightly_greedy(k_vec, n_vec, limit_vec, TARGET_CUBE_SIZE);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "Total Elapsed time: " 
              << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() 
              << " s\n";
              
    return 0;
}
