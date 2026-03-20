//
//  treenode.cpp
//  HW1
//
//  Created by Meg Grasse on 23/02/2026.
//

#include "classes.hpp"
#include <iostream>

using namespace std;

bool Treenode::is_leaf()
{
    if (right == NULL && left == NULL) {
        return true;
    }
    else if ((right != NULL || left != NULL) && byte == 0) {
        return false;
    }
    else {
        throw;
    }
}

void Treenode::serialize(bofstream& out)
{
    if (this->is_leaf()) {
        out << false;
        out.write_bits(byte, 8);
    }
    else {
        out << true;
        left->serialize(out);
        right->serialize(out);
    }
}

Treenode::Treenode(bifstream& in)
{
    bool is_branch;
    in >> is_branch;
    if (!is_branch) {
        in.read_bits(byte, 8);
    }
    else {
        left = new Treenode(in);
        right = new Treenode(in);
    }
}

Forest::Forest(int freq_table[256])
{
    int forest_size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq_table[i] != 0) {
            auto node = new Treenode();
            node->weight = freq_table[i];
            node->byte = i;
            nodes[forest_size] = node;
            forest_size++;
        }
    }
    size = forest_size;
}

// Representing the forest via a priority-queue (i.e., a heap) rather than an
// array would be more efficient, but chose an array for simplicity in this excercise.
Treenode* Forest::extract_min()
{
    int min_index = 0;
    int min_weight = INT_MAX;
    for (int i = 0; i < size; i++) {
        if (nodes[i] != NULL && nodes[i]->weight < min_weight) {
            min_index = i;
            min_weight = nodes[i]->weight;
        }
    }
    auto min_node = nodes[min_index];
    nodes[min_index] = nodes[size - 1];
    size--;
    return min_node;
}

Codeword Codeword::append(bool bit)
{
    int value = 2 * this->value;
    if (bit) {
        value++;
    }
    int length = this->length + 1;
    
    auto cw = Codeword();
    cw.value = value;
    cw.length = length;
    return cw;
}

// debugging helper
void Treenode::print_info()
{
    if (this->is_leaf()) {
        cout << "Leaf[w: " << weight << ", bv: " << byte << "]";
    }
    else {
        cout << "Branch[w: " << weight << ", l: ";
        left->print_info();
        cout << ", r: ";
        right->print_info();
        cout << "]";
    }
}

// Debugging helper
void Forest::print_info()
{
    for (int i = 0; i < 256; i++) {
        if (nodes[i] != NULL) {
            nodes[i]->print_info();
        }
    }
    cout << "forestSize: " << size << "\n";
}
