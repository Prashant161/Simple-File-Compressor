#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

struct Frequency {
    char ch;
    int count;
    Frequency* left = nullptr;
    Frequency* right = nullptr;
};
void generateCodes(Frequency* node, string code)
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            cout << node->ch << " : " << code << endl;
            return;
        }

        generateCodes(node->left, code + "0");
        generateCodes(node->right, code + "1");
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
        generateCodes(root, "");
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
    return 0;
}