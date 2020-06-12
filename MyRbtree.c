
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int KEY_TYPE;

#define RED 1
#define BLACK 2

//����ڵ���Ϣ
typedef struct _rbtree_node
{
	//���� ��ɫ
	unsigned char color;
	//�����������������������ڵ�
	struct _rbtree_node *left;
	struct _rbtree_node *right;
	struct _rbtree_node *parrent;

	//����ֵ
	KEY_TYPE key;
	void *value;
}rbtree_node;

//��������
typedef struct _rbtree
{
	//���ڵ�
	//struct _rbtree_node *root;
	rbtree_node *root;
	//Ҷ�ӽڵ�ȫ������Ϊ��ɫ
	//struct _rbtree_node *nil;
	rbtree_node *nil;
}rbtree;

//���Һ������Сֵ
rbtree_node *rbtree_mini(rbtree *T,rbtree_node *x)
{
	while(x->left != T->nil)
	{
		x = x->left; 
	}

	return x;
}

//���Һ�������ֵ
rbtree_node *rbtree_maxi(rbtree *T,rbtree_node *x)
{
	while(x->right != T->nil)
	{
		x = x->right;
	}

	return x;
}


//��ת�Ǻ�����Ļ���
//Ϊ���ж�Ҷ�ӽڵ����صĶ��Ǻ�ɫ������Ҫ�������������������
//�Ե�ǰ�ڵ�Ϊ���ģ������ǰ�ڵ��ǿ����ҵ�
//����������ĺ���
void rbtree_left_rotate(rbtree *T,rbtree_node *x)
{
	//y����x��������
	rbtree_node *y = x->right;
	
	//1����.����x��������ָ��ԭ��y��������
	x->right = y->left;
	//���y������������Ӱ�ؽڵ�
	if(y->left != T->nil)
	{
		//2����.ԭ��y���������ĸ��ڵ�ָ��x
		y->left->parrent = x;
	}

	//������ת��ʱ��x�Ǹ��ڵ�,��ת��ɺ�y��Ϊ���ڵ�
	
	//3����.����y��parrentָ��ԭ��x��parrent
	y->parrent = x->parrent;

	//4����.���x�ĸ��ڵ���Ҷ�ӽڵ㣬�ǿգ�����x��root�ڵ㡣
	if(x->parrent == T->nil)
	{
		//�Ǹ��ڵ�ָ��y
		T->root = y;

	}else if(x == x->parrent->left)
	{
		//���x�Ǹ��ڵ��������,��ô�Ͱ�ԭ��x���ڵ����������ָ��y
		x->parrent->left = y;
		
	}else
	{
		//��������Ͱ�ԭ��x���ڵ����������ָ��y
		x->parrent->right = y;
		
	}

	//5����.����y��������ָ����x
	y->left = x;
	//6����.����x�ĸ��ڵ�ָ����y
	x->parrent = y;
}


//x--y:��Ҫ��
//y-->x:��Ҫ��
//left-->right:��Ҫ��
//right-->left:��Ҫ��

void rbtree_right_rotate(rbtree *T,rbtree_node *y)
{
	//y����x��������
	rbtree_node *x = y->left;
	
	//1����.����x��������ָ��ԭ��y��������
	y->left = x->right;
	//���y������������Ӱ�ؽڵ�
	if(x->right != T->nil)
	{
		//2����.ԭ��y���������ĸ��ڵ�ָ��x
		x->right->parrent = y;
	}

	//������ת��ʱ��x�Ǹ��ڵ�,��ת��ɺ�y��Ϊ���ڵ�
	
	//3����.����y��parrentָ��ԭ��x��parrent
	x->parrent = y->parrent;

	//4����.���x�ĸ��ڵ���Ҷ�ӽڵ㣬�ǿգ�����x��root�ڵ㡣
	if(y->parrent == T->nil)
	{
		//�Ǹ��ڵ�ָ��y
		T->root = x;

	}else if(y == y->parrent->right)
	{
		//���x�Ǹ��ڵ��������,��ô�Ͱ�ԭ��x���ڵ����������ָ��y
		y->parrent->right= x;
		
	}else
	{
		//��������Ͱ�ԭ��x���ڵ����������ָ��y
		y->parrent->left= x;
		
	}

	//5����.����y��������ָ����x
	x->right= y;
	//6����.����x�ĸ��ڵ�ָ����y
	y->parrent = x;
}

//���ڵ�Ϊ��ɫ������Ҫ����������������Ӱ��ڸ�
void rbtree_insert_fixup(rbtree * T,rbtree_node * z)
{

	while(z->parrent->color == RED)
	{
		//������ڵ����游�ڵ��������
		if(z->parrent == z->parrent->parrent->left)
		{	
			//��ȡ�常�ڵ�
			rbtree_node *y = z->parrent->parrent->right;
			//����常�ڵ��Ǻ�ɫ
			//������2�����
			if(y->color == RED)
			{
				//�Ѹ��ڵ���ɫ��Ϊ��ɫ
				z->parrent->color = BLACK;
				//��ǰ�ڵ�Ҳ��Ϊ��ɫ
				y->color = BLACK;
				//�游�ڵ��Ϊ��ɫ
				z->parrent->parrent->color = RED;

				//�����游�ڵ�Ϊ��ת���ɵ����ڸ�
				z = z->parrent->parrent;
				
			}else
			{
				//����常�ڵ��Ǻ�ɫ,���ʱ�����Ҫ��ת
				if(z == z->parrent->right)
				{
					//���ʱ��,���ڵ���������ڵ�����࣬�Ը��ڵ��������
					z = z->parrent;
					rbtree_left_rotate(T,z);
					
				}
				//��ɫ
				z->parrent->color = BLACK;
				z->parrent->parrent->color = RED;
				//�ٽ�������
				rbtree_right_rotate(T, z->parrent->parrent);
			}
		}else
		{
			//������ڵ����游�ڵ��������
			rbtree_node *y = z->parrent->parrent->left;
			if(y->color == RED)
			{
				//�ı���ɫ
				z->parrent->color = BLACK;
				y->color = BLACK;
				z->parrent->parrent->color = RED;
				//���ĵ�
				z = z->parrent->parrent;

			}else
			{

				if(z == z->parrent->left)
				{

					z = z->parrent;
					//����
					rbtree_right_rotate(T,z);
				}
				//��ת�ڶ���
				z->parrent->color = BLACK;
				z->parrent->parrent->color = RED;
				//����
				rbtree_left_rotate(T,z->parrent->parrent);
			}

		}

	}

	T->root->color = BLACK;
	
}

void rbtree_insert(rbtree *T,rbtree_node *z)
{

	//y��Ҷ�ӽڵ�
	rbtree_node *y = T->nil;
	//x�Ǹ��ڵ�
	rbtree_node *x = T->root;

	
	while(x != T->nil)
	{

		//ֻҪx����Ҷ�ӽڵ�
		y = x;
		if(z->key < x->key)
		{
			//���Ҫ�����ֵС�ڵ�ǰ�ڵ��ֵ���Ǿ�����ǰ�ڵ����������
			x = x->left;
		}else if(z->key > x->key)
		{
			//���Ҫ�����ֵС�ڵ�ǰ�ڵ��ֵ���Ǿ�����ǰ�ڵ����������
			x = x->right;
		}else{
			//���ʾҪ����Ľڵ��Ѿ�������
			return;
		}
	
	}

	//ָ��ڵ��ĩ��,��z�ڵ�������
	z->parrent = y;
	if(y == T->nil)
	{
		//���y��Ҷ�ӽڵ�Ϊ��
		T->root = z;
	}else if(z->key < y->key)
	{
		y->left = z;
	}else
	{
		y->right = z;
	}
		//����ڵ����������ָ���
		z->left= T->nil;
		z->right = T->nil;
		//����ڵ����ɫ����Ǻ�ɫ����ɫ��Ӱ��ڸ�
		z->color = RED;

		//�����˲���
		rbtree_insert_fixup(T,z);
		
		
}

//Ѱ�ҽڵ�

rbtree_node *rbtree_successor(rbtree *T,rbtree_node *x)
{

	rbtree_node *y = x->parrent;

	//���������������
	if(x->right != T->nil)
	{
		//Ѱ����С�ڵ�
		return rbtree_mini(T,x->right);
	}

	//Ѱ��x���ڵ�
	while((y != T->nil) && (x == y->right))
	{
		x = y;
		y = y->parrent;
	}

	return y;
}

//����Ƴ��Ľڵ��Ǻ�ɫ�ģ����ʱ����Ҫ����
void rbtree_delete_fixup(rbtree * T,rbtree_node * x)
{

	//��ɫ�ĵ�
	while((x != T->root) && (x->color == BLACK))
	{
		//�����������
		if(x == x->parrent->left)
		{
			
			rbtree_node *w = x->parrent->right;
			//�������������ɫ�Ǻ�ɫ
			if(w->color == RED)
			{
				//�ı���ɫ
				w->color = BLACK;
				x->parrent->color = RED;

				//����
				rbtree_left_rotate(T,x->parrent);
				w = x->parrent->right;

			}

			//����������Ǻ�ɫ���������Ǻ�ɫ
			if((w->left->color == BLACK) && (w->right->color == BLACK))
			{

				//�ı���ɫ
				w->color = RED;
				//�����ƶ����ڵ�
				x = x->parrent;
				
			}else{

				//���������Ǻ�ɫ,�������Ǻ�ɫ
				if(w->right->color == BLACK)
				{
					//�ı���ɫ
					w->left->color = BLACK;
					w->color = RED;
					//������
					rbtree_right_rotate(T,w);
					w = x->parrent->right;
				}

				//������
				w->color = x->parrent->color;
				x->parrent->color = BLACK;
				//
				w->right->color = BLACK;
				rbtree_left_rotate(T,x->parrent);

				x = T->root;
			}

		}else
		{
			//������
			rbtree_node *w = x->parrent->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->parrent->color = RED;
				//����
				rbtree_right_rotate(T,x->parrent);
				w = x->parrent->left;
			}

			//����������Ǻ�ɫ��������Ҳ�Ǻ�ɫ
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
	//�����ǰֻ��һ���ڵ�
	if((z->left == T->nil) || (z->right == T->nil))
	{
		y = z;
	}else
	{
		//������������Ϊ�յ����
		//Ѱ�ҽڵ�
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
		//���û��Ҷ�ӽڵ�
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

	//����
	if(y->color == BLACK)
	{
		rbtree_delete_fixup(T,x);
	}

	/*
	//��������ظ�д�ˣ�������ڴ�δ���
		if(y->color == BLACK)
	{
		rbtree_delete_fixup(T,x);
	}

	*/
	

	return y;
}

//�����ڵ�
rbtree_node *rbtree_search(rbtree *T,KEY_TYPE key)
{

	rbtree_node *node = T->root;
	while(node != T->nil)
	{
		if(key < node->key)
		{ //С�ڵ�ǰ�ڵ㣬�嵽������
			node = node->left;
		}else if(key > node->key)
		{
			//���ڵ�ǰ�ڵ㣬�嵽������
			node = node->right;
		}else
		{
			return node;
		
		}

	}
	return T->nil;
}
//�������
void rbtree_traversal(rbtree *T,rbtree_node *node)
{
	if(node != T->nil)
	{
		//�ݹ�
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
		//����
		rbtree_insert(T,node);

	}
	//�������
	rbtree_traversal(T,T->root);
	printf("1----------------------------------------\n");
	for(i = 0; i < 20; i++)
	{
		//����
		rbtree_node *node = rbtree_search(T,keyArr[i]);
		//ɾ��
		rbtree_node *cur = rbtree_delete(T,node);
		//�ͷ�
		free(cur);
		//����
		rbtree_traversal(T,T->root);
		printf("2----------------------------------------\n");
	}
}

