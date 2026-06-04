#include "strumok_optimized.h"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;


dstu8845::dstu8845(const uint64_t *S, const uint64_t *r, const uint64_t *key, const uint64_t *iv)
{   
    cout << sizeof(key) / sizeof(key[0]) << endl;
    memcpy(this->S, S, 128);
    memcpy(this->r, r, 16);
    memcpy(this->key, key, sizeof(key) / sizeof(key[0]));
    memcpy(this->iv, iv, 32);
            
    static uint64_t outfrom_fsm, fsmtmp, tmp;
    for(uint8_t i = 0; i < 32; ++i)
    {
        outfrom_fsm = (this->r[0] + this->S[15]) ^ this->r[1];
        tmp = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[13];
        this->r[1] = this->T(this->r[0]);
        this->r[0] = fsmtmp;
        for(uint8_t j = 0; j < 15; ++j)
        {
            this->S[j] = this->S[j + 1];
        }
        this->S[15] = tmp;
        this->z_0 |= ((((this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15]))) & 1) << i);
    }
}

dstu8845 dstu8845::dstu8845_512(const uint64_t *key, const uint64_t *iv)
{
        uint64_t S[16];
        uint64_t r[2];

        S[0] = key[7] ^ iv[0];
        S[1] = key[6];
        S[2] = key[5];
        S[3] = key[4] ^ iv[1];
        S[4] = key[3];
        S[5] = key[2] ^ iv[2];
        S[6] = key[1];
        S[7] = ~key[0];
        S[8] = key[4] ^ iv[3];
        S[9] = ~key[6];
        S[10] = key[5];
        S[11] = ~key[7];
        S[12] = key[3];
        S[13] = key[2];
        S[14] = ~key[1];
        S[15] = key[0];
        r[0] = 0;
        r[1] = 0;
        return dstu8845(S, r, key, iv);
}

void dstu8845::print()
{
    for(uint8_t i = 0; i < 16; i++)
    {
        cout << "S_" << (unsigned)i << ": 0x" << hex << uppercase << setfill('0') << setw(16) << this->S[i] << dec << nouppercase << "\n";
    }
    cout << "r_0: 0x" << hex << uppercase << setfill('0') << setw(16) << this->r[0] << dec << nouppercase << "\n";
    cout << "r_1: 0x" << hex << uppercase << setfill('0') << setw(16) << this->r[1] << dec << nouppercase << "\n";
}
    
uint64_t dstu8845::next_stream()
{
  uint64_t fsmtmp, tmp, out_stream;

  tmp = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]);
  fsmtmp = this->r[1] + this->S[13];
  this->r[1] = this->T(this->r[0]);
  this->r[0] = fsmtmp;
  out_stream = (this->r[0] + tmp) ^ this->r[1] ^ this->S[1];

  for(uint8_t i = 0; i < 15; ++i)
  {
    this->S[i] = this->S[i + 1];
  }
  this->S[15] = tmp;

  return out_stream;
}

void dstu8845::dstu8845_crypt(const uint64_t *in, uint64_t *out)
{
    uint64_t size = sizeof(in) / sizeof(in[0]);
    //cout << "Size " << size << endl;
    for(uint64_t i = 0; i < size; i++)
    {
        out[i] = in[i] ^ this->next_stream();
    }
}
