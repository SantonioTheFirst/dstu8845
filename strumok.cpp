#include "strumok_optimized.h"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;


dstu8845::dstu8845(const uint64_t *S, const uint64_t *r, const uint64_t *key, const uint8_t key_size, const uint64_t *iv)
{   
    memcpy(this->S, S, 128);
    memcpy(this->r, r, 16);
    memcpy(this->key, key, key_size);
    memcpy(this->iv, iv, 32);
    uint64_t outfrom_fsm, fsmtmp;
    
    for(uint8_t i = 0; i < 2; i++)
    {
        outfrom_fsm = (this->r[0] + this->S[15]) ^ this->r[1];
        this->S[0] = a_mul(this->S[0]) ^ this->S[13] ^ ainv_mul(this->S[11]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[13];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[1] ^ this->r[1] ^ (this->r[0] + this->S[0])) & 1) << (i * 16);

        outfrom_fsm = (this->r[0] + this->S[0]) ^ this->r[1];
        this->S[1] = a_mul(this->S[1]) ^ this->S[14] ^ ainv_mul(this->S[12]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[14];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[2] ^ this->r[1] ^ (this->r[0] + this->S[1])) & 1) << (1 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[1]) ^ this->r[1];
        this->S[2] = a_mul(this->S[2]) ^ this->S[15] ^ ainv_mul(this->S[13]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[15];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[3] ^ this->r[1] ^ (this->r[0] + this->S[2])) & 1) << (2 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[2]) ^ this->r[1];
        this->S[3] = a_mul(this->S[3]) ^ this->S[0] ^ ainv_mul(this->S[14]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[0];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[4] ^ this->r[1] ^ (this->r[0] + this->S[3])) & 1) << (3 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[3]) ^ this->r[1];
        this->S[4] = a_mul(this->S[4]) ^ this->S[1] ^ ainv_mul(this->S[15]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[1];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[5] ^ this->r[1] ^ (this->r[0] + this->S[4])) & 1) << (4 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[4]) ^ this->r[1];
        this->S[5] = a_mul(this->S[5]) ^ this->S[2] ^ ainv_mul(this->S[0]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[2];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[6] ^ this->r[1] ^ (this->r[0] + this->S[5])) & 1) << (5 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[5]) ^ this->r[1];
        this->S[6] = a_mul(this->S[6]) ^ this->S[3] ^ ainv_mul(this->S[1]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[3];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[7] ^ this->r[1] ^ (this->r[0] + this->S[6])) & 1) << (6 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[6]) ^ this->r[1];
        this->S[7] = a_mul(this->S[7]) ^ this->S[4] ^ ainv_mul(this->S[2]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[4];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[8] ^ this->r[1] ^ (this->r[0] + this->S[7])) & 1) << (7 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[7]) ^ this->r[1];
        this->S[8] = a_mul(this->S[8]) ^ this->S[5] ^ ainv_mul(this->S[3]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[5];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[9] ^ this->r[1] ^ (this->r[0] + this->S[8])) & 1) << (8 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[8]) ^ this->r[1];
        this->S[9] = a_mul(this->S[9]) ^ this->S[6] ^ ainv_mul(this->S[4]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[6];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[10] ^ this->r[1] ^ (this->r[0] + this->S[9])) & 1) << (9 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[9]) ^ this->r[1];
        this->S[10] = a_mul(this->S[10]) ^ this->S[7] ^ ainv_mul(this->S[5]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[7];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[11] ^ this->r[1] ^ (this->r[0] + this->S[10])) & 1) << (10 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[10]) ^ this->r[1];
        this->S[11] = a_mul(this->S[11]) ^ this->S[8] ^ ainv_mul(this->S[6]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[8];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[12] ^ this->r[1] ^ (this->r[0] + this->S[11])) & 1) << (11 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[11]) ^ this->r[1];
        this->S[12] = a_mul(this->S[12]) ^ this->S[9] ^ ainv_mul(this->S[7]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[9];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[13] ^ this->r[1] ^ (this->r[0] + this->S[12])) & 1) << (12 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[12]) ^ this->r[1];
        this->S[13] = a_mul(this->S[13]) ^ this->S[10] ^ ainv_mul(this->S[8]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[10];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[14] ^ this->r[1] ^ (this->r[0] + this->S[13])) & 1) << (13 + i * 16);


        outfrom_fsm = (this->r[0] + this->S[13]) ^ this->r[1];
        this->S[14] = a_mul(this->S[14]) ^ this->S[11] ^ ainv_mul(this->S[9]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[11];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[15] ^ this->r[1] ^ (this->r[0] + this->S[14])) & 1) << (14 + i * 16);

        outfrom_fsm = (this->r[0] + this->S[14]) ^ this->r[1];
        this->S[15] = a_mul(this->S[15]) ^ this->S[12] ^ ainv_mul(this->S[10]) ^ outfrom_fsm;
        fsmtmp = this->r[1] + this->S[12];
        this->r[1] = T(this->r[0]);
        this->r[0] = fsmtmp;
        this->z_0 |= ((this->S[0] ^ this->r[1] ^ (this->r[0] + this->S[15])) & 1) << (15 + i * 16);
    }
    cout << "Z0 " << this->z_0 << endl;
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
        return dstu8845(S, r, key, 64, iv);
}

void dstu8845::print() const
{
    for(uint8_t i = 0; i < 16; i++)
    {
        cout << "S_" << (unsigned)i << ": 0x" << hex << uppercase << setfill('0') << setw(16) << this->S[i] << dec << nouppercase << "\n";
    }
    cout << "r_0: 0x" << hex << uppercase << setfill('0') << setw(16) << this->r[0] << dec << nouppercase << "\n";
    cout << "r_1: 0x" << hex << uppercase << setfill('0') << setw(16) << this->r[1] << dec << nouppercase << "\n";
}
    
void dstu8845::next_stream(uint64_t *out_stream)
{
    uint64_t fsmtmp;

    this->S[0] = a_mul(this->S[0]) ^ this->S[13] ^ ainv_mul(this->S[11]);
    fsmtmp = this->r[1] + this->S[13];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[0] = (this->r[0] + this->S[0]) ^ this->r[1] ^ this->S[1];

    this->S[1] = a_mul(this->S[1]) ^ this->S[14] ^ ainv_mul(this->S[12]);
    fsmtmp = this->r[1] + this->S[14];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[1] = (this->r[0] + this->S[1]) ^ this->r[1] ^ this->S[2];

    this->S[2] = a_mul(this->S[2]) ^ this->S[15] ^ ainv_mul(this->S[13]);
    fsmtmp = this->r[1] + this->S[15];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[2] = (this->r[0] + this->S[2]) ^ this->r[1] ^ this->S[3];

    this->S[3] = a_mul(this->S[3]) ^ this->S[0] ^ ainv_mul(this->S[14]);
    fsmtmp = this->r[1] + this->S[0];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[3] = (this->r[0] + this->S[3]) ^ this->r[1] ^ this->S[4];

    this->S[4] = a_mul(this->S[4]) ^ this->S[1] ^ ainv_mul(this->S[15]);
    fsmtmp = this->r[1] + this->S[1];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[4] = (this->r[0] + this->S[4]) ^ this->r[1] ^ this->S[5];

    this->S[5] = a_mul(this->S[5]) ^ this->S[2] ^ ainv_mul(this->S[0]);
    fsmtmp = this->r[1] + this->S[2];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[5] = (this->r[0] + this->S[5]) ^ this->r[1] ^ this->S[6];

    this->S[6] = a_mul(this->S[6]) ^ this->S[3] ^ ainv_mul(this->S[1]);
    fsmtmp = this->r[1] + this->S[3];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[6] = (this->r[0] + this->S[6]) ^ this->r[1] ^ this->S[7];

    this->S[7] = a_mul(this->S[7]) ^ this->S[4] ^ ainv_mul(this->S[2]);
    fsmtmp = this->r[1] + this->S[4];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[7] = (this->r[0] + this->S[7]) ^ this->r[1] ^ this->S[8];

    this->S[8] = a_mul(this->S[8]) ^ this->S[5] ^ ainv_mul(this->S[3]);
    fsmtmp = this->r[1] + this->S[5];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[8] = (this->r[0] + this->S[8]) ^ this->r[1] ^ this->S[9];

    this->S[9] = a_mul(this->S[9]) ^ this->S[6] ^ ainv_mul(this->S[4]);
    fsmtmp = this->r[1] + this->S[6];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[9] = (this->r[0] + this->S[9]) ^ this->r[1] ^ this->S[10];

    this->S[10] = a_mul(this->S[10]) ^ this->S[7] ^ ainv_mul(this->S[5]);
    fsmtmp = this->r[1] + this->S[7];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[10] = (this->r[0] + this->S[10]) ^ this->r[1] ^ this->S[11];

    this->S[11] = a_mul(this->S[11]) ^ this->S[8] ^ ainv_mul(this->S[6]);
    fsmtmp = this->r[1] + this->S[8];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[11] = (this->r[0] + this->S[11]) ^ this->r[1] ^ this->S[12];

    this->S[12] = a_mul(this->S[12]) ^ this->S[9] ^ ainv_mul(this->S[7]);
    fsmtmp = this->r[1] + this->S[9];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[12] = (this->r[0] + this->S[12]) ^ this->r[1] ^ this->S[13];

    this->S[13] = a_mul(this->S[13]) ^ this->S[10] ^ ainv_mul(this->S[8]);
    fsmtmp = this->r[1] + this->S[10];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[13] = (this->r[0] + this->S[13]) ^ this->r[1] ^ this->S[14];

    this->S[14] = a_mul(this->S[14]) ^ this->S[11] ^ ainv_mul(this->S[9]);
    fsmtmp = this->r[1] + this->S[11];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[14] = (this->r[0] + this->S[14]) ^ this->r[1] ^ this->S[15];

    this->S[15] = a_mul(this->S[15]) ^ this->S[12] ^ ainv_mul(this->S[10]);
    fsmtmp = this->r[1] + this->S[12];
    this->r[1] = T(this->r[0]);
    this->r[0] = fsmtmp;
    out_stream[15] = (this->r[0] + this->S[15]) ^ this->r[1] ^ this->S[0];
}

void dstu8845::next_stream_full_crypt(uint64_t * __restrict in, uint64_t * __restrict out)
{
    uint64_t fsmtmp;

    this->S[0] = this->a_mul(this->S[0]) ^ this->S[13] ^ this->ainv_mul(this->S[11]);
    fsmtmp = this->r[1] + this->S[13];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[0] = in[0] ^ (this->r[0] + this->S[0]) ^ this->r[1] ^ this->S[1];

    this->S[1] = this->a_mul(this->S[1]) ^ this->S[14] ^ this->ainv_mul(this->S[12]);
    fsmtmp = this->r[1] + this->S[14];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[1] = in[1] ^ (this->r[0] + this->S[1]) ^ this->r[1] ^ this->S[2];

    this->S[2] = this->a_mul(this->S[2]) ^ this->S[15] ^ this->ainv_mul(this->S[13]);
    fsmtmp = this->r[1] + this->S[15];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[2] = in[2] ^ (this->r[0] + this->S[2]) ^ this->r[1] ^ this->S[3];

    this->S[3] = this->a_mul(this->S[3]) ^ this->S[0] ^ this->ainv_mul(this->S[14]);
    fsmtmp = this->r[1] + this->S[0];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[3] = in[3] ^ (this->r[0] + this->S[3]) ^ this->r[1] ^ this->S[4];

    this->S[4] = this->a_mul(this->S[4]) ^ this->S[1] ^ this->ainv_mul(this->S[15]);
    fsmtmp = this->r[1] + this->S[1];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[4] = in[4] ^ (this->r[0] + this->S[4]) ^ this->r[1] ^ this->S[5];

    this->S[5] = this->a_mul(this->S[5]) ^ this->S[2] ^ this->ainv_mul(this->S[0]);
    fsmtmp = this->r[1] + this->S[2];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[5] = in[5] ^ (this->r[0] + this->S[5]) ^ this->r[1] ^ this->S[6];

    this->S[6] = this->a_mul(this->S[6]) ^ this->S[3] ^ this->ainv_mul(this->S[1]);
    fsmtmp = this->r[1] + this->S[3];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[6] = in[6] ^ (this->r[0] + this->S[6]) ^ this->r[1] ^ this->S[7];

    this->S[7] = this->a_mul(this->S[7]) ^ this->S[4] ^ this->ainv_mul(this->S[2]);
    fsmtmp = this->r[1] + this->S[4];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[7] = in[7] ^ (this->r[0] + this->S[7]) ^ this->r[1] ^ this->S[8];

    this->S[8] = this->a_mul(this->S[8]) ^ this->S[5] ^ this->ainv_mul(this->S[3]);
    fsmtmp = this->r[1] + this->S[5];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[8] = in[8] ^ (this->r[0] + this->S[8]) ^ this->r[1] ^ this->S[9];

    this->S[9] = this->a_mul(this->S[9]) ^ this->S[6] ^ this->ainv_mul(this->S[4]);
    fsmtmp = this->r[1] + this->S[6];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[9] = in[9] ^ (this->r[0] + this->S[9]) ^ this->r[1] ^ this->S[10];

    this->S[10] = this->a_mul(this->S[10]) ^ this->S[7] ^ this->ainv_mul(this->S[5]);
    fsmtmp = this->r[1] + this->S[7];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[10] = in[10] ^ (this->r[0] + this->S[10]) ^ this->r[1] ^ this->S[11];

    this->S[11] = this->a_mul(this->S[11]) ^ this->S[8] ^ this->ainv_mul(this->S[6]);
    fsmtmp = this->r[1] + this->S[8];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[11] = in[11] ^ (this->r[0] + this->S[11]) ^ this->r[1] ^ this->S[12];

    this->S[12] = this->a_mul(this->S[12]) ^ this->S[9] ^ this->ainv_mul(this->S[7]);
    fsmtmp = this->r[1] + this->S[9];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[12] = in[12] ^ (this->r[0] + this->S[12]) ^ this->r[1] ^ this->S[13];

    this->S[13] = this->a_mul(this->S[13]) ^ this->S[10] ^ this->ainv_mul(this->S[8]);
    fsmtmp = this->r[1] + this->S[10];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[13] = in[13] ^ (this->r[0] + this->S[13]) ^ this->r[1] ^ this->S[14];

    this->S[14] = this->a_mul(this->S[14]) ^ this->S[11] ^ this->ainv_mul(this->S[9]);
    fsmtmp = this->r[1] + this->S[11];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[14] = in[14] ^ (this->r[0] + this->S[14]) ^ this->r[1] ^ this->S[15];

    this->S[15] = this->a_mul(this->S[15]) ^ this->S[12] ^ this->ainv_mul(this->S[10]);
    fsmtmp = this->r[1] + this->S[12];
    this->r[1] = this->T(this->r[0]);
    this->r[0] = fsmtmp;
    out[15] = in[15] ^ (this->r[0] + this->S[15]) ^ this->r[1] ^ this->S[0];
}


void dstu8845::dstu8845_crypt(const uint8_t * __restrict in, uint8_t * __restrict out, uint64_t inl)
{
    uint8_t i;
    uint64_t keystream[16];

    for(; inl >= 128; inl -= 128, in += 128, out += 128)
    {
        this->next_stream_full_crypt((uint64_t *)in, (uint64_t *)out);
    }

    if(inl > 0)
    {
        this->next_stream(keystream);

        for(i = 0; i < inl; i++)
        {
            out[i] = in[i] ^ ((uint8_t *)keystream)[i];
        }
    }
}
