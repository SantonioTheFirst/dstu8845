#include "strumok_optimized.h"
#include <iostream>
#include <time.h>

int main()
{
    using namespace std;
    alignas(64) uint64_t key[8] = {
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x0000000000000000,
    0x0000000000000000, 0x8000000000000000
    };
    alignas(64) uint64_t iv[4] = {1, 2, 3, 4};

    uint32_t z_0 = 0;
    clock_t begin = clock();
    for(uint64_t i = 0; i < 1000'000; i++)
    {
        dstu8845 cipher = dstu8845::dstu8845_512_fast(key, iv);
        z_0 = cipher.z_0;
    }
    clock_t end = clock();
    cout << z_0 << "\n";
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    cout << "Init time: " << time_spent << "\n" << "Init per second: " << 1000'000 / time_spent << "\n";

    //dstu8845 cipher = dstu8845::dstu8845_512_fast(key, iv);
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
