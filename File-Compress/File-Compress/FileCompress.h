#pragma once
/*
 * 文件压缩
 */
#include"HuffmanTree.h"
#include<string>
#include<fstream>

struct HashInfo//哈希桶内的数据结构
{
	HashInfo()
	{}
	HashInfo(const HashInfo& h)
	{
		this->_ch = h._ch;
		this->_code = h._code;
		this->_count = h._count;
	}
	char _ch;
	size_t _count;
	std::string _code;

	bool operator!=(const HashInfo& h)
	{
		return this->_count != h._count;
	}
	bool operator=(const HashInfo& h)
	{
		this->_ch = h._ch;
		this->_code = h._code;
		this->_count = h._count;
	}
	bool operator<(const HashInfo& h)
	{
		return this->_count < h._count;
	}
	bool operator>(const HashInfo& h)
	{
		return this->_count > h._count;
	}

	HashInfo operator+(const HashInfo& h)
	{
		HashInfo tmp;
		tmp._count = this->_count + h._count;
		tmp._code = this->_code + h._code;
		return tmp;
	}

};


class FileCompress
{
	typedef HuffmanTreeNode<HashInfo> Node;
public:
	FileCompress()//构建哈希桶
	{
		for (size_t i = 0; i < 256; ++i)//初始化哈希桶
		{
			_hashInfo[i]._ch = (char)i;
			_hashInfo[i]._count = 0;
		}
	}
	void Compress(const char* filename)//压缩
	{
		char ch;
		std::ifstream ifs(filename);
		
		//遍历文件字符，统计文件中字符出现的次数
		while (ifs.get(ch))
		{
			_hashInfo[ch]._count++;
		}
		HashInfo invalid;
		invalid._count = 0;

		HuffmanTree<HashInfo> tree(_hashInfo, 256, invalid);
	}
	void UnCompress(char* filename);//解压缩
private:
	HashInfo _hashInfo[256];
};

void TestCompress(const char* filename)
{
	FileCompress fc;
	fc.Compress(filename);
}