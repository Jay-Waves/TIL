# 概述

## 声明
```c
#define ELEMENTTYPE int
#ifndef _TREE_H
#define _TREE_H
/* |left_h - right_h|<=1 */
    struct NODE;
    typedef struct NODE *POSITION_;
    typedef struct NODE *AVL_TREE_;

    static int Height(POSITION_ p);
    int Max(int a, int b);
    static POSITION_ SingleRotateLeft(AVL_TREE_ t);
    static POSITION_ DoubleRotateLeft(AVL_TREE_ t);
    static POSITION_ SingleRotateRight(AVL_TREE_ t);
    static POSITION_ DoubleRotateRight(AVL_TREE_ t);

    /*AVL_TREE_ MakeEmpty(AVL_TREE_ T);
    POSITION_ Find(ELEMENTTYPE, AVL_TREE_ T);
    POSITION_ FindMin(AVL_TREE_ T);
    POSITION_ FindMax(AVL_TREE_ T);
	AVL_TREE_ Delete(ELEMENTTYPE, AVL_TREE_ T);
    ELEMENTTYPE Retrieve(POSITION_ P);*/
    AVL_TREE_ Insert(ELEMENTTYPE, AVL_TREE_ T); //only one avaible now
    
    struct NODE{
        ELEMENTTYPE value;
        AVL_TREE_ left;
        AVL_TREE_ right;
        int height;
        int deleted;
    };
#endif
```
## 核心例程
1. 未实现删除, 过于复杂. 
2. 主要实现例程:
	- 插入!
	- 左/右单节点旋转
	- 左/右双节点旋转
```c
/*height of empty tree is -1*/
static int 
Height(POSITION_ p)
{
    if (p == NULL)
        return -1;
    else 
        return p->height;
}

/*do nothing while x existed already*/
AVL_TREE_
Insert(ELEMENTTYPE x, AVL_TREE_ t)
{
    if (t == NULL)
    {
        /*create and return a one-node tree*/
        t= (AVL_TREE_)malloc(sizeof(struct NODE));
        if (t == NULL)
            return NULL;//"out of space"
        else
        {
            t->value = x;
            t->height = 0;
            t->left= t->right = NULL;
        }
    }
    else
    if (x < t->value)
    {
        t->left = Insert(x, t->left);
        if (Height(t->left) - Height(t->right) == 2 
			|| Height(t->left) - Height(t->right) == -2){
            if (x < t->left->value)
                t = SingleRotateLeft(t);
            else //not equal
                t = DoubleRotateLeft(t);
        }
    }
    else
    if (x > t->value)
    {
        t->right = Insert(x, t->right);
        if (Height(t->left) - Height(t->right) == 2 
			|| Height(t->left) - Height(t->right) == -2){
            if (x > t->right->value)
                t = SingleRotateRight(t);
            else //not equal
                t = DoubleRotateRight(t);
        }
    }

    t->height = 
        Max(Height(t->left), Height(t->right)) + 1;
    return t;
}

/*the func can be called only if t has a left child*/
/*perform a retate between a node (k1) and its left child*/
/*update heights, then return new root*/
static POSITION_ 
SingleRotateLeft(POSITION_ k2)
{
    POSITION_ k1;//k1's past child
    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;

    k2->height = Max(Height(k2->left), Height(k2->right)) + 1;
    k1->height = Max(Height(k1->left), k2->height) + 1;
    
    return k1;//new root
}

static POSITION_ 
SingleRotateRight(POSITION_ k2)
{
    POSITION_ k1;//k1's past child
    k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;

    k2->height = Max(Height(k2->left), Height(k2->right)) + 1;
    k1->height = Max(Height(k1->right), k2->height) + 1;
    
    return k1;//new root
}

/*The func can be called only if k3 has a left-
    child(k1) and k3's left child has a right child(k2). */
/*Do the left-right double rotation. */
/*Update heights , then return new root. */
static POSITION_
DoubleRotateLeft(POSITION_ k3)
{
    /*rotate between k1 and k2*/
    k3->left = 
        SingleRotateRight(k3->left);
    
    /*rotate between k3 and k2*/
    return SingleRotateLeft(k3);
}

/*Only if k3 has a right child(k1)
And k1 has a right child(k2)*/
/*left-right double bottom-up rotation*/
static POSITION_
DoubleRotateRight(POSITION_ k3)
{
    /*rotate between k1 and k2*/
    k3->right = 
        SingleRotateLeft(k3->right);
    
    /*rotate between k3 and k2*/
    return SingleRotateRight(k3);
}
```