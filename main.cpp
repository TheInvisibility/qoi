#include <iostream>
#include <fstream>
#include "qoi.h"

#define clearChain for(int i = 0; i<header.height*header.width; i++){delete[] chain[i];} delete[] chain;

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
QOI::header header;



void load_ppm(const string& path)
{
    // init header
    header.magic; //???????
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
    chain = new char* [header.height * header.width];

    // fill chain
    for (unsigned long long index = 0; index < header.height * header.width; index++)
    {
        chain[index] = new char[3];

        file.read((char*)&chain[index][0], 1);
        file.read((char*)&chain[index][1], 1);
        file.read((char*)&chain[index][2], 1);

    }

    file.close();

}



const char palette[8] = { ' ','.', ':', ';', '/', 'X', '&', '@' };


unsigned get_index(const char* ptr)
{

    unsigned sum = (unsigned char)(ptr[0]) + (unsigned char)(ptr[1]) + (unsigned char)(ptr[2]);

    return sum / 96;
}


void display_chain()
{
    for (unsigned long long i = 0; i< header.height; i+=3)
    {
        for (unsigned long long j = 0; j< header.width; j++)
        {
            cout << palette[get_index(chain[j + header.width * i])];
        }
        cout << endl;
    }
}


void write_chain(string path)
{
    ofstream file(path);

    for (unsigned long long i = 0; i< header.height; i+=3)
    {
        for (unsigned long long j = 0; j< header.width; j++)
        {
            file << palette[get_index(chain[j + header.width * i])];
        }
        file << '\n';
    }

    file.close();
}


int main()
{
    load_ppm("images/test.ppm");


    //display_chain();
    write_chain("images/img.txt");

    clearChain
    return 0;
}
