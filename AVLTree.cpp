#include <iostream>
#include <malloc.h>
#include <cassert>
using namespace std;

template<typename Type>
class AVLTree
{
public:
	AVLTree(): root(NULL){}
	~AVLTree(){}
	
	bool Empty() const { return root == NULL; }
	bool Insert(const Type &x)
	{ 
		bool taller = true;
		return Insert(root, NULL, x, taller); 
	}
	bool Remove(const Type &x)
	{
		return Remove(root, x);
	}
	void InOrder()
	{
		InOrder(root);
	}
protected:
	struct AVLNode
	{
		Type data;
		AVLNode* leftchild;
		AVLNode* rightchild;
		AVLNode* parent;
		int balance;
	};
	static AVLNode *BuyNode()
	{
		AVLNode *p = (AVLNode*)malloc(sizeof(AVLNode));
		assert(p != NULL);
		
		return p;
	}
	static void FreeNode(AVLNode *ptr){
		free(ptr);
		ptr = NULL;
	}
	void InOrder(AVLNode *ptr)
	{
		if(ptr != NULL)
		{
			InOrder(ptr->leftchild);
			cout << ptr->data << " ";
			InOrder(ptr->rightchild);
		}
	}
	static void RotateRight(AVLNode *&ptr)
	{
		if(ptr == NULL) return;
		
		AVLNode *newroot = ptr->leftchild;
		
		ptr->leftchild = newroot->rightchild;
		if(newroot->rightchild != NULL)
			newroot->rightchild->parent = ptr;
		newroot->rightchild = ptr;
		newroot->parent = ptr->parent;
		ptr->parent = newroot;
	}
	static void RotateLeft(AVLNode *&ptr)
	{
		if(ptr == NULL) return;
		
		AVLNode *newroot = ptr->rightchild;
		
		ptr->rightchild = newroot->leftchild;
		if(newroot->leftchild != NULL)
			newroot->leftchild->parent = ptr;
		newroot->leftchild = ptr;
		newroot->parent = ptr->parent;
		ptr->parent = newroot;
	}
	static void RotateLeftToRight(AVLNode *&ptr)
	{
		if(ptr == NULL) return;
		
		RotateLeft(ptr->leftchild);
		RotateRight(ptr);
	}
	static void RotateRightToLeft(AVLNode *&ptr)
	{
		if(ptr == NULL) return;
		
		RotateRight(ptr->rightchild);
		RotateLeft(ptr);
	}
	static void LeftBalance(AVLNode *&ptr)
	{
		if(ptr == NULL) return;
		
		AVLNode *left = ptr->leftchild;
		AVLNode *rightsub = left->rightchild;
		
		switch(left->balance)
		{
		case -1: RotateRight(ptr); ptr->balance = left->balance = 0; break;
		case 0: cout << "Left already balanced !" << endl; break;
		case 1: 
			switch(rightsub->balance)
			{
			case -1:  left->balance = -1; ptr->balance = 0;  break;
			case 0:  left->balance = ptr->balance = 0;     break;
			case 1: left->balance = 0; ptr->balance = 1; break;
			}
			RotateLeftToRight(ptr);
			break;
		}
	}
	static void RightBalance(AVLNode *&ptr)
	{
		if(ptr == NULL) return;
		
		AVLNode *rightsub = ptr->rightchild;
		AVLNode *leftsub = ptr->leftchild;
		
		switch(rightsub->balance)
		{
		case 1: RotateLeft(ptr); ptr->balance = rightsub->balance = 0; break;
		case 0: cout << "Right already balanced !"; break;
		case -1: 
			switch(leftsub->balance)
			{
			case 1:  rightsub->balance = 1; ptr->balance = 0;  break;
			case 0:  rightsub->balance = ptr->balance = 0;     break;
			case -1:  rightsub->balance = 0; ptr->balance = -1; break;
			}
			RotateRightToLeft(ptr);
			break;
		}
	}
	static bool Insert(AVLNode* &ptr, AVLNode *pa, const Type &x, bool &taller)
	{
		if(ptr == NULL)
		{
			ptr = BuyNode();
			ptr->data = x;
			ptr->leftchild = ptr->rightchild = NULL;
			ptr->parent = pa;
			taller = true;
			
			return true;
		}
		
		if(ptr->data < x)
		{	
			if(Insert(ptr->rightchild, ptr, x, taller))
			{
				if(taller)
				{
					switch(ptr->balance)
					{ 
					case -1:  LeftBalance(ptr); taller = false; break;
					case 0:  ptr->balance = -1; taller = true;  break;
					case 1: ptr->balance = 0; taller = false; break;
					}
				}
			}
		}
		else if(ptr->data > x)
		{
			if(Insert(ptr->leftchild, ptr, x, taller))
			{
				if(taller)
				{
					switch(ptr->balance)
					{ 
					case 1: RightBalance(ptr); taller = false; break;
					case 0:  ptr->balance = 1; taller = true;  break;
					case -1:  ptr->balance = 0;  taller = false; break;
					}
				}
			}
		}
		
		return false;
	}
	static AVLNode *Frist(AVLNode *ptr)
	{
		if(ptr == NULL) return NULL;

		while(ptr != NULL && ptr->leftchild != NULL)
		{
			ptr = ptr->leftchild;
		}

		return ptr;
	}
	static AVLNode *Next(AVLNode *ptr)
	{
		if(ptr == NULL) return NULL;

		if(ptr->rightchild != NULL)
		{
			return Frist(ptr->rightchild);
		}
		AVLNode *pa = ptr->parent;
		while(pa != NULL && pa->leftchild != ptr)
		{
			ptr = pa;
			pa = pa->parent;
		}

		return pa;
	}
	static void RemoveRightBalance(AVLNode*& node, bool& taller)
	{
		AVLNode*& rightsub = node->rightchild;
		AVLNode* leftsub = NULL;

		switch (rightsub->balance)
		{
		case 0: node->balance = 1; rightsub->balance = -1; RotateRight(node); taller = false; break;
		case 1: node->balance = rightsub->balance = 0; RotateRight(node); break;
		case -1:
			leftsub = rightsub->leftchild;
			switch (leftsub->balance)
			{
			case 1: node->balance = -1; rightsub->balance = 0; break;
			case -1: node->balance = 0; rightsub->balance = 1;  break;
			case 0: node->balance = rightsub->balance = 0;     break;
			}
			leftsub->balance = 0;
			RotateLeftToRight(node);
			break;
		}
	}
	static void RemoveLeftBalance(AVLNode*& node,bool& taller)
	{
		AVLNode*& leftsub = node->leftchild;
		AVLNode* rightsub = NULL;

		switch (leftsub->balance)
		{
		case 0: node->balance = -1; leftsub->balance = 1; RotateLeft(node); taller = false; break;
		case -1: node->balance = leftsub->balance = 0; RotateLeft(node); break;
		case 1: 
			rightsub = leftsub->rightchild;
			switch (rightsub->balance)
			{
			case 1: node->balance = 0; leftsub->balance = -1; break;
			case -1: node->balance = 1; leftsub->balance = 0;  break;
			case 0: node->balance = leftsub->balance = 0; break;
			}
			rightsub->balance = 0;
			RotateRightToLeft(node);
			break;
		}

	}
	
	static bool Remove(AVLNode*& node, const Type data, bool& taller)
	{
		if ( node == NULL) return false;

		bool bRet = false;
		if (node->data > data)
		{
			bRet = Remove(node->leftchild, data, taller);

			if( bRet && taller )
				switch (node->balance)
				{
				case 0: node->balance = 1; taller = false; break;
				case -1: node->balance = 0; break;
				case 1: RemoveRightBalance(node, taller); taller = false; break;
				}
		}
		else if (node->data < data)
		{
			bRet = Remove(node->rightchild, data, taller);

			if( bRet && taller )
				switch (node->balance)
				{
				case 0: node->balance = -1; taller = false; break;
				case 1: node->balance = 0; break;
				case -1: RemoveLeftBalance(node, taller); taller = false; break;
				}
		}
		else
		{
			AVLNode* pa = node->parent;
			AVLNode* ptr = NULL;
			if (node->leftchild != NULL && node->rightchild != NULL)
			{
				AVLNode* lnode = node;
				ptr = node->leftchild;
				while ( ptr->rightchild != NULL) ptr = ptr->rightchild;
				Type tmp = ptr->data;

				if( node->leftchild == ptr)
				{ 
					bRet = Remove(node, ptr->data, taller);
					lnode->data = tmp;
					if (bRet && taller)
						switch (node->balance)
						{
						case 0: node->balance = 1; taller = false; break;
						case -1: node->balance = 0; break;
						case 1: RemoveLeftBalance(node, taller); break;
						}
				}
				else
				{
					bRet = Remove(node, ptr->data, taller);
					lnode->data = tmp;
					if (bRet && taller)
						switch (node->balance)
						{
						case 0: node->balance = -1; taller = false; break;
						case 1: node->balance = 0; break;
						case -1: RemoveRightBalance(node,taller); break;
						}
				}
			}
			else if (node->leftchild == NULL)//删除单分支，左分支为空
			{
				ptr = node->rightchild;
				if( ptr != NULL)
					ptr->parent = node->parent;
				FreeNode(node);
				node = ptr;

				bRet = true;
			}
			else if (node->rightchild == NULL)//删除单分支，右分支为空
			{
				ptr = node->leftchild;
				if( ptr != NULL)
					ptr->parent = node->parent;
				FreeNode(node);
				node = ptr;

				bRet = true;
			}
		}
		return bRet;
	}
	
	bool Remove(AVLNode *&ptr, const Type &x)
	{
		if(ptr == NULL) return false;
		bool taller = true;
		return Remove(ptr, x, taller);
	}
private:
	AVLNode *root;
};

int main()
{
	int arr[] = { 12,23,34, 80, 90, 85, 100};
	int n = sizeof(arr) / sizeof(arr[0]);
	AVLTree<int> myt;
	int value;
	for(int i = 0; i < 100; ++i)
	{
		value = rand()%100;
		myt.Insert(value);
	}
	/*for(int i = 0; i < n; ++i)
	{
		myt.Insert(arr[i]);
	}*/
	myt.InOrder();
	cout << endl;
	int x;
	while(cin>>x, x != -1)
	{
		myt.Remove(x);
		myt.InOrder();
		cout << endl;
	}

	return 0;
}



