# 简陋实现
- 指非正版堆排序结构, 而是简单数组模拟
==留意其中的交换结构, 利用辅助变量, 隐藏了明显的交换操作==
## 基础版
```c
//heap: 0(small)->size-1(big)
//unstable version
#define ELEMENT_TYPE int
void UpdateHeap( ELEMENT_TYPE *heap, const int heapSize, ELEMENT_TYPE x ) {
    int i;
    if ( *( heap ) < x ) {
        //利用结构, 隐藏明显的数值交换
        *( heap ) = x;
        for ( i = 1; i < heapSize && x>*( heap + i ); i++ ) {
            *( heap + i-1 ) = *( heap + i );
        }
        *( heap + --i) = x;
    }
    return;
}
```
## 稳定排序版
```ad-note
注意小数据量时，堆排序选取topk表现并不优异。
- 若用此法进行正常排序，就退化变成普通插入排序了；
- 此法核心便是不用的数据丢弃，若所有数据都要，就无优势。
```
### 排序空间=数据域的情况：
```ad-danger
注意开辟的heap空间存的就是数据本身
```
```c
ELEMENT_TYPE *CreateHeap( const int heapSize ) {
    //根据需要, 可以改成索引表(二级指针数组)
    ELEMENT_TYPE *p = ( ELEMENT_TYPE * )malloc( sizeof( ELEMENT_TYPE ) * heapSize );
    memset( p, 0, sizeof( ELEMENT_TYPE* ) * heapSize );
    return p;
}
//stable version
void UpdateStableHeap( ELEMENT_TYPE *heap, const int heapSize, ELEMENT_TYPE x , int (*Cmp)(ELEMENT_TYPE*fst, ELEMENT_TYPE*sec)) {
    int i;
    if ( Cmp(&x, heap) ) {//第二条件用于适应堆尚空情况
        //利用结构, 隐藏明显的数值交换
        *( heap ) = x;
        for ( i = 1; i < heapSize && Cmp(&x, heap+i); i++ ) {
            *( heap + i - 1 ) = *( heap + i );
        }
        *( heap + --i ) = x;
    }
    return;
}

int Cmp( ELEMENT_TYPE *a, ELEMENT_TYPE *b ) {
    //仍是不稳定版本, 可修改
    if ( *a > *b ) {
        return 1;
    }
    else {
        return 0;
    } 
}
```
### 排序空间非数据域，而是索引表的情况
```ad-danger
排序空间是索引表，可以保留原空间的物理顺序，但更复杂
以下仅贴出大作业（网页查找）中一种方法，未做适应性改进
```
```c
//topk
WEB_ *CreateHeap( const int heapSize ) {
    WEB_ *p = ( WEB_ * )malloc( sizeof( WEB_ ) * heapSize );
    memset( p, 0, sizeof( WEB_ ) * heapSize );
    return p; 
}
void UpdateStableHeap( WEB_ *heap, const int heapSize,
    WEB_ x, int ( *Cmp )( WEB_ fst, WEB_ sec ) ) {
    int i;
    if ( *heap == NULL || Cmp( x, *heap ) ) {
		//注意索引表就需要注意NULL指针不能传入了（为了保证cmp函数简洁性，没有对null的访问进行限制）
        *( heap ) = x;
        for ( i = 1; i < heapSize && ( *( heap + i ) == NULL || Cmp( x, *( heap + i ) ) ); i++ ) {
            *( heap + i - 1 ) = *( heap + i );
        }
        *( heap + --i ) = x;
    }
    return;
}
int Cmp( WEB_ a, WEB_ b ) {
    double gap = ( ( WEB_ )a )->doc_sim - ( ( WEB_ )b )->doc_sim;
    if ( gap < 0 && fabs( gap )>EPSILON )
        return 0;
    else if ( gap > 0 && fabs( gap ) > EPSILON )
        return 1;
    else {
        gap = ( ( WEB_ )a )->no_num - ( ( WEB_ )b )->no_num;
        if ( gap > 0 )
            return 0;
        else
            return 1;
    }
}
```