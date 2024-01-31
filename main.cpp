#include <iostream>
#include <fstream>

#define clearChain for(int i = 0; i<height*width; i++){delete[] chain[i];} delete[] chain;

using namespace std;

char** chain;
unsigned long long height, width;
char magicNum[4];

void load_ppm(string path)
{

    ifstream file(path, ios::binary | ios::in);



    //load height and width
    file.read(magicNum, 3); // 4 = sizeof ("P16\n")



    char c;
    height = 0;
    file.read(&c, 1);
    do
    {
        height *= 10;
        height += c-'0';
        file.read(&c, 1);
    }while (c!=' ');


    width = 0;
    file.read(&c, 1);
    do
    {
        width *= 10;
        width += c-'0';
        file.read(&c, 1);
    }while (c!='\n');

    cout << "height > " << height << endl;
    cout << "width > " << width;

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



const char palette[8] = { ' ', '.', ':', '*', '/', '%', '@' };


unsigned get_index(char* ptr)
{

    unsigned sum = (unsigned char)(ptr[0]) + (unsigned char)(ptr[1]) + (unsigned char)(ptr[2]);

    return sum / 96;
}


int main()
{
    load_ppm("moreboxes_2.ppm");

    ofstream file("test.bin", ios::binary | ios::out);


    for (unsigned long long i = 0; i< height * width; i++)
    {
        if (i%width == 0)
            cout << endl;
        cout << palette[get_index(chain[i])];

//file.write((char*)cursor, 1);
    }

    file.close();


    clearChain
    return 0;
}
