#include "strumok.cpp"
#include <cstdlib>

int main()
{
  uint64_t key[8];
  uint64_t iv[4];
  uint8_t key_size = 64;
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

  dstu8845 cipher(key, key_size, iv, true, false);
  return 0;
}
