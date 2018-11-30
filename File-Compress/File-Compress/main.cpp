#include"HuffmanTree.h"
#include"FileCompress.h"

int main()
{
	//test();
	TestCompress("input.txt");
	TestUnCompress("input.txt.huffman");
	return 0;
}