#include "strumok_optimized.h"
#include <iostream>
#include <time.h>

int main()
{
    using namespace std;
    alignas(64) const uint64_t key[8];
    alignas(64) const uint64_t iv[4];
    iv[0] = 1;
    iv[1] = 2;
    iv[2] = 3;
    iv[3] = 4;

    key[7] = 0x8000000000000000;
    key[6] = 0x0000000000000000;
    key[5] = 0x0000000000000000;
    key[4] = 0x0000000000000000;
    key[3] = 0x0000000000000000;
    key[2] = 0x0000000000000000;  
    key[1] = 0x0000000000000000;
    key[0] = 0x0000000000000000;

    clock_t begin = clock();
    for(uint64_t i = 0; i < 1000'000; i++)
    {
        dstu8845 cipher = dstu8845::dstu8845_512(key, iv);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Init time: " << time_spent << "\n" << "Init per second: " << 1000'000 / time_spent << "\n";

    dstu8845 cipher = dstu8845::dstu8845_512(key, iv);
    cipher.print();

    alignas(64) const uint8_t in[4096] = {0};
    alignas(64) uint8_t out[4096] = {0};
    
    begin = clock();
    for(uint64_t i = 0; i < 1024 * 15 * 256; i++)
    {
        cipher.dstu8845_crypt(in, out, 4096);
    }
    end = clock();
    cout << (unsigned) out[0] << endl;
    
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    cout << "Encrypted 15 GB in " << time_spent << " seconds\n";
    cout << "Speed is " << ((1 / ((time_spent * 1.0) / 15)) * 8) << " Gb/s\n";

    return 0;
}
