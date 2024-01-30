#ifndef TEST_QOI_H
#define TEST_QOI_H

namespace qoi {
    namespace flag {
        const char rgb = 0b11111110;
        const char rgba = 0b11111111;
    }

    namespace tag {
        const char index = 0b00000000;
        const char diff = 0b01000000;
        const char luma = 0b10000000;
        const char run = 0b11000000;
    }
}

#endif
