#include "qoi.h"
#include <fstream>

#define Wr(file, var) for(char* it = ((char*)&var)+sizeof(var)-1; it>=((char*)&var); it--){file.write(it, 1);}
#define Re(file, var) for(char* it = ((char*)&var)+sizeof(var)-1; it>=((char*)&var); it--){file.read(it, 1);}

void QOI::Header::Read(std::ifstream& file)
{
    Re(file, magic);
    Re(file, width);
    Re(file, height);
    Re(file, channels);
    Re(file, colorspace);
}

void QOI::Header::Write(std::ofstream& file)
{
    Wr(file, magic);
    Wr(file, width);
    Wr(file, height);
    Wr(file, channels);
    Wr(file, colorspace);
}

void QOI::Write(char* path, Header& header, char** chain)
{
    std::ofstream file (path, std::ios::binary | std::ios::out);

    header.Write(file);

    for (u32 i = 0u; i<header.width * header.height; i++)
    {
        Wr(file, flag8bit::rgb);
        file.write(chain[i], 3);
    }

    Wr(file, eof);

    file.close();
}