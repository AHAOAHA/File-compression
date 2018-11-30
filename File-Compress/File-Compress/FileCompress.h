#pragma once
/*
 * �ļ�ѹ��
 */
#include"HuffmanTree.h"
#include<string>
#include<fstream>
#include<iostream>

struct HashInfo//��ϣͰ�ڵ����ݽṹ
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
	FileCompress()//������ϣͰ
	{
		for (size_t i = 0; i < 256; ++i)//��ʼ����ϣͰ
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
	void Compress(std::string filename)//ѹ��
	{
		char ch;
		std::ifstream ifs(filename.c_str());
		

		//�����ļ��ַ���ͳ���ļ����ַ����ֵĴ���
		while (ifs.get(ch))
		{
			_hashInfo[(unsigned char)ch]._count++;
		}
		HashInfo invalid;
		invalid._count = 0;

		HuffmanTree<HashInfo> tree(_hashInfo, 256, invalid);


		// �����ַ����벢������õ��ַ�������_hashInfo
		MakeCode(tree.getroot());

		//�������ļ����ļ���������д�뷽ʽ��/�������ļ�
		std::string newfilename(filename);
		newfilename += ".huffman";

		//����Դ�ļ��е��ַ�������_hashInfo�и����ַ��ı����������ļ�

		std::ofstream ofs(newfilename.c_str());
		
		//��ʼѹ��֮ǰ����Ҫ�������ļ�ͷ��д����ַ����ֵĴ���
		//�����ڽ�ѹ�ļ���ʼ֮ǰ�����¹���huffman��
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

		//���Ƚ��ļ��Ķ�ȡ��λ�����ã���ifs����ָ���ļ��Ŀ�ʼ
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
			//Դ�ļ��е��ַ��޷��������ϣ�ֱ�ӽ���ʱ��ֱֵ�Ӵ���
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



	void UnCompress(std::string filename)//��ѹ��
	{
		//��ѹ���ļ����н�ѹ��
		std::ifstream ifs(filename.c_str());
	
		//����Դ�ļ���
		std::string newfilename = filename.erase(filename.rfind('.'));
		newfilename += ".unhuffman";

		//�������ɵ�Դ�ļ���
		std::ofstream ofs(newfilename.c_str());
		
		//�ؽ�huffman������ѹ���ļ���ͷ������ĳ����ַ�����Ϣ����_countΪ0��һ���ڵ���ϢΪ������־		
		HashInfo Info;
		while (true)
		{

			ifs.read((char*)&Info, sizeof(Info));

			if (Info._count > 0)
			{
				_hashInfo[(unsigned char)Info._ch]._count = Info._count;
				//_hashInfo[(unsigned char)CharInfo._ch]._code = CharInfo._code;
				//���ch��_code
			}
			else
			{
				break;
			}

		}



		//����_hashInfo�ؽ�huffman��,�˴�����ʹ��hashInfoͰ���������¹���huffman������Ϊ�������ַ���Ȩֵ��ͬʱ�����ܱ�֤�����ַ���˳��һ����ͬ
		//���Դ˴������µ�huffman��ʱӦ��ʹ��������������Ҳ����˵��ÿһ���ڵ�����һ��ָ�벢ָ������parent
		HashInfo invalid;
		invalid._count = 0;

		HuffmanTree<HashInfo> tree(_hashInfo, 256, invalid);


		//�����ؽ���huffman���ָ��ļ�

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