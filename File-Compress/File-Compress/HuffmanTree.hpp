#pragma once
/*
 * 构建Huffman树
 */
#include<queue>

template<class W>
struct HuffmanTreeNode//哈夫曼树的节点
{
	W _w;//节点内的权值，即就是节点内存储的数据
	HuffmanTreeNode<W>* _left;//指向左子树的指针
	HuffmanTreeNode<W>* _right;//指向右子树的指针
	HuffmanTreeNode<W>* _parent;//指向parent的指针，根节点的parent指针为nullptr
	

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
		//为什么这里不能用引用比较？
		bool operator()(Node* l, Node* r)
		{
			return l->_w > r->_w;
		}
	};
	//构建Huffman树
	HuffmanTree(W* w, size_t size, const W& invalid)//使用一个W类型的数组来构建哈夫曼树
	{
		std::cout << "HuffmanTree(W* w, size_t size, const W& invalid)" << std::endl;
		std::priority_queue<Node*, std::vector<Node*>, NodeCompare> minheap;

		//将出现过的字符保存至所创建的小堆中，为构建Huffman树做准备
		for (size_t i = 0; i < size; ++i)
		{
			if (w[i] != invalid)
			{
				minheap.push(new Node(w[i]));
			}
		}

		//开始构建Huffman树
		while (minheap.size() > 1)
		{
			Node* left = minheap.top();
			minheap.pop();
			Node* right = minheap.top();
			minheap.pop();

			
			Node* parent =new Node(left->_w + right->_w);//在此处需要开辟新节点 不能使用Node parent
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
	Node* _root;//代表HuffmanTree的根节点
};


