#pragma once
/*
 * �ļ�ѹ��
 */
#include"HuffmanTree.h"
#include<string>
#include<fstream>

struct HashInfo//��ϣͰ�ڵ����ݽṹ
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
	FileCompress()//������ϣͰ
	{
		for (size_t i = 0; i < 256; ++i)//��ʼ����ϣͰ
		{
			_hashInfo[i]._ch = (char)i;
			_hashInfo[i]._count = 0;
		}
	}
	void Compress(const char* filename)//ѹ��
	{
		char ch;
		std::ifstream ifs(filename);

		//�����ļ��ַ���ͳ���ļ����ַ����ֵĴ���
		while (ifs.get(ch))
		{
			_hashInfo[ch]._count++;
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

		std::ofstream ofs(newfilename);
		//��ʼѹ��֮ǰ����Ҫ�������ļ�ͷ��д����ַ����ֵĴ���
		//�����ڽ�ѹ�ļ���ʼ֮ǰ�����¹���huffman��
		for (size_t i = 0; i < 256; ++i)
		{
			if (_hashInfo[i]._count > 0)
			{
				ofs.write((char*)&_hashInfo[i], sizeof(HashInfo));
			}
		}
		HashInfo Chinfo;
		Chinfo._count = 0;
		ofs.write((char*)&Chinfo, sizeof(HashInfo));

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
		HashInfo CharInfo;
		while (1)
		{

			ifs.read((char*)&CharInfo, sizeof(CharInfo));

			if (CharInfo._count != 0)
			{
				_hashInfo[(unsigned int)CharInfo._ch]._count = CharInfo._count;
				_hashInfo[(unsigned int)CharInfo._ch]._code = CharInfo._code;
				//���ch��_code
			}
			else
			{
				break;
			}
		}

			//����_hashInfo�ؽ�huffman��
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
					if (((1 << pos)&ch) != 0)
					{
						ptr = ptr->_right;
					}
					if (((1 << pos) & ch) == 0)
					{
						ptr = ptr->_left;
					}
					if (count == 0)
						break;

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

void TestCompress(const char* filename)
{
	FileCompress fc;
	fc.Compress(filename);
}

void TestUnCompress(std::string filename)
{
	FileCompress fc;
	fc.UnCompress(filename);
	

}