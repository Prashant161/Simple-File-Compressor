//directly try to create frequency table
#include<iostream>
#include<string.h>
#include<vector>
using namespace std;
struct Frequency{
    char ch;
    int count;
};
int main(){
    int frequency[256] = {0}, count = 0;
    string message;
    cout << "Enter the input: " << endl;
    getline(cin, message);
    for (char ch: message){
        frequency[ch]++;
    }
    for( int i = 0; i < 256; i++){
        if(frequency[i]!=0){
            count++; 
            //cout << (char)i << " : " << frequency[i] << endl;
        }
    }
    struct Frequency f[count], temp;
    int j = 0;
    for( int i = 0; i < 256; i++){
        if(frequency[i]!=0){
            f[j].ch = (char)i;
            f[j].count = frequency[i];
            j++;
        }
    }
    for( int i = 0; i < count; i++){
        for( int j = i+1; j < count; j++){
            if (f[i].count<f[j].count){
                temp = f[i];
                f[i] = f[j];
                f[j] = temp;
            }
        }
    }
    for ( int i = 0; i < count; i++){
        cout << f[i].ch << " : " << f[i].count << endl;
    }
    return 0;
}