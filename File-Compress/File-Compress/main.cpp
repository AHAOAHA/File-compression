#include"HuffmanTree.hpp"
#include"FileCompress.hpp"
#include<string>
#include<iostream>
int main()
{
	std::string filename;
	std::cout << "������Ҫ���в������ļ�������ȷ���ļ�����ȫ����ȷ������ʹ������·������";
	std::cin >> filename;
	std::cout << "��ѡ��Ҫ���еĲ���" << std::endl;
	std::cout << "0. ѹ��" << std::endl;
	std::cout << "1. ��ѹ��" << std::endl;
	std::cout << "else. �˳�" << std::endl;

	int i = 0;
	std::cin >> i;
	switch (i)
	{
	case 0:
		TestCompress(filename);
		break;
	case 1:
		TestUnCompress(filename);
		break;
	default:
		break;
	}
	
	return 0;
}