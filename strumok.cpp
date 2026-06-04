#include "strumok_optimized.h"
#include <inttypes.h>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <iostream>
using namespace std;


dstu8845::dstu8845(const uint64_t *S, const uint64_t *r, const uint64_t *key, const uint64_t *iv)
{
    memcpy(this->S, S, 128);
    memcpy(this->r, r, 16);
    memcpy(this->key, key, sizeof(key) / sizeof(uint64_t));
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
    for(size_t i = 0; i < 16; i++)
    {
        printf("S_%zu 0x%016" PRIx64 "\n", i, this->S[i]);
    }
    printf("r_0 0x%016" PRIx64 "\n", this->r[0]);
    printf("r_0 0x%016" PRIx64 "\n", this->r[1]);
}
      

dstu8845::dstu8845_512_verbose(const uint64_t *key, const uint64_t *iv)
{
  this->z_0 = 0;
  memcpy(this->iv, iv, 32);
  memcpy(this->key, key, 64);
  this->S[0] = this->key[7] ^ this->iv[0];
  this->S[1] = this->key[6];
  this->S[2] = this->key[5];
  this->S[3] = this->key[4] ^ this->iv[1];
  this->S[4] = this->key[3];
  this->S[5] = this->key[2] ^ this->iv[2];
  this->S[6] = this->key[1];
  this->S[7] = ~this->key[0];
  this->S[8] = this->key[4] ^ this->iv[3];
  this->S[9] = ~this->key[6];
  this->S[10] = this->key[5];
  this->S[11] = ~this->key[7];
  this->S[12] = this->key[3];
  this->S[13] = this->key[2];
  this->S[14] = ~this->key[1];
  this->S[15] = this->key[0];
  
  this->r[0] = 0;
  this->r[1] = 0;

  //verbose
  for(uint8_t i = 0; i < 16; i++)
  {
    cout << "S_" << (unsigned)i << ": ";
    stringstream ss;
    ss<< hex << this->S[i];
    string res ( ss.str() );
    cout << res << endl;
  }
  for(uint8_t i = 0; i < 2; i++)
  {
    stringstream ss;
    ss<< hex << this->r[i];
    string res ( ss.str() );
    cout << "r_" << (unsigned)i << ": " << res << endl;
  }
  cout << endl;

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
    
    //verbose
    for(uint8_t j = 0; j < 16; j++)
    {
      cout << "S_" << (unsigned)j << ": ";
      stringstream ss;
      ss<< hex << this->S[j];
      string res ( ss.str() );
      cout << res << endl;
    }
    for(uint8_t j = 0; j < 2; j++)
    {
      stringstream ss;
      ss<< hex << this->r[j];
      string res ( ss.str() );
      cout << "r_" << (unsigned)j << ": " << res << endl;
    }
    cout << endl;
  }
}

dstu8845::dstu8845_512(const uint64_t *key, const uint64_t *iv)
{
  this->z_0 = 0;
  memcpy(this->iv, iv, 32);
  memcpy(this->key, key, 64);
  this->S[0] = this->key[7] ^ this->iv[0];
  this->S[1] = this->key[6];
  this->S[2] = this->key[5];
  this->S[3] = this->key[4] ^ this->iv[1];
  this->S[4] = this->key[3];
  this->S[5] = this->key[2] ^ this->iv[2];
  this->S[6] = this->key[1];
  this->S[7] = ~this->key[0];
  this->S[8] = this->key[4] ^ this->iv[3];
  this->S[9] = ~this->key[6];
  this->S[10] = this->key[5];
  this->S[11] = ~this->key[7];
  this->S[12] = this->key[3];
  this->S[13] = this->key[2];
  this->S[14] = ~this->key[1];
  this->S[15] = this->key[0];
  
  this->r[0] = 0;
  this->r[1] = 0;

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

uint64_t inline dstu8845::next_stream()
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
