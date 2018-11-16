#pragma once
/*
 * 文件压缩
 */

template<class W>
class FileCompress
{
public:
	FileCompress()
	{}
	void Compress(char* filename)//压缩
	{
		//1.遍历文件构建hash桶
	}
	void UnCompress(char* filename);//解压缩
private:
	W _hashInfo[256];
};