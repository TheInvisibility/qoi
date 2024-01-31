#include "qoi.h"
#include <fstream>

void QOI::Header::Read(std::ifstream file)
{
    file.read(magic, sizeof(magic));
    file.read((char*)&width, sizeof(u32));
    file.read((char*)&height, sizeof(u32));
    file.read((char*)&channels, sizeof(u8));
    file.read((char*)&colorspace, sizeof(u8));
}



void QOI::Header::Write(std::ofstream file)
{
    file.write(magic, sizeof(magic));
    file.write((char*)&width, sizeof(u32));
    file.write((char*)&height, sizeof(u32));
    file.write((char*)&channels, sizeof(u8));
    file.write((char*)&colorspace, sizeof(u8));
}

void QOI::Write(char* path, Header& header, char** chain)
{

}