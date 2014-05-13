/*
   run time on AMD Phenom x3 @2.3Ghz/Core: ~1.2 seconds
   + OMP_NUM_THREADS = 4
   + including memory allocations, not including I/O

   run time with OMP_NUM_THREADS: ~2.3 seconds

   no duplicates were generated in those 500000 entries. (or the script used to check for dups was broken)
   Coupled with the fact that these hashes will be appended to an already quasi unique name, the resulting name should be globally unique with great confidence

   the parallelisation is halves the time for 500k+ names; could be improved by tinkering with fine tuning the parallelisation and vectorisation
   the parallelisation is unnecesary for <10k names
*/
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <string>
#include <list>
#include <omp.h>

extern "C" {
void xPear16(void const*, size_t, uint64_t*);
} // extern C

// doesn't account for endianness
std::string to64(uint64_t const& hash)
{
    char const b64alphabet[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_', /**/
    };

    uint8_t const* phash = reinterpret_cast<uint8_t const*>(&hash);
    uint8_t const* ps[] = { phash, phash + 4 };
    uint8_t a = 0;
    char s[9];
    for(size_t i = 0; i < 2; ++i) {
        uint8_t const* p = ps[i];
        // 0-6      / 0
        a = (*p >> 2) & 0x3F;
        s[i * 4 + 0] = b64alphabet[a];
        // 6-12     / 0-1
        a = ((*p & 0x3) << 4) | ((*(p + 1) >> 4) & 0x0F);
        s[i * 4 + 1] = b64alphabet[a];
        ++p;
        // 12-18    / 1-2
        a = (*p & 0xF) | ((*(p + 1) >> 2) & 0x30);
        s[i * 4 + 2] = b64alphabet[a];
        ++p;
        // 18-24    / 2
        a = (*p & 0x3F);
        s[i * 4 + 3] = b64alphabet[a];
        ++p;
    }
    s[8] = '\0';

    return std::string(s);
}

int main(int argc, char* argv[])
{
#define SIZE (500000)
    char* names[SIZE];
    memset(names, 0, SIZE);

    for(size_t i = 0; i < SIZE; ++i) {
        names[i] = (char*)malloc(sizeof(char) * 40);
    }

#   pragma omp parallel for
    for(size_t i = 0; i < SIZE; ++i) {
        std::stringstream ss;
        ss << "A Certain Recipe " << i << " - Generic Name " << i;
        uint64_t hash = 0;
        xPear16(ss.str().c_str(), ss.str().length(), &hash);
        std::string s = to64(hash);
        strcpy(names[i], s.c_str());
    }

    for(size_t i = 0; i < SIZE; ++i) {
        std::cout << names[i] << std::endl;
        free(names[i]);
    }

    return 0;
}
