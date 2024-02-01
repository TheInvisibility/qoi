#ifndef TEST_QOI_H
#define TEST_QOI_H

#include <fstream>

#define u64 unsigned long long
#define u32 unsigned
#define u8 unsigned char

namespace QOI {
    const u32 magic = 0x716f6966 ; // octets de magie "qoif".
    const u64 eof = 0x0000000000000001;//8 bytes de fin

    namespace flag8bit {
        const u8 rgb = 0b11111110;
        const u8 rgba = 0b11111111;
    }

    namespace flag2bit {
        const u8 index = 0b00000000;
        const u8 diff = 0b01000000;
        const u8 luma = 0b10000000;
        const u8 run = 0b11000000;
    }



    struct Header {
        u32 width ; // largeur de l'image en pixels (BE)
        u32 height ; // hauteur de l'image en pixels (BE)
        u8 channels ; // 3 = RGB, 4 = RGBA
        u8 colorspace ; // 0 = sRGB avec alpha linéaire
                        // 1 = tous les canaux sont linéaires

        void Read(std::ifstream& file);
        void Write(std::ofstream& file);
    };


    void Write(char* path, Header& header, char** chain);
    void Read(char* path, Header& header, char** chain);
}

#endif
