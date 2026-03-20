//
//  huff.cpp
//  
//
//  Created by Meg Grasse on 05/02/2026.
//

#include "classes.hpp"
#include "bit_io.hpp"
#include <iostream>

using namespace eecs214;
using namespace std;

// Populate frequency table.
int count_bytes(const char *infile, int freq_table[256])
{
    bifstream bif(infile);
    int count = 0;
    while (true) {
        int byte;
        bif.read_bits(byte, 8);
        if (bif.eof()) {
            break;
        }
        freq_table[byte]++;
        count++;
    }
    return count;
}

Treenode* huffman(Forest *forest)
{
    // We can determine size of our byte set (`n`) from the size of the forest, thus determining how many merging operations we need (less 1, as we return the root).
    int n = forest->size;
    for (int i = 0; i < n - 1; i++) {
        auto left = forest->extract_min();
        auto right = forest->extract_min();
        
        auto *internal_node = new Treenode();
        internal_node->weight = left->weight + right->weight;
        internal_node->left = left;
        internal_node->right = right;
        
        // insert subtree node back into forest.
        forest->nodes[forest->size] = internal_node;
        forest->size++;
    }
    auto root = forest->extract_min();
    return root;
}

void build_cwt(Treenode *node, Codeword codeword, Codeword cwt[256])
{
    if (node->is_leaf()) {
        cwt[node->byte] = codeword;
    }
    else {
        Codeword left = codeword.append(false);
        build_cwt(node->left, left, cwt);
        Codeword right = codeword.append(true);
        build_cwt(node->right, right, cwt);
    }
}

void serialize(const char *infile, const char *outfile, Treenode *node, int total, Codeword cwt[256])
{
    bofstream bof(outfile);
    bof.write_bits(total, 32); // per guide; size will never be greater than what can fit in 32 bits.
    node->serialize(bof);
    
    bifstream bif(infile);
    while (true) {
        int byte;
        bif.read_bits(byte, 8);
        if (bif.eof()) {
            break;
        }
        Codeword cw = cwt[byte];
        bof.write_bits(cw.value, cw.length);
    }
}

int main(int argc, const char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "3 arguments required, not %d\n", argc);
        return 1;
    }
    
    const char *infile = argv[1];
    const char *outfile = argv[2];
    
    // (1) Create frequency table
    int freq_table[256] = {};
    int total = count_bytes(infile, freq_table);

    // (2) Populate forest
    auto forest = new Forest(freq_table);
    
    // (3) Create huffman tree
    auto *huffman_tree = huffman(forest);
    
    // (4) Build codeword table
    Codeword cwt[256] = {};
    build_cwt(huffman_tree, Codeword(), cwt);
    
    // (5) Serialize
    serialize(infile, outfile, huffman_tree, total, cwt);
    
    return 0;
}

// Debugging helper
void print_freq_table(int freq_table[256])
{
    for (int i = 0; i < 256; i++) {
        if (freq_table[i] != 0) {
            cout << i << ": " << freq_table[i] << "\n";
        }
    }
}

// Debugging helper
void print_cwt(Codeword cwt[256])
{
    for (int i = 0; i < 256; i++) {
        if (cwt[i].length != 0) {
            cout << "i: " << i << " (" << cwt[i].value << ", " << cwt[i].length << ")\n";
        }
    }
}

