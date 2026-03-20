//
//  classes.hpp
//  HW1
//
//  Created by Meg Grasse on 23/02/2026.
//

#include <stdio.h>
#include "bit_io.hpp"

using namespace eecs214;

class Treenode
{
public:
    int weight;
    int byte;
    Treenode *left;
    Treenode *right;
    
    bool is_leaf();
    void serialize(bofstream& out);
    Treenode() {}
    Treenode(bifstream& in);
    
    void print_info();
};

class Forest
{
public:
    Treenode *nodes[256];
    int size;
    
    Forest(int freq_table[256]);
    Treenode* extract_min();
    
    void print_info();
};

class Codeword
{
public:
    int value;
    int length;
    
    Codeword append(bool bit);
};
