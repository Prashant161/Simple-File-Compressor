
# Simple File Compressor

A simple file compression utility written in C++ using the **Huffman Coding** algorithm.

## Project Structure

- **huffmanEncoding.cpp** — Main program for compressing and decompressing files using Huffman coding.
- **sampleFile.txt** — Sample input file used for testing the compressor.
- **learnings/** — Contains intermediate programs and experiments developed while learning and building the compression system.

## User Manual

### Compile

g++ huffmanEncoding.cpp -o huffmanEncoding
### Logic
```
(to compress use 'c')
.\exe -c  INPUT_FILE.txt  OUTPUT_FILE.huff 

(to decompress use 'd')
.\exe -d  INPUT_FILE.huff  OUTPUT_FILE.txt 
```
### Compress a File

```bash
.\huffmanEncoding.exe -c <input_file> <output_file.huff>
```

Example:

```bash
.\huffmanEncoding.exe -c sampleFile.txt sample.huff
```

### Decompress a File

```bash
.\huffmanEncoding.exe -d <input_file.huff> <output_file>
```

Example:

```bash
.\huffmanEncoding.exe -d sample.huff restored.txt
```
