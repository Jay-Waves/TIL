```ad-info
也叫: 求中位数~
```
## 分析
### 介绍
- 由**Blum、Floyd、Pratt、Rivest、Tarjan**提出。
- 该算法的思想是修改快速选择算法的主元选取方法，提高算法在最坏情况下的时间复杂度。
- **递归**也是核心思想之一
### 优点
- 比简单分治思想更加高级, 即**数据分片**的思想, 可以快速抛弃掉无用数据(因为每次都将每部分的中位数放到开头来递归), 节省大量空间, 甚至可以不同机器和处理器处理后进行数据合并.(仅针对中位数, 不针对topK)
### 思路
>摘自[维基百科](https://en.wikipedia.org/wiki/Median_of_medians)

####  Algorithm
As stated before, median-of-medians is used as a pivot selection strategy in the [quickselect](https://en.wikipedia.org/wiki/Quickselect "Quickselect") algorithm, which in [pseudocode](https://en.wikipedia.org/wiki/Pseudocode "Pseudocode") looks as follows. Be careful to handle `left`, `right` and `n` when implementing. The following pseudocode assumes that `left`, `right`, and the list use one-based numbering and that `select` is initially called with 1 as the argument to `left` and the length of the list as the argument to `right`. Note that this returns the index of the n'th smallest number after rearranging the list, rather than the actual value of the n'th smallest number.

```rust
function select(list, left, right, n)//n即topK的k
    loop
        if left = right then
            return left
        pivotIndex := pivot(list, left, right)//这以上是select和pivot互递归
		//下面是寻找topK的单递归
        pivotIndex := partition(list, left, right, pivotIndex, n)//更新索引, 以中位数划片
        if n = pivotIndex then
            return n
        else if n < pivotIndex then
            right := pivotIndex - 1
        else
            left := pivotIndex + 1

```

Subroutine .mw-parser-output .monospaced{font-family:monospace,monospace}pivot is the actual median-of-medians algorithm. It divides its input (a list of length n) into groups of at most five elements, computes the median of each of those groups using some subroutine, then [recursively](https://en.wikipedia.org/wiki/Recursion_(computer_science) "Recursion (computer science)") computes the true median of the n 5 {\\displaystyle {\\frac {n}{5}}} ![{\displaystyle {\frac {n}{5}}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/dd29653a3a4c2054904439d7f180e17c525d5d00) medians found in the previous step:.[\[1\]](https://en.wikipedia.org/wiki/Median_of_medians#cite_note-clrs-1) Note that pivot calls select; this is an instance of **[mutual recursion](https://en.wikipedia.org/wiki/Mutual_recursion "Mutual recursion").**

```rust
function pivot(list, left, right)
    // for 5 or less elements just get median
    if right − left < 5 then
        return partition5(list, left, right)
    // otherwise move the medians of five-element subgroups to the first n/5 positions
    for i from left to right in steps of 5
        // get the median position of the i'th five-element subgroup
        subRight := i + 4
        if subRight > right then
            subRight := right
        median5 := partition5(list, i, subRight)
        swap list[median5] and list[left + floor((i − left)/5)]

    // compute the median of the n/5 medians-of-five
    mid := (right − left) / 10 + left + 1
    return select(list, left, left + floor((right − left) / 5), mid)

```

#### Partition helper functions
There is a subroutine called partition that can, in linear time, group a list (ranging from indices `left` to `right`) into three parts, those less than a certain element, those equal to it, and those greater than the element ([a three-way partition](https://en.wikipedia.org/wiki/Dutch_national_flag_problem "Dutch national flag problem")). The grouping into three parts ensures that the median-of-medians maintains linear execution time in a case of many or all coincident elements. Here is pseudocode that performs a partition about the element `list[pivotIndex]`:

```rust
function partition(list, left, right, pivotIndex, n)
    pivotValue := list[pivotIndex]
    swap list[pivotIndex] and list[right]  // Move pivot to end
    storeIndex := left
    // Move all elements smaller than the pivot to the left of the pivot
    for i from left to right − 1 do
        if list[i] < pivotValue then
            swap list[storeIndex] and list[i]
            increment storeIndex
    // Move all elements equal to the pivot right after
    // the smaller elements
    storeIndexEq = storeIndex
    for i from storeIndex to right − 1 do
        if list[i] = pivotValue then
            swap list[storeIndexEq] and list[i]
            increment storeIndexEq
    swap list[right] and list[storeIndexEq]  // Move pivot to its final place
    // Return location of pivot considering the desired location n
    if n < storeIndex then
        return storeIndex  // n is in the group of smaller elements
    if n ≤ storeIndexEq then
        return n  // n is in the group equal to pivot
    return storeIndexEq // n is in the group of larger elements

```

The partition5 subroutine selects the median of a group of at most five elements; an easy way to implement this is [insertion sort](https://en.wikipedia.org/wiki/Insertion_sort "Insertion sort"), as shown below.[\[1\]](https://en.wikipedia.org/wiki/Median_of_medians#cite_note-clrs-1) It can also be implemented as a [decision tree](https://en.wikipedia.org/wiki/Decision_tree "Decision tree").

```python
function partition5( list, left, right)
    i := left + 1
    while i ≤ right
        j := i
        while j > left and list[j−1] > list[j] do
            swap list[j−1] and list[j]
            j := j − 1
        i :=  i + 1
            
    return floor((left + right) / 2)
```
### 步骤

**算法步骤:**

1.  将n个元素每5个一组，分成n/5(上界)组，我到现在还在想为什么是5个一组，三个1组行不行，10个一组行不行...
2.  每组排序，找出中位数，5个一组的话，下标就是\[2\]，最后一组不用给中位数
3.  递归的调用，直到最后只有一个中位数，设为轴心，pivot，偶数个中位数的情况下设定为选取中间小的一个。在递归的过程中，最后肯定是构成的中位数<=5,最终返回的就是这个里面的中位数。
4.  用pivot来分割数组，left=\[<=pivot\],\[pivot\],right=\[>pivot\]。
5.  若len(left)+1==k，说明pivot就是要找的第k小的数，返回pivot即可；  
    若len(left)+1<k，在right中递归整个过程，topK(arr,k-len(left)+1)  
    若len(left)+1>k，在left中递归整个过程,topK(arr,k)；
6.  再严谨点的话可以加上异常处理
 
### 时间复杂度
最坏上界 O(N)
## 实现
- 找中位数:
	1. 仅实现寻找(近似的）中位数, 没有做topK
	2. 这里没有用到互递归(**Mutual Recursion**), 仅仅用到分治递归思想(**Divide and Conquer**)
```c
int bfprt(int *Data, int Len)
{
    const int Size = 5;
    int i;
    if (Len <= Size){//递归结束条件
        insertSort(Data, Len, 1);
        return *(Data + Len/2);
    }
    for (i = 0; i < Len-5; i += Size){
        insertSort(Data+i, Size, 1);
        *(Data+i/Size) = *(Data+i+Size/2+1);
    }
    insertSort(Data+i-Size, Len-i+Size+1, 1);
    *(Data+i/Size) = *(Data+i-Size/2);
    return bfprt(Data, Len/5+1);
}

int * insertSort(int *Data, int Len, int Mode)
{
    int i, j, Tmp = 0;
    if (Mode){
        for (i=1; i<Len; i++){
            for (j=0; j<i; j++){
                if (*(Data+j) > *(Data+i))
                {
                    Tmp = *(Data+i);
                    *(Data+i) = *(Data+j);
                    *(Data+j) = Tmp;
                }
            }
        }
    }
    else if(!Mode){
        for (i=1; i<Len; i++){
            for (j=0; j<i; j++){
                if (*(Data+j) < *(Data+i)){
                    Tmp = *(Data+i);
                    *(Data+i) = *(Data+j);
                    *(Data+j) = Tmp;
                }
            }
        }      
    }
    else
        printf("please input correct mode1");
}
```

### Notice
Median of Medians is just an approximate algorithm, it can not select the real(exact) median of numbers, but can select a well (approx) median with good porperty (such as become a pivot in other exact selection like QuickSort). 
Where the "approximate" means that there will be half of numbers on each side of it on average.
其实简单分析就能知道，起码有25%会在结果的左边，（25%是忽略掉各次选出的中位数情况，如果加上每组选出的中位数影响，应该是30%）
> 详见[[topK分析]]

> 以下是StackOverflow上大神的说法
> SO analisys:
> As you said, _"Select" does not find the true median rather a element that is median-enough for the purpose of choosing a pivot for quicksort._ In particular it is median enough that it is guaranteed to drop at least 30% of the dataset on every iteration. Unfortunately it is also an expensive operation.

> The key idea is that the median of medians is less than or equal to 3 out of every 5 elements whose median is less than or equal to it. So it is less than or equal to 3 out of every 5 elements for half the groups of 5, so at least 30% of the set is less than or equal to it. So it is in the largest 70% of the data set.

> Similarly it is in the smallest 70% of the data set.

> This guarantees that you avoid the potential pitfall of quickselect, which is picking pivot points that have extreme values.

> If you wish to combine efficiency and a good worst case you can combine this with quickselect. For instance 4 rounds of quickselect followed by one round of this followed by 4 rounds of quickselect, etc. The expensive rounds of BFPRT guarantee `O(n)` while the quickselect on average is going to be fast. By putting off your first round of BFPRT until you've done several rounds of quickselect you can make the extra running time only a few percent more than quickselect on average. (The worst case cost goes up by quite a bit, but we don't expect to encounter that.)