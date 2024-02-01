#include <iostream>
#include <fstream>
#include "qoi.h"

using namespace std;


unsigned readNum(ifstream& file)
{
    unsigned num;
    char c;
    num = 0;
    file.read(&c, 1);
    do {
        num *= 10;
        num += c - '0';
        file.read(&c, 1);
    }
    while (c >= '0' && c <= '9');

    return num;
}

char** chain;
QOI::Header header;



void load_ppm(const char* path)
{
    //clear2dim(chain, header.WxH);

    // init header
    header.channels = 3u;
    header.colorspace = 0u;

    ifstream file(path, ios::binary | ios::in);

    //load magic num, height, width and encoded pixel size
    char magicNum[4];
    file.read(magicNum, 3); // 3 = sizeof ("P6\n")

    header.width = readNum(file);

    header.height = readNum(file);

    unsigned encodedPixelSize;
    encodedPixelSize = readNum(file);

    // resize chain
    chain = new char* [header.WxH];

    // fill chain
    for (unsigned long long index = 0; index < header.WxH; index++)
    {
        chain[index] = new char[4];

        file.read(chain[index], 3);

    }

    file.close();

}



const char palette[8] = { ' ','.', ':', ';', '/', 'X', '&', '@' };


u32 getPaletteIndex(const char* ptr)
{
    u32 sum = (u8)(ptr[0]) + (u8)(ptr[1]) + (u8)(ptr[2]);
    return (((u8)ptr[3]) * sum)/24576u ;
}


void display_chain()
{
    for (unsigned long long i = 0; i< header.height; i+=3)
    {
        for (unsigned long long j = 0; j< header.width; j++)
        {
            cout << palette[getPaletteIndex(chain[j + header.width * i])];
        }
        cout << endl;
    }
}


void write_chain(const char* path)
{
    ofstream file(path);

    for (u32 i = 0; i< header.height; i+=3)
    {
        for (u32 j = 0; j< header.width; j++)
        {
            //cout << getPaletteIndex(chain[j + header.width * i]) << endl;
            file << palette[getPaletteIndex(chain[j + header.width * i])];
        }
        file << '\n';
    }

    file.close();
}


int main()
{

    load_ppm("images/test.ppm");


    QOI::Read("images/file.qoi", header, chain);

    //cout << (int)(u8)chain[17035][2] << " / " << (u64)QOI::nullColor << endl;

    write_chain("images/img.txt");


    clear2Dvec(chain, header.WxH)

    return 0;
}
