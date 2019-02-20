#include"HuffmanTree.hpp"
#include"FileCompress.hpp"
#include<string>
#include<iostream>
int main()
{
	std::string filename;
	std::cout << "请输入要进行操作的文件名（请确保文件名完全并正确，建议使用完整路径）：";
	std::cin >> filename;
	std::cout << "请选择要进行的操作" << std::endl;
	std::cout << "0. 压缩" << std::endl;
	std::cout << "1. 解压缩" << std::endl;
	std::cout << "else. 退出" << std::endl;

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