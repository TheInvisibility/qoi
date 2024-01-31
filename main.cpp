#include <iostream>
#include <fstream>


#define clearChaine

using namespace std;

char** chain;
unsigned long long height, width;

void load_ppm(string path)
{
    ifstream file(path, ios::binary | ios::in);

    //load height and width
    ifstream ascii_text; ascii_text.open(path);

    string magic_num;

    ascii_text >> magic_num >> height >> width;

// resize chain
    chain = new char* [height * width];

// fill chain
    for (unsigned long long index = 0; index < height * width; index++)
    {
        chain[index] = new char[3];

        file.read((char*)&chain[index][0], 1);
        file.read((char*)&chain[index][1], 1);
        file.read((char*)&chain[index][2], 1);

    }

    file.close();
}



const char palette[8] = { ' ', '.', ',', '*', '/', '%', '@' };


unsigned get_index(char* ptr)
{

    unsigned sum = (unsigned)ptr[0] + (unsigned)ptr[1] + (unsigned)ptr[2];

    if (sum/96 > 4473)
        sum = 255;
    return sum / 96;
}


int main()
{
    load_ppm("test.ppm");


    ofstream file("test.bin", ios::binary | ios::out);


    for (unsigned long long i = 0; i< height * width; i++)
    {
        if (i%width == 0)
            cout << endl;
        cout << palette[get_index(chain[i])];

//file.write((char*)cursor, 1);
    }

    file.close();

    return 0;
}
