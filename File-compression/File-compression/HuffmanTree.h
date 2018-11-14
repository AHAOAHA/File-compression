#pragma once
/*
 * 构建哈夫曼树
 */

#include<queue>

template<class W>
struct HuffmanTreeNode//哈夫曼树的节点
{
public:
	HuffmanTreeNode<W>* _left;
	HuffmanTreeNode<W>* _right;
	HuffmanTreeNode<W>* _parent;
	W _w;
	HuffmanTreeNode(const W& w)
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
	{}
	struct NodeCompare
	{
		bool operator()(Node* l, Node* r)
		{
			return l->_w > r->_w;
		}
	};

	HuffmanTree(W* w, size_t n, const W& invalid)
	{
		//构建哈夫曼树
		//优先级队列，默认为大堆
		std::priority_queue<Node*, std::vector<Node*>, NodeCompare> minheap;

		for (size_t i = 0; i < n; ++i)
		{
			if(w[i] != invalid)
				minheap.push(new Node(w[i]));
		}

		//构建树
		while (minheap.size() > 1)
		{
			Node* left = minheap.top();
			minheap.pop();
			Node* right = minheap.top();
			minheap.pop();

			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			minheap.push(parent);
		}

		_root = minheap.top();

	}

	~HuffmanTree()
	{
		Destory(_root);
		_root = nullptr;
	}

	void Destory(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}
		Destory(root->_left);
		Destory(root->_right);
		delete root;
	}

	Node* GetRoot()
	{
		return _root;
	}
	
	//防拷贝
	HuffmanTree(const HuffmanTree<W>& t) = delete;
	HuffmanTree<W>& operator=(const HuffmanTree<W>& t) = delete;
protected:

	Node* _root;
};