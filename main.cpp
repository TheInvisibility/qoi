#include <iostream>
#include <fstream>

#define clearChain for(int i = 0; i<height*width; i++){delete[] chain[i];} delete[] chain;

using namespace std;

char** chain;
unsigned long long height, width;
char magicNum[4];
unsigned encodedPixelSize;

void load_ppm(const string& path)
{

    ifstream file(path, ios::binary | ios::in);



    //load height and width
    file.read(magicNum, 3); // 3 = sizeof ("P6\n")

    char c;

    width = 0;
    file.read(&c, 1);
    do
    {
        width *= 10;
        width += c-'0';
        file.read(&c, 1);
    }while (c!=' ');

    height = 0;
    file.read(&c, 1);
    do
    {
        height *= 10;
        height += c-'0';
        file.read(&c, 1);
    }while (c!='\n');


    encodedPixelSize = 0;
    file.read(&c, 1);
    do
    {
        encodedPixelSize *= 10;
        encodedPixelSize += c-'0';
        file.read(&c, 1);
    }while (c!='\n');

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



const char palette[8] = { ' ','.', ':', ';', '/', 'X', '&', '@' };


unsigned get_index(const char* ptr)
{

    unsigned sum = (unsigned char)(ptr[0]) + (unsigned char)(ptr[1]) + (unsigned char)(ptr[2]);

    return sum / 96; // 96 = 3*(255/
}


void display_chain()
{
    for (unsigned long long i = 0; i< height; i+=3)
    {
        for (unsigned long long j = 0; j< width; j++)
        {
            cout << palette[get_index(chain[j + width * i])];
        }
        cout << endl;
    }
}


void write_chain(string path)
{
    ofstream file(path);

    for (unsigned long long i = 0; i< height; i+=3)
    {
        for (unsigned long long j = 0; j< width; j++)
        {
            file << palette[get_index(chain[j + width * i])];
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
