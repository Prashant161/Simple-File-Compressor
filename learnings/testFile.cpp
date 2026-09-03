#include<iostream>
using namespace std;
int main(){

string encoded = "01101011100";

unsigned char byte = 0;

for (int i = 0; i < 8; i++)
{
    byte = byte << 1;

    if (encoded[i] == '1')
    {
        byte = byte | 1;
    }
}

cout << (int)byte << endl;
}

