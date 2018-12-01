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


struct CharInfo
{
	CharInfo()
		:_ch('\0')
		, _count(0)
	{}
	char _ch;
	size_t _count;
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
	
	void Compress(const char* file)//ѹ��
	{
		char ch;
		std::string filename(file);
		std::ifstream ifs(file);
		

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
		
		filename += ".huffman";
		const std::string newfilename(filename);

		//����Դ�ļ��е��ַ�������_hashInfo�и����ַ��ı����������ļ�

		std::ofstream ofs(newfilename.c_str());
		
		//��ʼѹ��֮ǰ����Ҫ�������ļ�ͷ��д����ַ����ֵĴ���
		//�����ڽ�ѹ�ļ���ʼ֮ǰ�����¹���huffman��
		CharInfo chInfo;
		for (size_t i = 0; i < 256; ++i)
		{
			if (_hashInfo[i]._count > 0)
			{
				chInfo._ch = _hashInfo[i]._ch;
				chInfo._count = _hashInfo[i]._count;
				ofs.write((char*)&chInfo, sizeof(CharInfo));
			}
		}
		CharInfo end;
		end._count = 0;
		ofs.write((char*)&end, sizeof(CharInfo));

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
					tmpcode.resize(0);
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

		if(root->_left)
			MakeCode(root->_left);
		if(root->_right)
			MakeCode(root->_right);
	}




	void UnCompress(const char* CompressFile)//��ѹ��
	{
		//��ѹ���ļ�
		std::ifstream ifs(CompressFile);
	
		//����Դ�ļ���
		std::string tmpnewfilename(CompressFile);
		tmpnewfilename += ".unffman";
		const std::string newfilename(tmpnewfilename);
		

		//�������ɵ�Դ�ļ���
		std::ofstream ofs(newfilename.c_str());
		
		//�ؽ�huffman������ѹ���ļ���ͷ������ĳ����ַ�����Ϣ����_countΪ0��һ���ڵ���ϢΪ������־		
		CharInfo chInfo;

		while (1)
		{

			ifs.read((char*)&chInfo, sizeof(chInfo));

			if (chInfo._count > 0)
			{
				_hashInfo[(unsigned char)chInfo._ch]._count = chInfo._count;
				//_hashInfo[(unsigned char)CharInfo._ch]._code = CharInfo._code;
				//���ch��_code
			}
			else
			{
				break;
			}

		}

		//����_hashInfo�ؽ�huffman��,�˴�����ʹ��hashInfoͰ���������¹���huffman������Ϊ�������ַ���Ȩֵ��ͬʱ�����ܱ�֤�����ַ���˳��һ����ͬ
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
				{
					break;
				}

				if (((1 << pos)&ch) != 0)
				{
					if(ptr->_right)
						ptr = ptr->_right;
				}
				else if ((1 << pos&ch) == 0)
				{
					if(ptr->_left)
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

void TestCompress(const char* file)
{
	FileCompress fc;
	fc.Compress(file);
	
}

void TestUnCompress(const char* CompressFile)
{
	FileCompress fc;
	fc.UnCompress(CompressFile);
}
