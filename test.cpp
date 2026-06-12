#include "strumok_optimized.h"
#include <iostream>
#include <time.h>

int main()
{
    using namespace std;
    alignas(64) uint64_t key_512[8] = {
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x8000000000000000
    };

    alignas(64) uint64_t key_256[8] = {
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x8000000000000000
    };
    alignas(64) uint64_t iv[4] = {1, 2, 3, 4};

    uint32_t z_0 = 0;
    dstu8845 cipher;
    // warmup
    for(uint64_t i = 0; i < 1000'000; i++)
    {
        cipher.dstu8845_256_fast(key_256, iv);
        z_0 ^= cipher.z_0;
    }

    clock_t begin = clock();
    for(uint64_t i = 0; i < 10'000'000; i++)
    {
        cipher.dstu8845_256_fast(key_256, iv);
        z_0 ^= cipher.z_0;
    }
    clock_t end = clock();
    cout << z_0 << "\n";
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Init time 256 bit: " << time_spent << "\n" << "Init per second: " << 10'000'000 / time_spent << "\n";
   
    begin = clock();
    for(uint64_t i = 0; i < 10'000'000; i++)
    {
        cipher.dstu8845_512_fast(key_512, iv);
        z_0 ^= cipher.z_0;
    }
    end = clock();
    cout << z_0 << "\n";
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Init time 512 bit: " << time_spent << "\n" << "Init per second: " << 10'000'000 / time_spent << "\n";

    cipher.dstu8845_512_fast(key_512, iv);
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
