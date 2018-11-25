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
	void operator=(const HashInfo& h)
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

		// 构建字符编码并将制造好的字符编码存进_hashInfo
		MakeCode(tree.getroot());

		//生成新文件的文件名，并以写入方式打开/创建新文件
		std::string newfilename(filename);
		newfilename += ".huffman";

		//遍历源文件中的字符，根据_hashInfo中各个字符的编码生成新文件

		std::ofstream ofs(newfilename);
		//首先将文件的读取的位置重置，让ifs重新指向文件的开始
		ifs.clear();
		ifs.seekg(0);
		char newch = '\0';
		int pos = 0;
		std::string tmpcode;
		while (ifs.get(ch))
		{
			tmpcode = _hashInfo[ch]._code;
			
			for (size_t i = 0; i < tmpcode.size(); ++i)
			{
				if (tmpcode[i] == '0')
				{
					//1&0=0 1&1=1
					newch = newch & (~(1 << pos));
				}
				else
				{
					//0|1=1 0|0=0
					newch = newch | (1 << pos);
				}
				++pos;

				if (pos == 8)
				{
					ofs.put(newch);
					pos = 0;
					newch = '\0';
					tmpcode.clear();
				}
			}
		}

		if (pos != 0)
		{
			//源文件中的字符无法完美贴合
		}

	}

	void MakeCode(Node*& root)
	{
		if (root->_left == nullptr && root->_right == nullptr)
		{
			_hashInfo[root->_w._ch]._code = root->_w._code;
			return;
		}

		if (root->_left)
		{
			root->_left->_w._code += root->_w._code + '0';
		}
		if (root->_right)
		{
			root->_right->_w._code = root->_w._code + '1';
		}

		MakeCode(root->_left);
		MakeCode(root->_right);
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