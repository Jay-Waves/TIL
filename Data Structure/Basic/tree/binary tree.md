# 概述
## 注意
1. 如果删除的元素, 有很大可能还会再次建立和访问, 那么使用懒惰删除是非常棒的方案
# 实现
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
