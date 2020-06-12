
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int KEY_TYPE;

#define RED 1
#define BLACK 2

//定义节点信息
typedef struct _rbtree_node
{
	//声明 颜色
	unsigned char color;
	//声明左子树，右子树，符节点
	struct _rbtree_node *left;
	struct _rbtree_node *right;
	struct _rbtree_node *parrent;

	//声明值
	KEY_TYPE key;
	void *value;
}rbtree_node;

//定义红黑树
typedef struct _rbtree
{
	//父节点
	//struct _rbtree_node *root;
	rbtree_node *root;
	//叶子节点全部隐藏为黑色
	//struct _rbtree_node *nil;
	rbtree_node *nil;
}rbtree;

//查找红黑树最小值
rbtree_node *rbtree_mini(rbtree *T,rbtree_node *x)
{
	while(x->left != T->nil)
	{
		x = x->left; 
	}

	return x;
}

//查找红黑树最大值
rbtree_node *rbtree_maxi(rbtree *T,rbtree_node *x)
{
	while(x->right != T->nil)
	{
		x = x->right;
	}

	return x;
}


//旋转是红黑树的基础
//为了判断叶子节点隐藏的都是黑色，那需要把整个红黑树都传进来
//以当前节点为轴心，这个当前节点是可以找到
//这个是左旋的函数
void rbtree_left_rotate(rbtree *T,rbtree_node *x)
{
	//y等于x的右子树
	rbtree_node *y = x->right;
	
	//1方向.现在x的右子树指向原来y的左子树
	x->right = y->left;
	//如果y的左子树不是影藏节点
	if(y->left != T->nil)
	{
		//2方向.原来y的左子树的父节点指向x
		y->left->parrent = x;
	}

	//这里旋转的时候，x是根节点,旋转完成后，y变为根节点
	
	//3方向.现在y的parrent指向原来x的parrent
	y->parrent = x->parrent;

	//4方向.如果x的父节点是叶子节点，是空，代表x是root节点。
	if(x->parrent == T->nil)
	{
		//那根节点指向y
		T->root = y;

	}else if(x == x->parrent->left)
	{
		//如果x是父节点的左子树,那么就把原来x父节点的左子树，指向y
		x->parrent->left = y;
		
	}else
	{
		//这种情况就把原来x父节点的右子树，指向y
		x->parrent->right = y;
		
	}

	//5方向.现在y的左子树指向了x
	y->left = x;
	//6方向.现在x的父节点指向了y
	x->parrent = y;
}


//x--y:需要换
//y-->x:需要换
//left-->right:需要换
//right-->left:需要换

void rbtree_right_rotate(rbtree *T,rbtree_node *y)
{
	//y等于x的右子树
	rbtree_node *x = y->left;
	
	//1方向.现在x的右子树指向原来y的左子树
	y->left = x->right;
	//如果y的左子树不是影藏节点
	if(x->right != T->nil)
	{
		//2方向.原来y的左子树的父节点指向x
		x->right->parrent = y;
	}

	//这里旋转的时候，x是根节点,旋转完成后，y变为根节点
	
	//3方向.现在y的parrent指向原来x的parrent
	x->parrent = y->parrent;

	//4方向.如果x的父节点是叶子节点，是空，代表x是root节点。
	if(y->parrent == T->nil)
	{
		//那根节点指向y
		T->root = x;

	}else if(y == y->parrent->right)
	{
		//如果x是父节点的左子树,那么就把原来x父节点的左子树，指向y
		y->parrent->right= x;
		
	}else
	{
		//这种情况就把原来x父节点的右子树，指向y
		y->parrent->left= x;
		
	}

	//5方向.现在y的左子树指向了x
	x->right= y;
	//6方向.现在x的父节点指向了y
	y->parrent = x;
}

//父节点为红色，就需要调整红黑树，否则会影响黑高
void rbtree_insert_fixup(rbtree * T,rbtree_node * z)
{

	while(z->parrent->color == RED)
	{
		//如果父节点是祖父节点的左子树
		if(z->parrent == z->parrent->parrent->left)
		{	
			//获取叔父节点
			rbtree_node *y = z->parrent->parrent->right;
			//如果叔父节点是红色
			//这里有2种情况
			if(y->color == RED)
			{
				//把父节点颜色变为黑色
				z->parrent->color = BLACK;
				//当前节点也变为黑色
				y->color = BLACK;
				//祖父节点变为红色
				z->parrent->parrent->color = RED;

				//再以祖父节点为旋转即可调整黑高
				z = z->parrent->parrent;
				
			}else
			{
				//如果叔父节点是黑色,这个时候就需要旋转
				if(z == z->parrent->right)
				{
					//这个时候,父节点的右子树节点个数多，以父节点进行左旋
					z = z->parrent;
					rbtree_left_rotate(T,z);
					
				}
				//定色
				z->parrent->color = BLACK;
				z->parrent->parrent->color = RED;
				//再进行右旋
				rbtree_right_rotate(T, z->parrent->parrent);
			}
		}else
		{
			//如果父节点是祖父节点的右子树
			rbtree_node *y = z->parrent->parrent->left;
			if(y->color == RED)
			{
				//改变作色
				z->parrent->color = BLACK;
				y->color = BLACK;
				z->parrent->parrent->color = RED;
				//轴心点
				z = z->parrent->parrent;

			}else
			{

				if(z == z->parrent->left)
				{

					z = z->parrent;
					//右旋
					rbtree_right_rotate(T,z);
				}
				//旋转第二次
				z->parrent->color = BLACK;
				z->parrent->parrent->color = RED;
				//左旋
				rbtree_left_rotate(T,z->parrent->parrent);
			}

		}

	}

	T->root->color = BLACK;
	
}

void rbtree_insert(rbtree *T,rbtree_node *z)
{

	//y是叶子节点
	rbtree_node *y = T->nil;
	//x是根节点
	rbtree_node *x = T->root;

	
	while(x != T->nil)
	{

		//只要x不是叶子节点
		y = x;
		if(z->key < x->key)
		{
			//如果要插入的值小于当前节点的值，那就往当前节点的左子树走
			x = x->left;
		}else if(z->key > x->key)
		{
			//如果要插入的值小于当前节点的值，那就往当前节点的右子树走
			x = x->right;
		}else{
			//这表示要插入的节点已经存在了
			return;
		}
	
	}

	//指向节点的末端,把z节点插入进来
	z->parrent = y;
	if(y == T->nil)
	{
		//如果y的叶子节点为空
		T->root = z;
	}else if(z->key < y->key)
	{
		y->left = z;
	}else
	{
		y->right = z;
	}
		//插入节点的左右子树指向空
		z->left= T->nil;
		z->right = T->nil;
		//插入节点的颜色最好是红色，黑色会影响黑高
		z->color = RED;

		//别忘了插入
		rbtree_insert_fixup(T,z);
		
		
}

//寻找节点

rbtree_node *rbtree_successor(rbtree *T,rbtree_node *x)
{

	rbtree_node *y = x->parrent;

	//如果有子树有子树
	if(x->right != T->nil)
	{
		//寻找最小节点
		return rbtree_mini(T,x->right);
	}

	//寻找x父节点
	while((y != T->nil) && (x == y->right))
	{
		x = y;
		y = y->parrent;
	}

	return y;
}

//如果移除的节点是黑色的，这个时候需要调整
void rbtree_delete_fixup(rbtree * T,rbtree_node * x)
{

	//黑色的点
	while((x != T->root) && (x->color == BLACK))
	{
		//如果是左子树
		if(x == x->parrent->left)
		{
			
			rbtree_node *w = x->parrent->right;
			//如果右子树的颜色是红色
			if(w->color == RED)
			{
				//改变作色
				w->color = BLACK;
				x->parrent->color = RED;

				//左旋
				rbtree_left_rotate(T,x->parrent);
				w = x->parrent->right;

			}

			//如果左子树是黑色，右子树是黑色
			if((w->left->color == BLACK) && (w->right->color == BLACK))
			{

				//改变作色
				w->color = RED;
				//重新制定父节点
				x = x->parrent;
				
			}else{

				//左子树不是黑色,右子树是黑色
				if(w->right->color == BLACK)
				{
					//改变颜色
					w->left->color = BLACK;
					w->color = RED;
					//并右旋
					rbtree_right_rotate(T,w);
					w = x->parrent->right;
				}

				//再左旋
				w->color = x->parrent->color;
				x->parrent->color = BLACK;
				//
				w->right->color = BLACK;
				rbtree_left_rotate(T,x->parrent);

				x = T->root;
			}

		}else
		{
			//右子树
			rbtree_node *w = x->parrent->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parrent->color = RED;
				//右旋
				rbtree_right_rotate(T,x->parrent);
				w = x->parrent->left;
			}

			//如果左子树是黑色，右子树也是黑色
			if((w->left->color == BLACK) && (w->right->color == BLACK))
			{
				w->color = RED;
				x = x->parrent;

			}else
			{
				if(w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					rbtree_left_rotate(T,w);
					w = x->parrent->left;
				}


				w->color = x->parrent->color;
				x->parrent->color = BLACK;
				w->left->color = BLACK;
				rbtree_right_rotate(T,x->parrent);

				x = T->root;
				
			}
			
		}
	}

	x->color = BLACK;
}

rbtree_node *rbtree_delete(rbtree * T,rbtree_node * z)
{

	rbtree_node *y = T->nil;
	rbtree_node *x = T->nil;
	//如果当前只有一个节点
	if((z->left == T->nil) || (z->right == T->nil))
	{
		y = z;
	}else
	{
		//左右子树都不为空的情况
		//寻找节点
		y = rbtree_successor(T,z);
	}

	if(y->left != T->nil)
	{
		x = y->left;

	}else if(y->right != T->nil)
	{
		x = y->right;
	}
	
	x->parrent = y->parrent;
	if(y->parrent == T->nil)
	{
		//如果没有叶子节点
		T->root = x;

	}else if(y == y->parrent->left)
	{
		y->parrent->left = x;
		
	}else
	{
		y->parrent->right = x;
	}

	if(y != z)
	{
		z->key = y->key;
		z->value = y->value;
	}

	//调整
	if(y->color == BLACK)
	{
		rbtree_delete_fixup(T,x);
	}

	/*
	//如果这里重复写了，会出现内存段错误
		if(y->color == BLACK)
	{
		rbtree_delete_fixup(T,x);
	}

	*/
	

	return y;
}

//搜索节点
rbtree_node *rbtree_search(rbtree *T,KEY_TYPE key)
{

	rbtree_node *node = T->root;
	while(node != T->nil)
	{
		if(key < node->key)
		{ //小于当前节点，插到左子树
			node = node->left;
		}else if(key > node->key)
		{
			//大于当前节点，插到右子树
			node = node->right;
		}else
		{
			return node;
		
		}

	}
	return T->nil;
}
//中序遍历
void rbtree_traversal(rbtree *T,rbtree_node *node)
{
	if(node != T->nil)
	{
		//递归
		rbtree_traversal(T,node->left);
		printf("key:%d, color:%d\n", node->key, node->color);
		rbtree_traversal(T,node->right);
	}
}

int main()
{
	int keyArr[20] = {24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15};

	rbtree *T = (rbtree *)malloc(sizeof(rbtree));
	if(T == NULL)
	{

		printf("malloc failed\n");
		return -1;
	}

	T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
	T->nil->color = BLACK;
	T->root = T->nil;

	rbtree_node *node = T->nil;
	int i = 0;
	for(i = 0; i < 20; i++)
	{

		node = (rbtree_node*)malloc(sizeof(rbtree_node));
		node->key = keyArr[i];
		node->value = NULL;
		//插入
		rbtree_insert(T,node);

	}
	//中序遍历
	rbtree_traversal(T,T->root);
	printf("1----------------------------------------\n");
	for(i = 0; i < 20; i++)
	{
		//搜索
		rbtree_node *node = rbtree_search(T,keyArr[i]);
		//删除
		rbtree_node *cur = rbtree_delete(T,node);
		//释放
		free(cur);
		//遍历
		rbtree_traversal(T,T->root);
		printf("2----------------------------------------\n");
	}
}

