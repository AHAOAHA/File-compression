#pragma once

#include<fstream>
#include<string>
#include<assert.h>
#include"HuffmanTree.h"

typedef long long LongType;
struct CharInfo
{
	char _ch;//字符
	 LongType _count;//出现的次数
	std::string _code;//编码

	CharInfo operator+(const CharInfo& info)
	{
		CharInfo infotmp;
		infotmp._count = _count + info._count;
		return infotmp;
	}

	bool operator>(const CharInfo& info)
	{
		return this->_count > info._count;  
	}

	bool operator!=(const CharInfo& info)
	{
		return this->_count != info._count;
	}
};

class FileCompress
{
	typedef HuffmanTreeNode<CharInfo> Node;
public:
	FileCompress()
	{
		for (size_t i = 0; i < 256; ++i)
		{
			_hashInfo[i]._ch = i;
			_hashInfo[i]._count = 0;
		}
	}
	void Compress(const char* file)
	{
		//1.统计文件中的字符出现的次数
		std::ifstream ifs(file);
		char ch;
		while (ifs.get(ch))
		{
			_hashInfo[ch]._count++;
		}

		//2.生称哈夫曼树
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_hashInfo, 256, invalid);

		//3.生成哈夫曼编码
		GenerateHuffmanCode(tree.GetRoot());

		//4.进行压缩
		std::string compressfile = file;
		compressfile += ".huffman";
		std::ofstream ofs(compressfile.c_str());

		char value = 0;
		int pos = 0;
		ifs.clear();
		ifs.seekg(0);
		while (ifs.get(ch))
		{
			std::string& code = _hashInfo[ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				if (code[i] == '0')
				{
					value &= (~(1 << pos));
				}
				else if(code[i] = '1')
				{
					value |= (1 << pos);
				}
				else
				{
					assert(0);
				}
				++pos;

				if (pos == 8)
				{
					ofs.put(value);
					pos = 0;
					value = 0;
				}
			}
		}

		if (pos > 0)
		{
			ofs.put(value);
		}

	}

	void GenerateHuffmanCode(Node* root)
	{
		if (root == nullptr)
			return;

		if (root->_left == nullptr
			&& root->_right == nullptr)
		{
			std::string& code = _hashInfo[root->_w._ch]._code;
			Node* cur = root;
			Node* parent = cur->_parent;

			while (parent)
			{
				if (cur == parent->_left)
				{
					code += '0';
				}
				else
				{
					code += '1';
				}

				cur = parent;
				parent = parent->_parent;
			}

			reverse(code.begin(), code.end());
			
		}
		GenerateHuffmanCode(root->_left);
		GenerateHuffmanCode(root->_right);
	}

	//生成哈夫曼树的另一种方法
	/*void GenerateHuffmanCode(Node* root)
	{
		if (root == nullptr)
			return;

		if(root->_left == nullptr && root->_right == nullptr)
		{
			_hashInfo[root->_w._ch]._code = root->_w._code;
		}
		if(root->_left)
		{
			root->_left->_w._code = root->_w._code + '0';
			GenerateHuffmanCode(root->_left);
		}

		if(root->_right)
		{
			root->_right->_w._code = root->_w._code + '1';
			GenerateHuffmanCode(root->_right);
		}
	}*/
	
	void Uncompress(const char* file);
private:
	CharInfo _hashInfo[256];
};


void TestCompress()
{
	FileCompress fc;
	fc.Compress("input.txt");
}