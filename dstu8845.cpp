#include "dstu8845.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#if defined(__x86_64__) || defined(_M_X64)
    #include <immintrin.h>
#elif defined(__aarch64__) || defined(_M_ARM64)
    #include "sse2neon.h"
#endif
using namespace std;

void dstu8845::print() const
{
    for(uint8_t i = 0; i < 16; i++)
    {
        cout << "S_" << (unsigned)i << ": 0x" << hex << uppercase << setfill('0') << setw(16) << this->S[i] << dec << nouppercase << "\n";
    }
    cout << "r_0: 0x" << hex << uppercase << setfill('0') << setw(16) << this->r[0] << dec << nouppercase << "\n";
    cout << "r_1: 0x" << hex << uppercase << setfill('0') << setw(16) << this->r[1] << dec << nouppercase << "\n";
    cout << "Z_0: 0x" << hex << uppercase << setfill('0') << setw(16) << this->z_0 << dec << nouppercase << "\n";
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

void dstu8845::dstu8845_crypt(const uint8_t * __restrict in, uint8_t * __restrict out, uint64_t inl)
{
    uint64_t l_S[16];
    uint64_t l_r[2];

    __builtin_memcpy(l_S, this->S, 128);
    __builtin_memcpy(l_r, this->r, 16);

    const uint64_t * __restrict in64 = (const uint64_t*)__builtin_assume_aligned(in, 64);
    uint64_t * __restrict out64 = (uint64_t*)__builtin_assume_aligned(out, 64);
    
    uint64_t blocks = inl / 128;

    while (blocks--)
    {
        __builtin_prefetch(in64 + 32, 0, 0); 
        __builtin_prefetch(out64 + 32, 1, 0);
        
        uint64_t fsmtmp;

        l_S[0] = a_mul(l_S[0]) ^ l_S[13] ^ ainv_mul(l_S[11]);
        fsmtmp = l_r[1] + l_S[13];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks0 = l_S[1] ^ (l_r[0] + l_S[0]) ^ l_r[1];

        l_S[1] = a_mul(l_S[1]) ^ l_S[14] ^ ainv_mul(l_S[12]);
        fsmtmp = l_r[1] + l_S[14];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks1 = l_S[2] ^ (l_r[0] + l_S[1]) ^ l_r[1];

        l_S[2] = a_mul(l_S[2]) ^ l_S[15] ^ ainv_mul(l_S[13]);
        fsmtmp = l_r[1] + l_S[15];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks2 = l_S[3] ^ (l_r[0] + l_S[2]) ^ l_r[1];

        l_S[3] = a_mul(l_S[3]) ^ l_S[0] ^ ainv_mul(l_S[14]);
        fsmtmp = l_r[1] + l_S[0];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks3 = l_S[4] ^ (l_r[0] + l_S[3]) ^ l_r[1];

        __m256i k_vec0 = _mm256_set_epi64x(ks3, ks2, ks1, ks0);
        __m256i in_vec0 = _mm256_loadu_si256((const __m256i*)(in64));
        _mm256_storeu_si256((__m256i*)(out64), _mm256_xor_si256(in_vec0, k_vec0));

        l_S[4] = a_mul(l_S[4]) ^ l_S[1] ^ ainv_mul(l_S[15]);
        fsmtmp = l_r[1] + l_S[1];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks4 = l_S[5] ^ (l_r[0] + l_S[4]) ^ l_r[1];

        l_S[5] = a_mul(l_S[5]) ^ l_S[2] ^ ainv_mul(l_S[0]);
        fsmtmp = l_r[1] + l_S[2];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks5 = l_S[6] ^ (l_r[0] + l_S[5]) ^ l_r[1];

        l_S[6] = a_mul(l_S[6]) ^ l_S[3] ^ ainv_mul(l_S[1]);
        fsmtmp = l_r[1] + l_S[3];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks6 = l_S[7] ^ (l_r[0] + l_S[6]) ^ l_r[1];

        l_S[7] = a_mul(l_S[7]) ^ l_S[4] ^ ainv_mul(l_S[2]);
        fsmtmp = l_r[1] + l_S[4];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks7 = l_S[8] ^ (l_r[0] + l_S[7]) ^ l_r[1];

        __m256i k_vec1 = _mm256_set_epi64x(ks7, ks6, ks5, ks4);
        __m256i in_vec1 = _mm256_loadu_si256((const __m256i*)(in64 + 4));
        _mm256_storeu_si256((__m256i*)(out64 + 4), _mm256_xor_si256(in_vec1, k_vec1));

        l_S[8] = a_mul(l_S[8]) ^ l_S[5] ^ ainv_mul(l_S[3]);
        fsmtmp = l_r[1] + l_S[5];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks8 = l_S[9] ^ (l_r[0] + l_S[8]) ^ l_r[1];

        l_S[9] = a_mul(l_S[9]) ^ l_S[6] ^ ainv_mul(l_S[4]);
        fsmtmp = l_r[1] + l_S[6];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks9 = l_S[10] ^ (l_r[0] + l_S[9]) ^ l_r[1];

        l_S[10] = a_mul(l_S[10]) ^ l_S[7] ^ ainv_mul(l_S[5]);
        fsmtmp = l_r[1] + l_S[7];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks10 = l_S[11] ^ (l_r[0] + l_S[10]) ^ l_r[1];

        l_S[11] = a_mul(l_S[11]) ^ l_S[8] ^ ainv_mul(l_S[6]);
        fsmtmp = l_r[1] + l_S[8];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks11 = l_S[12] ^ (l_r[0] + l_S[11]) ^ l_r[1];

        __m256i k_vec2 = _mm256_set_epi64x(ks11, ks10, ks9, ks8);
        __m256i in_vec2 = _mm256_loadu_si256((const __m256i*)(in64 + 8));
        _mm256_storeu_si256((__m256i*)(out64 + 8), _mm256_xor_si256(in_vec2, k_vec2));

        l_S[12] = a_mul(l_S[12]) ^ l_S[9] ^ ainv_mul(l_S[7]);
        fsmtmp = l_r[1] + l_S[9];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks12 = l_S[13] ^ (l_r[0] + l_S[12]) ^ l_r[1];

        l_S[13] = a_mul(l_S[13]) ^ l_S[10] ^ ainv_mul(l_S[8]);
        fsmtmp = l_r[1] + l_S[10];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks13 = l_S[14] ^ (l_r[0] + l_S[13]) ^ l_r[1];

        l_S[14] = a_mul(l_S[14]) ^ l_S[11] ^ ainv_mul(l_S[9]);
        fsmtmp = l_r[1] + l_S[11];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks14 = l_S[15] ^ (l_r[0] + l_S[14]) ^ l_r[1];

        l_S[15] = a_mul(l_S[15]) ^ l_S[12] ^ ainv_mul(l_S[10]);
        fsmtmp = l_r[1] + l_S[12];
        l_r[1] = T(l_r[0]);
        l_r[0] = fsmtmp;
        uint64_t ks15 = l_S[0] ^ (l_r[0] + l_S[15]) ^ l_r[1];

        __m256i k_vec3 = _mm256_set_epi64x(ks15, ks14, ks13, ks12);
        __m256i in_vec3 = _mm256_loadu_si256((const __m256i*)(in64 + 12));
        _mm256_storeu_si256((__m256i*)(out64 + 12), _mm256_xor_si256(in_vec3, k_vec3));

        in64 += 16;
        out64 += 16;
    }

    __builtin_memcpy(this->S, l_S, 128);
    __builtin_memcpy(this->r, l_r, 16);
    
    uint8_t tail = inl % 128;
    if(tail > 0)
    {
        uint64_t keystream[16];
        this->next_stream(keystream); 

        uint64_t offset = inl - tail;
        uint64_t words = tail / 8;
        uint8_t bytes = tail % 8;

        const uint64_t *in_tail64 = reinterpret_cast<const uint64_t*>(in + offset);
        uint64_t *out_tail64 = reinterpret_cast<uint64_t*>(out + offset);

        for(uint64_t i = 0; i < words; ++i)
        {
            out_tail64[i] = in_tail64[i] ^ keystream[i];
        }

        if(bytes > 0) {
            const uint8_t *in_tail8 = in + offset + words * 8;
            uint8_t *out_tail8 = out + offset + words * 8;
            const uint8_t *ks8 = reinterpret_cast<const uint8_t*>(keystream) + words * 8;

            for(uint8_t i = 0; i < bytes; ++i)
            {
                out_tail8[i] = in_tail8[i] ^ ks8[i];
            }
        }
    }
}
