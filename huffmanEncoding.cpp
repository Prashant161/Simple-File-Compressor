#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cstdint>
#include <stdexcept>

using namespace std;

struct Frequency {
    unsigned char ch = 0;
    uint64_t count = 0;
    Frequency* left = nullptr;
    Frequency* right = nullptr;
};

struct Compare {
    bool operator()(const Frequency* a, const Frequency* b) const {
        return a->count > b->count;
    }
};

void deleteTree(Frequency* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void generateCodes(Frequency* node, const string& code,
                   vector<string>& codes) {
    if (!node) return;

    // Leaf node
    if (!node->left && !node->right) {
        // Special case: a file containing only one unique byte.
        codes[node->ch] = code.empty() ? "0" : code;
        return;
    }

    generateCodes(node->left, code + "0", codes);
    generateCodes(node->right, code + "1", codes);
}

Frequency* buildTree(const uint64_t freq[256]) {
    priority_queue<Frequency*, vector<Frequency*>, Compare> pq;

    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            Frequency* node = new Frequency;
            node->ch = static_cast<unsigned char>(i);
            node->count = freq[i];
            pq.push(node);
        }
    }

    if (pq.empty()) return nullptr;

    while (pq.size() > 1) {
        Frequency* left = pq.top();
        pq.pop();

        Frequency* right = pq.top();
        pq.pop();

        Frequency* parent = new Frequency;
        parent->count = left->count + right->count;
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

bool compressFile(const string& inputName, const string& outputName) {
    ifstream input(inputName, ios::binary);
    if (!input) {
        cerr << "Error: cannot open input file.\n";
        return false;
    }

    // Count byte frequencies and original size.
    uint64_t freq[256] = {};
    uint64_t originalSize = 0;

    char c;
    while (input.get(c)) {
        unsigned char byte = static_cast<unsigned char>(c);
        ++freq[byte];
        ++originalSize;
    }

    Frequency* root = buildTree(freq);

    vector<string> codes(256);
    generateCodes(root, "", codes);

    // Rewind the input so we can encode it.
    input.clear();
    input.seekg(0);

    ofstream output(outputName, ios::binary);
    if (!output) {
        cerr << "Error: cannot create output file.\n";
        deleteTree(root);
        return false;
    }

    /*
        File format:

        4 bytes  : "HUF1"
        8 bytes  : original file size
        256 x 8 bytes : frequency table
        remaining bytes : Huffman encoded data

        We store the frequency table instead of the tree.
        The decoder can rebuild the exact same Huffman tree.
    */

    const char magic[4] = {'H', 'U', 'F', '1'};
    output.write(magic, 4);
    output.write(reinterpret_cast<const char*>(&originalSize),
                 sizeof(originalSize));

    for (int i = 0; i < 256; ++i) {
        output.write(reinterpret_cast<const char*>(&freq[i]), sizeof(freq[i]));
    }

    // Pack Huffman bits into bytes.
    unsigned char currentByte = 0;
    int bitCount = 0;

    while (input.get(c)) {
        unsigned char byte = static_cast<unsigned char>(c);
        const string& code = codes[byte];

        for (char bit : code) {
            currentByte = static_cast<unsigned char>(currentByte << 1);

            if (bit == '1')
                currentByte = static_cast<unsigned char>(currentByte | 1);

            ++bitCount;

            if (bitCount == 8) {
                output.put(static_cast<char>(currentByte));
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    // Pad the last byte with zeroes.
    if (bitCount != 0) {
        currentByte = static_cast<unsigned char>(currentByte << (8 - bitCount));
        output.put(static_cast<char>(currentByte));
    }

    output.close();
    input.close();

    cout << "Compression complete.\n";
    cout << "Original size : " << originalSize << " bytes\n";

    ifstream compressed(outputName, ios::binary | ios::ate);
    if (compressed) {
        uint64_t compressedSize =
            static_cast<uint64_t>(compressed.tellg());
        cout << "Compressed size: " << compressedSize << " bytes\n";
    }

    deleteTree(root);
    return true;
}

bool decompressFile(const string& inputName, const string& outputName) {
    ifstream input(inputName, ios::binary);
    if (!input) {
        cerr << "Error: cannot open compressed file.\n";
        return false;
    }

    char magic[4];
    input.read(magic, 4);

    if (input.gcount() != 4 ||
        magic[0] != 'H' || magic[1] != 'U' ||
        magic[2] != 'F' || magic[3] != '1') {
        cerr << "Error: not a valid HUF1 file.\n";
        return false;
    }

    uint64_t originalSize = 0;
    input.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));

    if (!input) {
        cerr << "Error: corrupted header.\n";
        return false;
    }

    uint64_t freq[256] = {};

    for (int i = 0; i < 256; ++i) {
        input.read(reinterpret_cast<char*>(&freq[i]), sizeof(freq[i]));
        if (!input) {
            cerr << "Error: corrupted frequency table.\n";
            return false;
        }
    }

    Frequency* root = buildTree(freq);

    ofstream output(outputName, ios::binary);
    if (!output) {
        cerr << "Error: cannot create output file.\n";
        deleteTree(root);
        return false;
    }

    if (originalSize == 0) {
        output.close();
        deleteTree(root);
        cout << "Decompression complete. Empty file restored.\n";
        return true;
    }

    if (!root) {
        cerr << "Error: invalid Huffman tree.\n";
        output.close();
        deleteTree(root);
        return false;
    }

    // A file with one unique byte is a special case.
    if (!root->left && !root->right) {
        for (uint64_t i = 0; i < originalSize; ++i)
            output.put(static_cast<char>(root->ch));

        output.close();
        deleteTree(root);
        cout << "Decompression complete.\n";
        return true;
    }

    Frequency* current = root;
    uint64_t produced = 0;
    char byteChar;

    while (produced < originalSize && input.get(byteChar)) {
        unsigned char byte = static_cast<unsigned char>(byteChar);

        for (int bit = 7; bit >= 0 && produced < originalSize; --bit) {
            bool isOne = (byte >> bit) & 1;

            current = isOne ? current->right : current->left;

            if (!current) {
                cerr << "Error: corrupted compressed data.\n";
                output.close();
                deleteTree(root);
                return false;
            }

            if (!current->left && !current->right) {
                output.put(static_cast<char>(current->ch));
                ++produced;
                current = root;
            }
        }
    }

    output.close();
    deleteTree(root);

    if (produced != originalSize) {
        cerr << "Error: compressed data ended before the original file was restored.\n";
        return false;
    }

    cout << "Decompression complete.\n";
    cout << "Restored size: " << produced << " bytes\n";
    return true;
}

void printUsage(const string& program) {
    cout << "Huffman File Compressor\n\n";
    cout << "Usage:\n";
    cout << "  " << program << " -c <input> <output.huff>\n";
    cout << "  " << program << " -d <input.huff> <output>\n\n";
    cout << "Examples:\n";
    cout << "  " << program << " -c sample.txt sample.huff\n";
    cout << "  " << program << " -d sample.huff restored.txt\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printUsage(argv[0]);
        return 1;
    }

    string mode = argv[1];
    string input = argv[2];
    string output = argv[3];

    try {
        if (mode == "-c") {
            return compressFile(input, output) ? 0 : 1;
        }

        if (mode == "-d") {
            return decompressFile(input, output) ? 0 : 1;
        }

        printUsage(argv[0]);
        return 1;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
