#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
using namespace std;

struct Frequency {
    char ch;
    int count;
    Frequency* left = nullptr;
    Frequency* right = nullptr;
};
void generateCodes(Frequency* node, string code, map<char, string>& codes)
{
    if (node->left == nullptr && node->right == nullptr)
    {
        codes[node->ch] = code;
        return;
    }

    generateCodes(node->left, code + "0", codes);
    generateCodes(node->right, code + "1", codes);
}
int main() {

    int frequency[256] = {0};
    string message;

    vector<Frequency*> nodes;

    cout << "Enter the input: " << endl;
    getline(cin, message);

    // Count frequencies
    for (char ch : message) {
        frequency[static_cast<unsigned char>(ch)]++;
    }

    // Create a node for every character
    for (int i = 0; i < 256; i++) {

        if (frequency[i] != 0) {

            Frequency* node = new Frequency;

            node->ch = (char)i;
            node->count = frequency[i];

            nodes.push_back(node);
        }
    }

    // Sort by frequency
    sort(nodes.begin(), nodes.end(),
        [](const Frequency* a, const Frequency* b) {

            if (a->count != b->count) {
                return a->count < b->count;
            }

            return a->ch < b->ch;
        });
    while (nodes.size() > 1) {

        // Take the two smallest nodes
        Frequency* left = nodes[0];
        Frequency* right = nodes[1];

        // Create their parent
        Frequency* parent = new Frequency;

        parent->ch = '\0';
        parent->count = left->count + right->count;

        parent->left = left;
        parent->right = right;

        // Remove the two smallest nodes
        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());

        // Add the new parent
        nodes.push_back(parent);

        // Sort again
        sort(nodes.begin(), nodes.end(),
            [](const Frequency* a, const Frequency* b) {

                if (a->count != b->count) {
                    return a->count < b->count;
                }

                return a->ch < b->ch;
            });
        }
        Frequency* root = nodes[0];
        map<char, string> codes;

        generateCodes(root, "", codes);
        for (const auto& item : codes)
{
    cout << item.first << " : " << item.second << endl;
}
string encoded;

for (char ch : message)
{
    encoded += codes[ch];
}
        int padding = 8 - (encoded.length() % 8);

if (padding == 8)
{
    padding = 0;
}

encoded.append(padding, '0');

vector<unsigned char> bytes;

for (int i = 0; i < encoded.length(); i += 8)
{
    unsigned char byte = 0;

    for (int j = 0; j < 8; j++)
    {
        byte = byte << 1;

        if (encoded[i + j] == '1')
        {
            byte = byte | 1;
        }
    }

    bytes.push_back(byte);
}
// //     test print code 
//     for (const auto& item : nodes) {
//         cout << item->ch << " : " << item->count << endl;
//     }
//     cout << "Root: " << root->ch << " : " << root->count << endl;

//     cout << "Left: " << root->left->ch
//      << " : " << root->left->count << endl;

//     cout << "Right: " << root->right->ch
//      << " : " << root->right->count << endl;

//encoding

//temporarily printing the map


//cout << "Encoded: " << encoded << endl;
    return 0;
}