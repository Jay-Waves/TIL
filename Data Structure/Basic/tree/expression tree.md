### 输入后缀表达式， 转表达式树、
- 表达式树中序遍历并赋予括号的例程值得一看
```c
/*notice: 1.输入后缀表达式转表达式树；2.表达式树计算；3.中缀表达式计算*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<windows.h>


struct T_NODE;
typedef struct T_NODE *TREE_;
struct T_NODE {
    int opOrNum;// op or num
    int type;//num->1, op->0, 这是因为公用一个int时，数字42可能会被误解为+，类似
    TREE_ left;
    TREE_ right;
};
int Prio( int op ) {
    if ( op == '+'||op=='-' ) {
        return 0;
    }
    else if(op=='*'||op=='/') {
        return 1;
    }
    else {
        return 10;//digit's prio is max
    }
}
int IsOp( TREE_ t ) {
    if ( !t->type )
        return 1;
    else
        return 0;
}

void InOrder( TREE_ t ) {
    if ( t != NULL ) {
        if ( t->left!=NULL &&Prio( t->opOrNum ) > Prio( t->left->opOrNum ) ) {
            printf( "(" );
            InOrder( t->left );
            printf( ")" );
        }
        else {
            InOrder( t->left );
        }
		
        if (IsOp(t)) {
            printf( "%c", t->opOrNum );
        }
        else {
            printf( "%d", t->opOrNum );//注意区分int和（int）char
        }
		
        if ( t->right !=NULL&&Prio( t->opOrNum ) > Prio( t->right->opOrNum ) ) {
            printf( "(" );
            InOrder( t->right );
            printf( ")" );
        }
        else {
            InOrder( t->right );
        }
    }
}

#define STK_SIZE 100
#ifndef _STACK_H_
#define _STACK_H_

struct S_NODE {
    double value;//每个子树的计算结果
    TREE_ t;
}stack[STK_SIZE];
int sTop = -1;
struct S_NODE StackPop( ) {
    if ( sTop == -1 ) {
        printf( "error! out of space with index -1" );
    }
    return stack[sTop--];
}
void StackPush( TREE_ t, double value ) {
    if ( sTop == STK_SIZE ) {
        printf( "error! out of space with overflow" );
    }
    stack[++sTop].t = t;
    stack[sTop].value = value;
    return;
}

#endif  //_STACK_H_

double Compute( double left, double right, int op ) {
    if ( op == '+' ) {
        return left + right;
    }
    else if ( op == '-' ) {
        return left - right;
    }
    else if ( op == '*' ) {
        return left * right;
    }
    else {
        //div
        return left / right;
    }
}

char* ReadIn(char *buffer , char *source) {
    int i=0;
    while (*(source+i)!=0 &&*( source + i ) != ' ' && *( source + i ) != '\n' ) {
        buffer[i] = *( source + i );
        ++i;
    }
    buffer[i] = 0;
    if ( *( source + i ) == 0 )
        return NULL;
    else
    return source + i + 1;
}

int main( ) {
    TREE_ t;
    struct S_NODE s_right, s_left;
    char str[100], tmp[10], *p=str;
    int num, mod;
    fgets( str, 100, stdin );
    while ((p= ReadIn(tmp, p))!=NULL ) {
        if ( isdigit( tmp[0] ) ) {
            sscanf( tmp, "%d", &num );
            t = ( TREE_ )malloc( sizeof( struct T_NODE ) );
            t->left = NULL, t->right = NULL;
            t->opOrNum = num, t->type = 1;
            StackPush( t, num );
        }
        else {
            t = ( TREE_ )malloc( sizeof( struct T_NODE ) );
            t->opOrNum = tmp[0];
            s_right = StackPop( ), t->right = s_right.t;
            s_left = StackPop( ), t->left = s_left.t;
            t->type = 0;//op
            StackPush( t, Compute( s_left.value, s_right.value, t->opOrNum ) );
        }
    }
	InOrder( stack[0].t );
	printf( "\n%.2lf", stack[0].value );
    return 0;
}
```