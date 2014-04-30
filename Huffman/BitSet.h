#pragma once
#include <iostream>

using namespace std;


struct Node{

	Node* left;
	Node* right;
	size_t weight;
	char symbol;

	Node(char s, size_t w);
	Node(){this->left = NULL; this->right = NULL;};
	~Node();

};

class BitSet{

public:
	BitSet(size_t size = 256);
	~BitSet();
    void set(size_t idx);
    BitSet & operator+=(const BitSet &);
	void CountLength();
	void SetBitLength(size_t length);
	unsigned char * getData();
	size_t getLengthPath();
	void resize();
	void setData(int data);
	void setlength(size_t w);

private:

	unsigned char* data;
	size_t SizeBit;
	size_t LengthPath;
};

