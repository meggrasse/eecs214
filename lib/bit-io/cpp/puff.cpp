//
//  puff.cpp
//  HW1
//
//  Created by Meg Grasse on 05/02/2026.
//

#include "classes.hpp"
#include <iostream>
#include <iostream>

using namespace std;

int main(int argc, const char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "3 arguments required, not %d\n", argc);
        return 1;
    }

    const char *infile = argv[1];
    const char *outfile = argv[2];
    
    bifstream bif(infile);
    int total;
    bif.read_bits(total, 32);
    auto huffman_tree = new Treenode(bif);
    
    bofstream bof(outfile);
    for (int i = 0; i < total; i++) {
        Treenode *node = huffman_tree;
        while (!node->is_leaf()) {
            bool b;
            bif.read(b);
            if (!b) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        bof.write_bits(node->byte, 8);
    }
    
}

