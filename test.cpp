#include "strumok_optimized.h"
#include <iostream>

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

    const uint64_t in[4096] = {0};
    uint64_t out[4096] = {0};
  
    for(uint64_t i = 0; i < 1024 * 15 * 256; i++)
      cipher.dstu8845_crypt(in, out);
    cout << out[0] << endl;
    clock_t begin = clock();

    for (int i = 0; i < 1024 * 15 * 256; i++) {
        dstu8845_crypt(ctx, out, 4096, out);
    }

    clock_t end = clock();


    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Encrypted 15GB in %f seconds\n", time_spent);
    printf("Speed is %f Gb/s\n", ((1 / ((time_spent * 1.0) / 15)) * 8));

    return 0;
}
