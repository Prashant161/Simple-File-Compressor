//directly try to create frequency table
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;
struct Frequency{
    char ch;
    int count;
};
int main(){
    int frequency[256] = {0};
    string message;
    vector<Frequency> f;

    cout << "Enter the input: " << endl;
    getline(cin, message);

    for (char ch: message){
        frequency[static_cast<unsigned char>(ch)]++;
    }

    for( int i = 0; i < 256; i++){

        if(frequency[i]!=0){
            f.push_back({(char)i, frequency[i]});
        }
    }

    sort(f.begin(), f.end(), [](const Frequency& a, const Frequency& b){
        if(a.count!=b.count){
            return a.count > b.count;
        }
            return a.ch < b.ch;
    });

    for( const auto& item: f){
        cout << item.ch << " : " << item.count << endl;
    }

    return 0;
}