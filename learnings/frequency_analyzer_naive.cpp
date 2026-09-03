#include<iostream>
#include<string>
using namespace std;

int main(){
    string message;
    int messageLength;

    std::cout << "Enter the message: " << endl;
    getline( cin, message);
    messageLength = message.length();
//using new[] & delete[] operation for dynamic array
    int* frequency = new int[messageLength]();
    bool* visited = new bool[messageLength]();

    for(int i = 0; i < messageLength ; i++){
        if(visited[i] == false){
            for ( int j = i+1; j < messageLength; j++){
            
                if(message[i] == message[j]){
                     visited[j] = true;
                     frequency[i]++;  
                }
            }
         std::cout << message [i] << "  appeared " << frequency[i] + 1 << " times. " << endl;
        }
    }
   delete[] frequency;
   delete[] visited;
return 0;
}