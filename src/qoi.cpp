#include "qoi.h"
#include <fstream>

#define Wr(file, ptr, size) for(char* it = ptr+size-1; it>=ptr; it--){file.write(it, 1);}

void QOI::Header::Read(std::ifstream& file)
{
    file.read((char*)&magic, sizeof(magic));
    file.read((char*)&width, sizeof(u32));
    file.read((char*)&height, sizeof(u32));
    file.read((char*)&channels, sizeof(u8));
    file.read((char*)&colorspace, sizeof(u8));
}



void QOI::Header::Write(std::ofstream& file)
{
    Wr(file, (char*)&magic, sizeof(magic));
    Wr(file, (char*)&width, sizeof(width));
    Wr(file, (char*)&height, sizeof(height));
    Wr(file, (char*)&channels, sizeof(channels));
    Wr(file, (char*)&colorspace, sizeof(colorspace));
}

void QOI::Write(char* path, Header& header, char** chain)
{
    std::ofstream file (path, std::ios::binary | std::ios::out);

    header.Write(file);

    for (u32 i = 0u; i<header.width * header.height; i++)
    {
        file.write(&flag8bit::rgb, 1);
        Wr(file, chain[i], 3);
    }

    Wr(file, (char*)&eof, sizeof(eof));


    file.close();
}