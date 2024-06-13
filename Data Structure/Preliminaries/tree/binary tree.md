![|400](../../../attach/Pasted%20image%2020240527115840.png)

# Binary Search Tree

1. 节点至多有两个分叉, 称为左子树和右子树.
1. 左子树节点值总小于当前节点值
2. 右子树节点值总大于当前节点值

**节点深度**: 从根节点走到当前节点所经历的节点数.

**树高度**: 最深的节点深度.

**平衡二叉树**: 叶子节点的深度差至多为一. 平衡树有多种实现:
- [red-black tree](red-black%20tree.md)
- [avl tree](avl%20tree.md)
- [b tree](b%20tree.md)

**完全二叉树**: 所有叶节点的深度相同, 内部节点的出度 (子节点数) 相同.

## 声明
```c
#define ELEMENTTYPE int
#ifndef _TREE_H
#define _TREE_H
    struct NODE;
    typedef struct NODE *POSITION_;
    typedef struct NODE *TREE_;

    TREE_ MakeEmpty(TREE_ T);
    POSITION_ Find(ELEMENTTYPE, TREE_ T);
    POSITION_ FindMin(TREE_ T);
    POSITION_ FindMax(TREE_ T);
    TREE_ Insert(ELEMENTTYPE, TREE_ T); //kernel
    TREE_ Delete(ELEMENTTYPE, TREE_ T);//kernel
    ELEMENTTYPE Retrieve(POSITION_ P);

    struct NODE{
        ELEMENTTYPE value;
        TREE_ left;
        TREE_ right;
		//BOOL deleted
    };
#endif
```

## 核心例程
```c
TREE_ MakeEmpty(TREE_ T){
    if (T != NULL)
    {
        MakeEmpty(T->left);
        MakeEmpty(T->right);
        free(T);
    }
    return NULL;
}

POSITION_ Find(ELEMENTTYPE x, TREE_ t)
{
    if (t == NULL)//empty tree!
        return NULL;
    else if (t->value < x)
        return Find(x, t->right);
    else if (t->value > x)
        return Find(x, t->left);
    else
        return t;
}

POSITION_ FindMin(TREE_ t)
{
    if (t == NULL)  
        return NULL;//error
    else if (t->left == NULL) 
        return t;
    else  
        return FindMin(t->left);
}

POSITION_ FindMax(TREE_ t)
{
    if (t == NULL)
        return NULL;
    else if (t->left == NULL)
        return t;
    else
        return FindMax(t->right);
}

/*insert element, but not dispose repeated item*/
TREE_ Insert(ELEMENTTYPE x, TREE_ t)
{
    if (t == NULL)
    {
        /*create and return a one-node tree*/
        t= (TREE_)malloc(sizeof(struct NODE));
        if (t == NULL)
            printf("error , unable to malloc a memory");
        else
        {
            t->value = x;
            t->left = t->right = NULL;
        }
    }
    else
    if (x < t->value)
        t->left = Insert(x, t->left);
    else
    if (x > t->value)
        t->right = Insert(x, t->right);
    /*else x is in the tree already; we'll do nothing*/
    return t;
}

//used in Delete to implement alternate substitution
int flag = 0;
/*find the least element in the right_tree( or the biggest in left) 
to replace the deleted item.
when possibly use the deleted item again, use the lazy delete*/
TREE_ Delete(ELEMENTTYPE x, TREE_ t)
{
    POSITION_ tmp;
    if (t == NULL)
        return NULL;//not found
    else
    if (x < t->value)
        t->left = Delete(x, t->left);
    else
    if (x > t->value)
        t->right = Delete(x, t->right);
    else
    if (t->left && t->right)//&&x==t->value; with two sons
    {
        /*replace in balance*/
        flag %= 2;
        if (flag)
        {
            tmp= FindMin(t->right);
            t->right= Delete(tmp->value, t->right);
        }
        else
        {
            tmp= FindMax(t->left);
            t->left= Delete(tmp->value, t->left); 
        }
        t->value = tmp->value;
    }
    else //one or zero son
    {
        tmp = t;
        if (t->left == NULL)
            t= t->right;
        else
        if (t->right == NULL)
            t= t->left;
        free(tmp);
    }
    return t;//return the new son
}
```

## 遍历
```c
int Height(TREE_ t)
{
    int lh, rh;
    if (t == NULL)
        return -1;
    else
    {
        lh = Height(t->left);
        rh = Height(t->right);
        return 1 + (rh>lh?rh:lh);
    }
}

//树的遍历
void LevelOrder(TREE_ t)
{
    int h = Height(t), i, j, lvSize = 1;
    POSITION_ *current = &t;
    POSITION_ *p;
    for (i=0; i<h; i++)
    {
        for (j=0; j<lvSize; j++)
        {
            //*(current+j)->
            //do something in curr lv
        }
        lvSize *= 2;//binary tree only, for next level
        p = (POSITION_ *)malloc(lvSize * sizeof(POSITION_));
        for (j=0; j<lvSize; j++)
        {
            if (j%2 == 0)
            {
                if ((*current+j/2)->left != NULL)
                    *(p+j) = (*current+j/2)->left;
                else
                    memset(j+p, 0, sizeof(POSITION_));
            }
            else
            {
                if ((*current+j/2)->right != NULL)
                    *(p+j) = (*current+j/2)->right;
                else
                    memset(j+p, 0, sizeof(POSITION_));
            }
        }
        free(current);
        current = p;
    }
    return;
}

void PreOrder(TREE_ t)
{
    if (t!=NULL)
    {
        //current node, do sth
        PreOrder(t->left);
        PreOrder(t->right);
    }
}

void PostOrder(TREE_ t)
{
    if (t!=NULL)
    {
        PostOrder(t->left);
        PostOrder(t->right);
        //current node, do sth
    }
}

void InOrder(TREE_ t)
{
    if (t!=NULL)
    {
        InOrder(t->left);
        //current node, do sth
        InOrder(t->right);
    }
}
```

***

## 简洁例程
```c
#define ELEMENTTYPE int
#ifndef _TREE_H
#define _TREE_H
    struct NODE;
    typedef struct NODE *POSITION_;
    typedef struct NODE *TREE_;

    POSITION_ Find(ELEMENTTYPE, TREE_ T);
    TREE_ Insert(ELEMENTTYPE, TREE_ T);

    struct NODE{
        ELEMENTTYPE value;
        TREE_ left;
        TREE_ right;
    };
#endif


POSITION_ Find(ELEMENTTYPE x, TREE_ t)
{
    if (t == NULL)//empty tree!
        return NULL;
    else if (t->value < x)
        return Find(x, t->left);
    else if (t->value > x)
        return Find(x, t->right);
    else
        return t;
}


/*insert element, but not dispose repeated item*/
TREE_ Insert(ELEMENTTYPE x, TREE_ t)
{
    if (t == NULL)
    {
        /*create and return a one-node tree*/
        t= (TREE_)malloc(sizeof(struct NODE));
        if (t == NULL)
            printf("error , unable to malloc a memory");
        else
        {
            t->value = x;
            t->left = t->right = NULL;
        }
    }
    else
    if (x < t->value)
        t->left = Insert(x, t->left);
    else
    if (x > t->value)
        t->right = Insert(x, t->right);
    /*else x is in the tree already; we'll do nothing*/
    return t;
}
```


```c

/* 二叉查找树：
    结点右侧全部节点元素都大于该结点, 结点左侧所有元素都小于该结点, 并且每个结点最多只能有两个分叉
    二叉查找树平均深度为 O( log N ) */

#include<stdio.h>
#include<stdlib.h>

typedef int ELEMENTTYPE;

#ifndef _Tree_H
#define _Tree_H

    struct TreeNode;
    typedef struct TreeNode * POSITION_;
    typedef struct TreeNode * SEARCHTREE_;

    SEARCHTREE_ makeEmpty( SEARCHTREE_ T );
    POSITION_ findElement( ELEMENTTYPE X, SEARCHTREE_ T );
    POSITION_ findMin( SEARCHTREE_ T );
    POSITION_ findMax( SEARCHTREE_ T );
    SEARCHTREE_ insertElement( ELEMENTTYPE X, SEARCHTREE_ T );
    SEARCHTREE_ deleteElement( ELEMENTTYPE X, SEARCHTREE_ T );
    ELEMENTTYPE retrieve( POSITION_ P);

#endif /*  _Tree_H */


#include"BinarySearchTreeArgument.h"

struct TreeNode
{
    ELEMENTTYPE Element;
    SEARCHTREE_ Left_ ;
    SEARCHTREE_ Right_ ;
};

/* 为了递归的一致性，选择初始化为null而不是单节点 */
SEARCHTREE_ 
makeEmpty( SEARCHTREE_ T){

    if( T != NULL )
    {
        makeEmpty( T->Left_ );
        makeEmpty( T->Right_ );
        free( T );
    }
    return NULL; 
}

POSITION_
findElement( ELEMENTTYPE X, SEARCHTREE_ T){
    if( T == NULL ){
        printf( "error find, empty tree " );
        return NULL;
    }
    else if( T->Element < X )
        findElement( X, T->Right_ );
    else if( T->Element > X)
        findElement( X, T->Left_ );
    else
        return T;
}

/* 最左侧的结点必定是最小的 
    递归实现 */
POSITION_
findMin( SEARCHTREE_ T){
    if( T == NULL ){
        printf( "error findMin, empty tree!");
        return NULL;
    }
    else if( T->Left_ == NULL )//基准情形
        return T;
    else   
        return FindMin( T->Left_ );
}

/* 非递归实现 findMax
注意T自身赋值或赋值操作没有问题， 但是不要改变T指向的内容比如给T->right赋值就是根本错误的 */
POSITION_
findMax( SEARCHTREE_ T){
    if( T != NULL )
        while( T->Right_ != NULL )
            T= T->Right_;
    return T;
}

/* 插入
对于重复元，本实现方法遇到重复元不会进行任何处理
但另一种方法是在树结点中增加重复信息，这样的好处是删除也更加方便了。 如果重复的关键字是更大结构的一部分，那么就可以将具有i选哪个如关键字的所有结构保留在一个辅助数据结构中 */
/* 插入原理：
不断向下寻找空位， 直至寻找到合适的位置 */
SEARCHTREE_
insertElement( ELEMENTTYPE X, SEARCHTREE_ T){
    if( T == NULL )
    {
        /* create and return a one-node tree */
        T= malloc( sizeof( struct TreeNode ) );
        if( T == NULL )
            printf(" error insert a new node, out of space \n");
        else
        {
            T->Element= X;
            T->Left_= T->Right_= NULL;
        }
    }
    else 
    if( X < T->Element )
        T->Left_= insertElement( X, T->Left_ );
    else
    if( X > T->Element )
        T->Right_= insertElement( X, T->Right_ );
    /* Else X is in the tree already; we'll do noting */

    return T;//注意T在例程中并没有被修改，认识该子树的根。返回树的根，是为了方便递归
}

/* 删除操作：（树最困难例程）
考虑两种情况：
    1. 要删除的结点只有一个分支。那么只需要调整该分支的父节点绕过该指针即可
    2. 要删除的节点有两个分支。 直接把这个节点释放了不大现实，因为他的子节点不一定能够替代它。什么能够完美替代它的位置呢？只有右子树的最小值（或左子树的最小值）
        因此， 需要先找到右子树的最小值进行替换，又因为右子树的最小值一定只有一个右节点，所以利用第一种情况删掉即可。
这里仍然使用递归的办法，因为是递归，所以一次只能处理树的一层。这样每个节点只需要关注他的两个子节点就可以，并且需要保证函数回调值也是子树。
 */
SEARCHTREE_
deleteElement( ELEMENTTYPE X, SEARCHTREE_ T ){
    POSITION_ TmpCell;
    if( T == NULL )
        printf( " Element not found, delete error " );
    else
    if( X < T->Element )
        T->Left_= deleteElement( X, T->Left_ );//postorder后序遍历， 浅层等待深层
    else
    if( X > T->Element )
        T->Right_= deleteElement( X, T->Right_ );
    else
    if( T->Left_ && T->Right_ )/* 有两个节点  */
    {
        /* replace with smallest in right subtree */
        TmpCell= findMin( T->Right_ );//记录最小值位置
        /* 在这颗右子树上因为有findmin和deleteelement操作，所以相当于进行了两遍重复的查找操作。这其实是可以合并的，这里是为了简洁性，  */
        T->Element= TmpCell->Element;
        T->Right_= deleteElement( T->Element, T->Right_ );//删除最小值， 并返回更新后的右子树
    }
    else /* One or zero children */
    {
        TmpCell= T; //记录T的值， 以便后续将T此时指向的无用节点释放掉。注意，不能先释放，否则就出现了NULL指针
        if( T->Left_ == NULL ) /* 同时处理了没有分支的情况 */
            T= T->Right_;
        else if( T->Right_ == NULL )
            T= T->Left_;
        free( TmpCell );
    }

    return T;
}
/* 删除操作中， 我们既可以选择右子树最小元素作为该结点的替代值，也可以选择左子树最大元素作为该结点的替代值。
但是这样有一个问题，就是当数据很大时，会导致树的不平衡，左边子树会明显更加庞大。 解决办法可以是随机交替使用上述两种方案。
当然，如果直接使用平衡树（ 如avl ）更好，因为可以顺便解决数据输入时按序输入的不平衡问题 */
/* 还有另一种删除思路， 就是不删除节点，只将出现等 频率数减一（做个记号）。 特别是有重复关键字时这种方案很好用
如果树中的实际简单树和“被删除”的节点数相同，那么树的深度也预计只上升一个小的阐述。 因此懒惰删除时间开销极小，且如果重新插入，就避免分配一个新单元的开销了。 */

```

## 如何支持区间查找?

![|500](../../../attach/Pasted%20image%2020240527120240.png)

## 树的数组实现


其中，**数组A下标从1到N**，N为堆的大小，A\[1\]是根节点，A\[2\]是根节点的左子孩节点，A\[3\]是根节点的右子孩节点。实际上，对于任何一个节点，若其在数组中的位置是i，则它的左子孩节点位置$left\_child(i)=2i$，右子孩节点位置$right\_child=2i+1$，它的父节点（假如有）的位置$parent(i)=\lfloor i/2 \rfloor$，$\lfloor \rfloor$表示向下取整。图3中的箭头从父节点分别指向左右子孩节点。???