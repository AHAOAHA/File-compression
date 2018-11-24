#pragma once
/*
 * ����Huffman��
 */
#include<queue>

template<class W>
struct HuffmanTreeNode//���������Ľڵ�
{
	HuffmanTreeNode<W>* _left;//ָ����������ָ��
	HuffmanTreeNode<W>* _right;//ָ����������ָ��
	W _w;//�ڵ��ڵ�Ȩֵ�������ǽڵ��ڴ洢������

	HuffmanTreeNode(const W& w = W())
		:_w(w)
		,_left(nullptr)
		,_right(nullptr)
	{}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree()
		:_root(nullptr)
	{}

	struct NodeCompare
	{
		//Ϊʲô���ﲻ�������ñȽϣ�
		bool operator()(Node* l, Node* r)
		{
			return l->_w > r->_w;
		}
	};
	//����Huffman��
	HuffmanTree(W* w, size_t size, const W& invalid)//ʹ��һ��W���͵�������������������
	{
		std::priority_queue<Node*, std::vector<Node*>, NodeCompare> minheap;

		//�����ֹ����ַ���������������С���У�Ϊ����Huffman����׼��
		for (size_t i = 0; i < size; ++i)
		{
			if (w[i] != invalid)
			{
				minheap.push(new Node(w[i]));
			}
		}

		//��ʼ����Huffman��
		while (minheap.size() > 1)
		{
			Node* left = minheap.top();
			minheap.pop();
			Node* right = minheap.top();
			minheap.pop();

			
			Node* parent =new Node(left->_w + right->_w);//�ڴ˴���Ҫ�����½ڵ� ����ʹ��Node parent
			parent->_left = left;
			parent->_right = right;

			minheap.push(&*parent);
		}
		_root = minheap.top();
		minheap.pop();

	}

	Node*& getroot()
	{
		return _root;
	}

private:
	Node* _root;//����HuffmanTree�ĸ��ڵ�
};


