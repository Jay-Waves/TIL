**堆排序**（英语：Heapsort）
是指利用[堆](https://zh.m.wikipedia.org/wiki/%E5%A0%86_(%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84) "堆 (数据结构)")这种数据结构所设计的一种[排序算法](https://zh.m.wikipedia.org/wiki/%E6%8E%92%E5%BA%8F%E7%AE%97%E6%B3%95 "排序算法")。堆是一个近似[完全二叉树](https://zh.m.wikipedia.org/wiki/%E5%AE%8C%E5%85%A8%E4%BA%8C%E5%8F%89%E6%A0%91 "完全二叉树")的结构，并同时满足**堆积的性质**：即子节点的键值或索引总是小于（或者大于）它的父节点。

| 堆排序 |
| --- |
| [![Sorting heapsort anim.gif](https://upload.wikimedia.org/wikipedia/commons/1/1b/Sorting_heapsort_anim.gif)](https://zh.m.wikipedia.org/wiki/File:Sorting_heapsort_anim.gif)
堆排序算法的演示。首先，将元素进行重排，以符合堆的条件。图中排序过程之前简单地绘出了堆树的结构。

 
## 概述

若以升序排序说明，把[阵列](https://zh.m.wikipedia.org/wiki/%E9%99%A3%E5%88%97 "阵列")转换成[最大堆积](https://zh.m.wikipedia.org/wiki/%E6%9C%80%E5%A4%A7%E2%80%94%E6%9C%80%E5%B0%8F%E5%A0%86 "最大—最小堆")（Max-Heap Heap），这是一种满足最大堆积性质（Max-Heap Property）的[二元树](https://zh.m.wikipedia.org/wiki/%E4%BA%8C%E5%85%83%E6%A8%B9 "二元树")：对于除了根之外的每个节点i, A\[parent(i)\] ≥ A\[i\]。

重复从最大堆积取出数值最大的结点（把根结点和最后一个结点交换，把交换后的最后一个结点移出堆），并让残馀的[堆积](https://zh.m.wikipedia.org/wiki/%E5%A0%86%E7%A9%8D "堆积")维持最大堆积性质。

## 堆节点的访问
通常堆是通过一维[数组](https://zh.m.wikipedia.org/wiki/%E6%95%B0%E7%BB%84 "数组")来实现的。在阵列起始位置为0的情形中：
通常堆是通过一维[数组](https://zh.m.wikipedia.org/wiki/%E6%95%B0%E7%BB%84 "数组")来实现的。在阵列起始位置为0的情形中：

-   父节点i的左子节点在位置 ( 2 i + 1 ) {\\displaystyle (2i+1)}

-   父节点i的右子节点在位置 ( 2 i + 2 ) {\\displaystyle (2i+2)}

-   子节点i的父节点在位置 ⌊ ( i − 1 ) / 2 ⌋ {\\displaystyle \\lfloor (i-1)/2\\rfloor }

## 堆的操作
在堆的[资料结构](https://zh.m.wikipedia.org/wiki/%E8%B3%87%E6%96%99%E7%B5%90%E6%A7%8B "资料结构")中，堆中的最大值总是位于根节点（在优先队列中使用堆的话堆中的最小值位于根节点）。堆中定义以下几种操作：

-   最大堆调整（Max Heapify）：将堆的末端子节点作调整，使得子节点永远小于父节点
-   建立最大堆（Build Max Heap）：将堆中的所有数据重新排序
-   堆排序（HeapSort）：移除位在第一个数据的根节点，并做最大堆调整的[递回](https://zh.m.wikipedia.org/wiki/%E9%81%9E%E8%BF%B4 "递回")运算

## 实作范例

### C语言
```c
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}

void max_heapify(int arr[], int start, int end) {
	//这是默认了两个子堆已经是大顶堆, 只有堆首不对的情况
    // 建立父節點指標和子節點指標
    int dad = start;
    int son = dad * 2 + 1;
    while (son <= end) { // 若子節點指標在範圍內才做比較
        if (son + 1 <= end && arr[son] < arr[son + 1]) // 先比較兩個子節點大小，選擇最大的
            son++;
        if (arr[dad] > arr[son]) //如果父節點大於子節點代表調整完畢，直接跳出函數
            return;
        else { // 否則交換父子內容再繼續子節點和孫節點比较
            swap(&arr[dad], &arr[son]);
            dad = son;
            son = dad * 2 + 1;
        }
    }
}

void heap_sort(int arr[], int len) {
    int i;
    // 初始化，i從最後一個父節點開始調整
    for (i = len / 2 - 1; i >= 0; i--)
        max_heapify(arr, i, len - 1);
    // 先將第一個元素和已排好元素前一位做交換，再重新調整，直到排序完畢
    for (i = len - 1; i >=1; i--) {
        swap(&arr[0], &arr[i]);
        max_heapify(arr, 0, i - 1);
    }
}

int main() {
    int arr[] = { 3, 5, 3, 0, 8, 6, 1, 5, 8, 6, 2, 4, 9, 4, 7, 0, 1, 8, 9, 7, 3, 1, 2, 5, 9, 7, 4, 0, 2, 6 };
    int len = (int) sizeof(arr) / sizeof(*arr);
    heap_sort(arr, len);
    int i;
    for (i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
    return 0;
}

```

### Python
```python
#!/usr/bin/env python
#-*-coding:utf-8-*-


def heap_sort(lst):
    def sift_down(start, end):
        """最大堆调整"""
        root = start
        while True:
            child = 2 * root + 1
            if child > end:
                break
            if child + 1 <= end and lst[child] < lst[child + 1]:
                child += 1
            if lst[root] < lst[child]:
                lst[root], lst[child] = lst[child], lst[root]
                root = child
            else:
                break

# 创建最大堆

    for start in xrange((len(lst) - 2) // 2, -1, -1):
        sift_down(start, len(lst) - 1)

# 堆排序
    for end in xrange(len(lst) - 1, 0, -1):
        lst[0], lst[end] = lst[end], lst[0]
        sift_down(0, end - 1)
    return lst



if __name__ == "__main__":
    l = [9, 2, 1, 7, 6, 8, 5, 3, 4]
    heap_sort(l)

```


### 原地堆排序

基于以上[堆](https://zh.m.wikipedia.org/wiki/%E5%A0%86_(%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84) "堆 (数据结构)")相关的操作，我们可以很容易的定义堆排序。例如，假设我们已经读入一系列数据并创建了一个堆，一个最直观的算法就是反复的调用`del_max()`函数，因为该函数总是能够返回堆中最大的值，然后把它从堆中删除，从而对这一系列返回值的输出就得到了该序列的降序排列。真正的原地堆排序使用了另外一个小技巧。堆排序的过程是：

1.  建立一个堆 H[0..n-1]
2.  把堆首（最大值）和堆尾互换
3.  把堆的尺寸缩小1，并调用`shift_down(0)`,目的是把新的数组顶端数据调整到相应位置
4.  重复步骤2，直到堆的尺寸为1

## 平均复杂度

堆排序的平均时间复杂度为{\displaystyle \mathrm {O} (n\log n)})，空间复杂度为![\Theta(1)