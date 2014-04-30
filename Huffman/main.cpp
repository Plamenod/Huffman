#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "BitSet.h"
#include <fstream>
#include <Windows.h>
using namespace std;

void Sorting(Node** array , size_t size_)
{
	bool cnt = true;
	while(cnt)
	{
		cnt = false;
		for(int c = 0; c < size_ -1; ++c)
		{
			if(array[c]->weight > array[c+1]->weight)
			{
				swap(array[c], array[c+1]);
				cnt = true;
			}
		}
		--size_;
	}
	return;
}
void CreateTree(Node ** container, size_t size_)
{
	while(size_ != 1)
	{
		Node* temp = new Node;
		temp->weight = container[0]->weight + container[1]->weight;
		temp->left = container[0];
		temp->right = container[1];
		container[0] = temp;
		swap(container[1],container[size_-1]);
		--size_;
		Sorting(container, size_);
	}
}

Node** MakeLeafs(char* expresion ,int& size_ )
{
	size_t ArrayCount[256];
	int SizeExpresion = 0;
	memset(ArrayCount, 0, 256 * sizeof(size_t));
	for( ; *expresion; ++expresion)
	{
		++SizeExpresion;
		ArrayCount[*expresion]+= 1;
	}
	size_ = 0;
	for(int i =0; i<256; ++i)
	{
		if(ArrayCount[i] != 0)
		{
			++size_;
		}
	}

	Node** StoreLieaf = new Node*[size_];
	for(int c =0, pos= 0; c<256; ++c)
	{
		if(ArrayCount[c]!=0)
		{
			Node* temp = new Node((char)c,ArrayCount[c]);
			StoreLieaf[pos] = temp;
			++pos;
		}
	}

	Sorting(StoreLieaf,size_);
	return StoreLieaf;
}

struct NodeFile{

	size_t weight;
	char symbol;
};

NodeFile* FillNodeInfo(int size_ , Node** StoreLeaf)
{
	NodeFile* temp = new NodeFile[size_];
	for(int i = 0 ; i<size_; ++i)
	{
		temp[i].symbol = StoreLeaf[i]->symbol;
		temp[i].weight = StoreLeaf[i]->weight;
	}
	return temp;
}

bool search(const Node* current , const char symbol) 
{
	if(!current)
		return false;
	if(current->left == NULL)
	{
		if(current->symbol == symbol)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(search(current->left , symbol))
			return true;
		search(current->right, symbol);
	}
}

void SingleSet(Node*currentNode, BitSet& key, int& cntLen, char* ch)
{
	if(search(currentNode->right,*ch))
	{
		if(currentNode->right == NULL)
		{
			return;
		}
		key.set(cntLen++);
		SingleSet(currentNode->right, key, cntLen, ch);
	}
	else
	{
		if(currentNode->left == NULL)
		{
			return;
		}
		SingleSet(currentNode->left, key, ++cntLen, ch);
	}
}


void MakeAKey(char* expresion , Node** tree,BitSet& BitF)
{
	for(; *expresion; ++expresion)
	{
		BitSet BitS;
		int cntPos = 0;
		SingleSet(*tree, BitS,cntPos, expresion);
		BitS.SetBitLength(cntPos);
		BitF += BitS;
		unsigned char * k= BitF.getData();
	}
}

void MakeAKeyF(FILE* pf, Node** tree,BitSet& BitF)
{
	char buffer[1024] = { 0, };
	size_t readSize = 0;
	int checkSymbol[256];
	memset(checkSymbol,7,sizeof(int)*256);
	size_t checkLength[256] = {0};
	BitSet * map[256] = { NULL, };
	while (readSize = fread(buffer, sizeof(char), 1024, pf))
	{
		for (size_t cnt = 0; cnt < readSize; ++cnt)
		{
			if(map[buffer[cnt]])
			{
				BitF += *map[buffer[cnt]];
			} 
			else
			{
				int cntPos = 0;
				map[buffer[cnt]] = new BitSet;
				SingleSet(*tree, *map[buffer[cnt]], cntPos, &buffer[cnt]);
				map[buffer[cnt]]->SetBitLength((size_t)cntPos);
				BitF += *map[buffer[cnt]];
			}
		}
	}
	for(size_t c = 0; c < 256; ++c)
		delete map[c];
}

void MakeFile(NodeFile* leafs, int numberOfNodes, BitSet& BS, char* FileName)
{
	FILE* pf = fopen(FileName,"wb");
	int s = (int)BS.getLengthPath();
	fwrite(&numberOfNodes,1,sizeof(int),pf);            //  kolko sa wyzlite
	fwrite(leafs,1,sizeof(NodeFile)*numberOfNodes,pf); //   wsiki wyzli
	fwrite(&s,1,sizeof(size_t),pf);    //  goleminata na koda 
	int sizeByte;
	if(!BS.getLengthPath()%8)
	{
		sizeByte = BS.getLengthPath()/8;
	}
	else
	{
		sizeByte = BS.getLengthPath()/8 +1;
	}
	fwrite(BS.getData(),1,sizeof(unsigned char)*sizeByte,pf);
	fclose(pf);
}

Node** RecoverLeafs(NodeFile* nodeFile, int size)
{
	Node** temp = new Node*[size];
	for(int i=0; i<size; ++i)
	{
		Node* currentNode = new Node(nodeFile[i].symbol,nodeFile[i].weight);
		temp[i] = currentNode;
	}
	return temp;
}
Node* getChidren(Node* current, int way)
{
	if(way)
	{
		return current->right;
	}
	else
	{
		return current->left;
	}
}
bool checkBite(unsigned char* code , int pos)
{
	if(code[pos/8] & (1<<(7-pos%8)))  
		return true;
	else
		return false;
}

void HelpDeCode(Node** tree, unsigned char* code, int lengthOfCode)
{
	FILE * pf = fopen("NewText.txt", "w");
	Node* temp = tree[0];
	int pos= 0;
	while(pos < lengthOfCode)
	{
		if(temp->left!=NULL )
		{
			bool switch_ = checkBite(code,pos++);
			if(switch_)
				temp = getChidren(temp,1);
			else
				temp = getChidren(temp,0);
		}
		if(temp->left==NULL )
		{
			fwrite(&temp->symbol,sizeof(char),1,pf);
			temp = tree[0];
		}
	}
}


void decode(char* SourceFile)
{

	FILE* pf = fopen(SourceFile, "rb");
	int numberOfNodes;
	int lengthOfCode;
	if(!pf)
		return;
	fread(&numberOfNodes,sizeof(int),1,pf);
	NodeFile* nf = new NodeFile[numberOfNodes];
	fread(nf,sizeof(NodeFile),numberOfNodes,pf);
	fread(&lengthOfCode,sizeof(int),1,pf);
	int memory = lengthOfCode;
	if(memory%8)
	{
		memory = memory/8 +1;
	}
	else
	{
		memory = memory/8;
	}
	unsigned char* Code = new unsigned char[memory];
	fread(Code,sizeof(unsigned char),memory,pf);
	Node** Ntree = RecoverLeafs(nf,numberOfNodes);
	CreateTree(Ntree,numberOfNodes);
	HelpDeCode(Ntree,Code,lengthOfCode);
}

FILE* GetStream(char* fileName)
{
	FILE* pf = fopen(fileName , "r");
	if(!pf)
	{
		cout<<"Can't open it!!!"<<endl;
	}
	return pf;
}
void refresh(FILE* pf)
{
	rewind(pf);
}

void countChars(FILE* input, int* chars)
{
	char buffer[1024] = { 0 };
	size_t readSize = 0;
	while (readSize = fread(buffer, sizeof(char), 1024, input))
	{
		for (size_t cnt = 0; cnt < readSize; ++cnt)
		{
			++chars[buffer[cnt]];
		}
	}
}

Node** MakeLeafsFromFile(FILE* pf, int& numberOfLeafs)
{
	
	int countSymbol[256] = {0,};
	refresh(pf);
	countChars(pf,countSymbol);
	numberOfLeafs = 0;
	for(int i=0; i<256; ++i)
	{
		if(countSymbol[i]!=0)
		{
			++numberOfLeafs;
		}
	}
	Node** StoreLieaf = new Node*[numberOfLeafs];
	for(int c =0, pos= 0; c<256; ++c)
	{
		if(countSymbol[c]!=0)
		{
			Node* temp = new Node((char)c,countSymbol[c]);
			StoreLieaf[pos] = temp;
			++pos;
		}
	}
	Sorting(StoreLieaf,numberOfLeafs);
	refresh(pf);
	return StoreLieaf;
}

int main(int argc, char * argv[])
{
	if(argc!=2) return 1;
	size_t start = GetTickCount();
	char* file = argv[1];
	FILE* pf = GetStream(file);
	int numberOfNode;
	long long int sizeOfFile;
	streampos size;
	Node** StoreLeaf = MakeLeafsFromFile(pf,numberOfNode);
	NodeFile* NodeInfo = FillNodeInfo(numberOfNode, StoreLeaf);   
	CreateTree(StoreLeaf,numberOfNode);
	BitSet Buffer;   
	MakeAKeyF(pf,StoreLeaf,Buffer);
	char* nameF = "compresed";
	MakeFile(NodeInfo,numberOfNode,Buffer,nameF);
	decode(nameF);
	cout << GetTickCount() - start;
	return 0;
}


