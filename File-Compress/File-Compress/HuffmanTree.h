#pragma once
/*
 * 构建Huffman树
 */

template<class W>
struct HuffmanTreeNode//哈夫曼树的节点
{
	char _ch;//代表该节点存储字符
	size_t _count;//字符出现的次数
	W _w;//权重
};

template<class W>
class HuffmanTree
{
public:
	HuffmanTree(W* _array, size_t size)
	{

	}

private:
	W* _root;//代表HuffmanTree的根节点
};
