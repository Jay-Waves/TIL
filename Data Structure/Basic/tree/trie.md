## 分析:
核心是一位一位进行查找, 以路径为前缀, 以前缀为基础向下查询
>请参考![[../../../Algorithm/查找问题/算法时间复杂度分析]]

## 实现:
### 一般链表:
- 有两种方式
1. 每次开辟仅开辟一个节点, 好处是节省空间(但太过庞大时, 动态申请空间, 有过多无效信息空间), 费时, **适合小数据量**
2. 每次开辟都开辟同层的26个空间(26个连续的node), 好处是节省时间, 坏处是空间浪费大
> 两者不难相互转化
```c
/*每次开辟单节点, 例*/
#define MAX_SIZE 26//字典宽度
enum BOOL{true, false};
 
struct TRIE;
typedef struct TRIE *TREE_;
typedef struct TRIE *POSITION_;
struct TRIE{
    //int count;
    enum BOOL isEnd;
    POSITION_ p[MAX_SIZE];//0代表a, 和二维数组实现不同
    //char str[50]; //*如果需要频繁读取整个单词, 可以在末端直接储存
};

POSITION_ 
CreateNode(){
    TREE_ t;
    t = (TREE_)malloc(sizeof(struct TRIE));
        memset(t, 0, sizeof(struct TRIE));
    return t;
}
  
//需要头节点, 储存初始26个字母
TREE_ 
Init(){
    return CreateNode();
}

/*need empty head!
not recursive, always start from head*/
void 
Insert(char * str, TREE_ root){
    POSITION_ pos = root;
    int len = strlen(str), index, i;
    for (i=0; i<len; i++){
        index = str[i] - 'a';
        if (pos->p[index] == NULL)
            pos->p[index] = CreateNode();
        pos = pos->p[index];
        //pos->count++;
    }
    pos->isEnd = true;//或存储整个单词
    return ;
}
  
enum BOOL 
Find(char * str, TREE_ root){
    POSITION_ pos = root;
    int len = strlen(str), i, index;
    for (i=0; i<len; i++){
        index = str[i] - 'a';
        if (pos->p[index] == NULL)
            return false;
        pos = pos->p[index];
    }
    if (pos->isEnd)//要注意存在前缀,但单词不存在的情况
        return true;
    else
        return false;
}
```

### 静态:
- 又称"二维数组实现"
#### 分析:
- 优点: 
	1. 比链表字典树访问更快, 因为避免了不断申请和释放空间的时间
	2. 在数据量极大的情况下, 所占空间更小利用率更大, 因为不断动态申请细小空间时,malloc的前缀信息变得臃肿
		- 当然, 这一点可以用同子代(26个)一起申请空间来解决
	3. 方便利用qsort()函数
- 缺点: 
	需要提前开辟较大(通常是非常大)空间, 容易出现数组越界(overflow), 并且无法更改
	不够直观, 理解困难, **调试困难**
#### 原理:
- 将字母转化为数字int(映射到0-26)处理, 一是方便直接作为角标, 二是同时存储编号需要较大空间
1. 用二维数组tree[i]\[j]来标识一颗字典树，其中i为父节点，j为子节点.
2. 程序开始时父节点为parent节点。tree[i]\[j]表示节点i第j个儿子的编号
3. 规定最上层根节点parent = 0;

![[../../../attach/1922008-20200113195151095-70348301.jpg|400]]

![[../../../attach/1922008-20200113195157015-783649434.jpg|400]]
#### 源码:
```c
#define MAX_SIZE 10000000//最大10e7, 
int tree[MAX_SIZE][30];//tree[i][j]表示节点i的第j个儿子的节点编号
  
enum BOOL{false, true};
enum BOOL flag[MAX_SIZE];//表示该节点结尾是一个单词
int total; //总节点数, 同时也用于给树节点编号

/*input word, and init or insert tree*/
void Insert(char *str)
{
    int len = strlen(str);
    int i, id;
    int parent = 0;//总父节点统一置为0
    if (total+1 == MAX_SIZE){
        printf("error, out of memory with larger data!");
        return ;
    }
    for (i=0; i<len; i++){//这里可能出现问题: 如果用fgets读入, 可能无法处理\n
        //a->1...
        id = str[i] - 'a' + 1;//?str[i]-'0'
        if (!tree[parent][id])//尚不存在
            tree[parent][id] = ++total;
        parent = tree[parent][id];//游标下沉, 实际指向物理已用空间的下一个地址
    }
    flag[parent] = true;
}

enum BOOL Find(char *str)
{
    int len = strlen(str);
    int parent = 0, i, id;
    for (i=0; i<len; i++){
        id = str[i]-'a'+1;//?str[i]-'0'
        if (!tree[parent][id])
            return false;
        parent = tree[parent][id];//游标下沉
    }
	if (flag[parent]!=0)
    	return true;
	else
		return false;
}

/*清空树*/
void init(){
    int i, j;
    for (i=0; i<total; i++){
        flag[i] = false;
        for (j=1; j<27; j++)//?j<10
            tree[i][j] = 0;
    }
    total = 0;
}
这里提供一个造树范例: 很简单
/*
void CreateStopTrie( ) {
    FILE *fp_in_stop;
    char word[WORD_SIZE];
    fp_in_stop = fopen( "F:\\notes\\assign\\bonus\\reference\\dictionary.txt", "r" );
    while ( fscanf( fp_in_stop, "%s", word ) != EOF )
        Insert( stop_trie, flag_stop_trie, total_stop_trie, word );
    fclose( fp_in_stop );
}*/
```