#pragma once
/*
 * ����Huffman��
 */

template<class W>
struct HuffmanTreeNode//���������Ľڵ�
{
	char _ch;//����ýڵ�洢�ַ�
	size_t _count;//�ַ����ֵĴ���
	W _w;//Ȩ��
};

template<class W>
class HuffmanTree
{
public:
	HuffmanTree(W* _array, size_t size)
	{

	}

private:
	W* _root;//����HuffmanTree�ĸ��ڵ�
};
