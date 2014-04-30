#include "BitSet.h"
#include <cmath>


BitSet::BitSet(size_t size)
{
	this->SizeBit = size;
	this->data = new unsigned char[size/8+1];
	memset(this->data, 0 ,size/8+1);
	this->LengthPath = 0;
}

BitSet::~BitSet()
{
	delete [] this->data;
}


Node::Node(char s, size_t w)
{	
	this->left = NULL;
	this->right = NULL;
	this->symbol = s;
	this->weight = w;
}

void BitSet::set(size_t index)
{
	if(index > this->SizeBit)
	{
		return;
	}
	size_t posByte = index/8;
	this->data[posByte]|= 1<<(8 - (index % 8) - 1);
	return;
}

BitSet& BitSet::operator+= (const BitSet& right)
{
	int newLength = this->LengthPath + right.LengthPath;
	if(newLength > this->SizeBit)
	{
		this->resize();
	}
	int pos = this->LengthPath;
	int pos2 = 0;
	while (pos2 < right.LengthPath)
	{
		
			if(pos>= newLength)
				break;
			if(right.data[pos2/8] & 1<<(7-pos2%8))
			{
				this->set(pos++);
				++pos2;
			}
			else
			{
				++pos;
				++pos2;
				continue;
			}
		
	}
	this->LengthPath = pos;
	return *this;
}
void BitSet::CountLength()
{
	this->LengthPath++;
}

void BitSet::SetBitLength(size_t length)
{
	this->LengthPath = length;
}

unsigned char * BitSet:: getData()
{
	return this->data;
}

size_t BitSet::getLengthPath()
{
	return this->LengthPath;
}

void BitSet::resize()
{
	this->SizeBit = (this->SizeBit*2);
	unsigned char* Temp = new unsigned char[this->SizeBit ];
	memset(Temp, 0 ,this->SizeBit);
	int Integer_ =(this->LengthPath/8)+1;
	for(int i=0; i<Integer_; ++i)
	{
		Temp[i] = this->data[i];
	}
	delete [] this->data;
	this->data = Temp;
}

void BitSet:: setData(int  data)
{
	*(this->data) = (unsigned char)data;

}
void BitSet::setlength(size_t w)
{
	this->LengthPath;
}

