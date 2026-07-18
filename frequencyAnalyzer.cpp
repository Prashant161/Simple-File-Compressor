//directly try to create frequency table
#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
using namespace std;
struct Frequency{
    char ch;
    int count;
};
int main(){
    int frequency[256] = {0}, count = 0;
    string message;
    vector<Frequency> f;
    struct Frequency temp;
    cout << "Enter the input: " << endl;
    getline(cin, message);
    for (char ch: message){
        frequency[ch]++;
    }
    for( int i = 0; i < 256; i++){
        if(frequency[i]!=0){
            f.push_back({(char)i, frequency[i]});
        }
    }
    for( int i = 0; i < f.size(); i++){
        for( int j = i+1; j < f.size(); j++){
            if (f[i].count<f[j].count){
                temp = f[i];
                f[i] = f[j];
                f[j] = temp;
            }
        }
    }
    for ( int i = 0; i < f.size(); i++){
        cout << f[i].ch << " : " << f[i].count << endl;
    }
    return 0;
}