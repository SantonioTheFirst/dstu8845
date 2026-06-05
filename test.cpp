#include "strumok_optimized.h"
#include <iostream>
#include <time.h>

int main()
{
    using namespace std;
    uint64_t key[8];
    uint64_t iv[4];
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

    dstu8845 cipher = dstu8845::dstu8845_512(key, iv);
    cipher.print();

    const uint8_t in[4096] = {0};
    uint8_t out[4096] = {0};
    
    clock_t begin = clock();
    for(uint64_t i = 0; i < 1024 * 128 * 256; i++)
    {
        cipher.dstu8845_crypt(in, out, 4096);
    }
    clock_t end = clock();
    cout << out[0] << endl;
    
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    cout << "Encrypted 128 GB in " << time_spent << " seconds\n";
    cout << "Speed is " << ((1 / ((time_spent * 1.0) / 128)) * 8) << " Gb/s\n";

    return 0;
}
