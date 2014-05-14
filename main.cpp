#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdint>

extern "C" {
void xPear16(void const*, size_t, uint64_t*);
} // extern C

// doesn't account for endianness
void print64(uint64_t const& hash)
{
    char const b64alphabet[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_', /**/
    };

    uint8_t const* p = reinterpret_cast<uint8_t const*>(&hash);
    uint8_t a = 0;
    for(size_t i = 0; i < 2; ++i) {
        // 0-6      / 0
        a = (*p >> 2) & 0x3F;
        std::cout << b64alphabet[a];
        // 6-12     / 0-1
        a = ((*p & 0x3) << 4) | ((*(p + 1) >> 4) & 0x0F);
        std::cout << b64alphabet[a];
        ++p;
        // 12-18    / 1-2
        a = (*p & 0xF) | ((*(p + 1) >> 2) & 0x30);
        std::cout << b64alphabet[a];
        ++p;
        // 18-24    / 2
        a = (*p & 0x3F);
        std::cout << b64alphabet[a];
        ++p;
    }
    // 48-54 6
    a = (*p >> 2) & 0x3F;
    std::cout << b64alphabet[a];
    // 54-60 6-7
    a = ((*p & 0x3) << 4) | ((*(p + 1) >> 4) & 0x0F);
    std::cout << b64alphabet[a];
    ++p;
    // 60 7
    a = (*p & 0xF);
    std::cout << b64alphabet[a];
}

int main(int argc, char* argv[])
{
    std::stringstream ss;
    while(!std::cin.eof()) {
        char c = 0;
        std::cin.get(c);
        ss << c;
    }

    uint64_t hash = 0;
    xPear16(ss.str().c_str(), ss.str().length(), &hash);

    std::cout << std::hex << hash << std::endl;

    print64(hash);
    std::cout << std::endl;

    return 0;
}
