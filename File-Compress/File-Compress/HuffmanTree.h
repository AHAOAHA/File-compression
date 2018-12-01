#pragma once
/*
 * ����Huffman��
 */
#include<queue>

template<class W>
struct HuffmanTreeNode//���������Ľڵ�
{
	W _w;//�ڵ��ڵ�Ȩֵ�������ǽڵ��ڴ洢������
	HuffmanTreeNode<W>* _left;//ָ����������ָ��
	HuffmanTreeNode<W>* _right;//ָ����������ָ��
	HuffmanTreeNode<W>* _parent;//ָ��parent��ָ�룬���ڵ��parentָ��Ϊnullptr
	

	HuffmanTreeNode(const W& w = W())
		:_w(w)
		,_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
	{}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree()
		:_root(nullptr)
	{
		std::cout << "HhuffmanTree()" << std::endl;
	}

	~HuffmanTree()
	{
		DestoryNode(_root);
		std::cout << "~huffmanTree()" << std::endl;
	}

	void DestoryNode(Node* root)
	{
		if (root == nullptr)
			return;

		if(root->_left)
			DestoryNode(root->_left);
		if(root->_right)
			DestoryNode(root->_right);
		
		delete root;
	}
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
		std::cout << "HuffmanTree(W* w, size_t size, const W& invalid)" << std::endl;
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
			left->_parent = parent;
			right->_parent = parent;

			minheap.push(parent);
		}
		_root = minheap.top();
		minheap.pop();
	}

	Node* getroot()
	{
		return _root;
	}

private:
	Node* _root;//����HuffmanTree�ĸ��ڵ�
};


