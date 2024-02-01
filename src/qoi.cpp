#include "qoi.h"
#include <fstream>
#include <iostream>

#define Wr(file, var) for(char* it = ((char*)&var)+sizeof(var)-1; it>=((char*)&var); it--){file.write(it, 1);}
#define Re(file, var) for(char* it = ((char*)&var)+sizeof(var)-1; it>=((char*)&var); it--){file.read(it, 1);}



u8 getIndex(const char* c)
{
    return ((u8)c[0]*3u + (u8)c[1]*5u + (u8)c[2]*7u + (u8)c[3]*11u)%64u;
}

void QOI::Header::Read(std::ifstream& file)
{
    u32 magic_check;
    Re(file, magic_check);
    if (magic_check != magic)
        throw std::invalid_argument("invalid file format");

    Re(file, width);
    Re(file, height);
    Re(file, channels);
    Re(file, colorspace);

    WxH = width * height;
}

void QOI::Header::Write(std::ofstream& file)
{
    Wr(file, magic);
    Wr(file, width);
    Wr(file, height);
    Wr(file, channels);
    Wr(file, colorspace);
}

void QOI::Write(const char* path, Header& header, char**& chain)
{
    std::ofstream file (path, std::ios::binary | std::ios::out);

    header.Write(file);

    for (u32 i = 0u; i<header.WxH; i++)
    {
        Wr(file, flag8bit::rgb);
        file.write(chain[i], 3);
    }

    Wr(file, eof);

    file.close();
}


void QOI::Read(const char* path, Header& header, char**& chain)
{
    clear2dim(chain, header.WxH); // clear chain

    std::ifstream file (path, std::ios::binary | std::ios::out);

    header.Read(file);
    chain = new char*[header.WxH]; // resize chain

    char* lookupTable[64];
    for (auto & i : lookupTable)
    {
        i = (char*)nullColor;
    }

    u8 byte = 0x00, flag = 0x00, arg = 0x00;
    for (u32 i = 0u; i<header.WxH; i++)
    {
        Re(file, byte);

        // check for 8 bit flag
        if (byte == flag8bit::rgb)
        {
            //std::cout << i << " : " <<"rgb" << std::endl;
            chain[i] = new char[4];
            file.read(chain[i], 3);
            chain[i][3] = i!=0 ? chain[i-1][3] : (char)0xff;

            lookupTable[getIndex(chain[i])] = chain[i];
            continue;
        }
        else if (byte == flag8bit::rgba)
        {
            //std::cout << i << " : " <<"rgba" << std::endl;
            chain[i] = new char[4];
            file.read(chain[i], 4);

            lookupTable[getIndex(chain[i])] = chain[i];
            continue;
        }

        // check for 2 bit flag
        flag = byte & 0b11000000;
        arg  = byte & 0b00111111;
        if (flag == flag2bit::run)
        {
            //std::cout << i << " : " <<"run" << std::endl;
            for (u8 j = 0; j<=arg; j++) // <= because run length is stored with a bias of -1
            {
                chain[i+j] = chain[i-1]; // copying the pointer instead of allocating more memory
                // don't need to update the lookup table since the key is the same
            }

            i+= arg;
            continue;
        }
        else if (flag == flag2bit::diff)
        {
            //std::cout << i << " : " <<"diff" << std::endl;
            char dr, dg, db;
            db = (char)((arg&0b11)-2);
            dg = (char)(((arg>>2)&0b11)-2);
            dr = (char)(((arg>>4)&0b11)-2);

            chain[i] = new char[4];

            chain[i][0] = (char)(chain[i-1][0] + dr);
            chain[i][1] = (char)(chain[i-1][1] + dg);
            chain[i][2] = (char)(chain[i-1][2] + db);
            chain[i][3] = chain[i-1][3];

            lookupTable[getIndex(chain[i])] = chain[i];
            continue;
        }
        else if (flag == flag2bit::index)
        {
            //std::cout << i << " : " <<"index" << std::endl;
            chain[i] = lookupTable[arg];

            continue;
        }
        else if (flag == flag2bit::luma)
        {
            //std::cout << i << " : " <<"luma" << std::endl;
            char dg, dr_dg, db_dg;
            dg = (char)(arg - 32);

            Re(file, byte);

            db_dg = (char)(byte&0b1111 - 8);
            dr_dg = (char)((byte>>4)&0b1111 - 8);

            chain[i] = new char[4];

            chain[i][1] = (char)(chain[i-1][1] + dg);
            chain[i][0] = (char)(chain[i-1][0] + dr_dg + dg);
            chain[i][2] = (char)(chain[i-1][2] + db_dg + dg);
            chain[i][3] = chain[i-1][3];

            lookupTable[getIndex(chain[i])] = chain[i];
            continue;
        }
    }

    u64 eof_check;
    Re(file, eof_check);
    if (eof_check != eof)
        throw std::invalid_argument("invalid file format");

    //std::cout << (u8)chain[17035][2] << " / " << (u64)nullColor << std::endl;

    file.close();
}