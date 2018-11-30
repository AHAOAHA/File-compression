#pragma once
/*
 * 文件压缩
 */
#include"HuffmanTree.h"
#include<string>
#include<fstream>
#include<iostream>

struct HashInfo//哈希桶内的数据结构
{
	HashInfo()
		:_ch('\0')
		, _count(0)
		,_code("")
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
		std::cout << "FileCompress()" << std::endl;
	}
	~FileCompress()
	{
		std::cout << "~FileCompress()" << std::endl;
	}
	void Compress(std::string filename)//压缩
	{
		char ch;
		std::ifstream ifs(filename.c_str());
		

		//遍历文件字符，统计文件中字符出现的次数
		while (ifs.get(ch))
		{
			_hashInfo[(unsigned char)ch]._count++;
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

		std::ofstream ofs(newfilename.c_str());
		
		//开始压缩之前，需要现在新文件头部写入各字符出现的次数
		//用于在解压文件开始之前，重新构建huffman树
		for (size_t i = 0; i < 256; ++i)
		{
			if (_hashInfo[i]._count > 0)
			{
				ofs.write((char*)&_hashInfo[i], sizeof(HashInfo));
			}
		}
		HashInfo end;
		end._count = 0;
		ofs.write((char*)&end, sizeof(HashInfo));

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
			//源文件中的字符无法完美贴合，直接将此时的值直接存入
			ofs.put(newch);
		}
	}

	void MakeCode(Node* root)
	{
		if (root->_left == nullptr && root->_right == nullptr)
		{
			_hashInfo[(unsigned char)root->_w._ch]._code = root->_w._code;
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



	void UnCompress(std::string filename)//解压缩
	{
		//打开压缩文件进行解压缩
		std::ifstream ifs(filename.c_str());
	
		//生成源文件名
		std::string newfilename = filename.erase(filename.rfind('.'));
		newfilename += ".unhuffman";

		//打开新生成的源文件名
		std::ofstream ofs(newfilename.c_str());
		
		//重建huffman树，在压缩文件的头部保存的出现字符的信息，以_count为0的一个节点信息为结束标志		
		HashInfo Info;
		while (true)
		{

			ifs.read((char*)&Info, sizeof(Info));

			if (Info._count > 0)
			{
				_hashInfo[(unsigned char)Info._ch]._count = Info._count;
				//_hashInfo[(unsigned char)CharInfo._ch]._code = CharInfo._code;
				//添加ch的_code
			}
			else
			{
				break;
			}

		}



		//根据_hashInfo重建huffman树,此处不能使用hashInfo桶的数据重新构建huffman树，因为当两个字符的权值相同时，不能保证两个字符的顺序一定相同
		//所以此处创建新的huffman树时应该使用三叉链方法，也就是说让每一个节点增加一个指针并指向他的parent
		HashInfo invalid;
		invalid._count = 0;

		HuffmanTree<HashInfo> tree(_hashInfo, 256, invalid);


		//根据重建的huffman树恢复文件

		int pos = 0;
		char ch = '\0';
		Node* root = tree.getroot();
		int count = root->_w._count;
		Node* ptr = root;
		while (ifs.get(ch))
		{


			for (size_t pos = 0; pos < 8; ++pos)
			{
				if (count == 0)
					break;

				if (((1 << pos)&ch) != 0)
				{
					ptr = ptr->_right;
				}
				else if ((1 << pos&ch) == 0)
				{
					ptr = ptr->_left;
				}
				
				if (ptr->_left == nullptr && ptr->_right == nullptr)
				{
					ofs.put(ptr->_w._ch);
					ptr = root;
					count--;
				}
			}
		}

	}

private:
	HashInfo _hashInfo[256];
};

void TestCompress(std::string filename)
{
	FileCompress fc;
	fc.Compress(filename);
}

void TestUnCompress(std::string filename)
{
	FileCompress fc;
	fc.UnCompress(filename);
}

void test()
{
	HuffmanTree<HashInfo> tree;
}