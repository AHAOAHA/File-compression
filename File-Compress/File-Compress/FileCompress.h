#pragma once
/*
 * �ļ�ѹ��
 */

template<class W>
class FileCompress
{
public:
	FileCompress()
	{}
	void Compress(char* filename)//ѹ��
	{
		//1.�����ļ�����hashͰ
	}
	void UnCompress(char* filename);//��ѹ��
private:
	W _hashInfo[256];
};