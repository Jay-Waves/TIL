# 游标实现
## 方法
- 即数组实现, 游标指以数组下标作为参考的标志
> 详见: [哈夫曼树详解_csdn](https://blog.csdn.net/chenlong_cxy/article/details/117929139)
```c
#define WEIGHT_TYPE int
struct HUFFMAN_NODE;
typedef struct HUFFMAN_NODE *TREE_;
typedef struct HUFFMAN_NODE *POSITION_;
struct HUFFMAN_NODE{
    WEIGHT_TYPE weight;
    int parent;
    int left, right;
    //int info;//ELEMENT
};
/*永远以最小的两个权值进行树组合*/
//!游标形式, 非链形式

#define MAX_WGHT 1000
//0位置不储存任何值是因为需要0来标识未入树子节点
void Select(TREE_ t, int n, int *least, int *second)
{
    int min[2], i, j;
    min[0] = min[1] = 0;
    t[min[0]].weight = MAX_WGHT;
    for (i=1; i<=n; i++)//n代表当前非零值
    {
        if (t[i].weight < t[min[1]].weight && t[i].parent == 0)
        {
            if (t[i].weight < t[min[0]].weight)
                min[0] = i;
            else
                min[1] = i;
        }
    }
    *least = min[0];
    *second = min[1];
}

/*only need weight and its size, weight default in an array*/
/*just REWRITE it by yourself in need for other info*/
TREE_
Init(WEIGHT_TYPE *wght, int n)
{
    int total = n * 2 -1;//预期总节点数, n为实际数据数
    int i, s1, s2;//权值最小的s1和s2
    TREE_ t = (TREE_)malloc(sizeof(struct HUFFMAN_NODE)*(total + 1));
        //saze a size for empty [0]
    memset(t, 0, sizeof(struct HUFFMAN_NODE)*(total + 1));
    for (i=1; i<=n; i++)
        t[i].weight = wght[i-1];//为新空间赋初值
    for (i= n+1; i <= total; i++)
    {
        Select(t, i-1, &s1, &s2);//!出错

        t[i].weight = t[s1].weight + t[s2].weight;
        t[s1].parent = t[s2].parent = i;
        t[i].left = s1, t[i].right = s2;
    }
    return t;//数组头
}
```

## 应用
### 前缀码
>**前缀编码**是指对字符集进行**编码**时，要求字符集中任一字符的**编码**都不是其它字符的**编码**的**前缀**

### 生成哈夫曼树编码
>承前游标实现[[haffman tree#游标实现]]

```c
//指向各字符huffman_code的指针哈希表
typedef char ** HUFFMAN_CODE__;

HUFFMAN_CODE__
HuffCoding(TREE_ t, int n)
{
    int i, cursor, c, p;
    HUFFMAN_CODE__ hList = 
        (HUFFMAN_CODE__)malloc(sizeof(char *)*(n+1));
        //同样舍弃0空间 
    char * assit = (char *)malloc(sizeof(char *)*n);
        //辅助空间
        //最长编码长度为n-1, n处存放0标识结尾
    assit[n-1] = 0;
    for (i=1; i<=n; i++)
    {
        c = i;//正在进行第i个数据的编码
        cursor= n- 1;//最初游标指向\0, 后向前移动
        p = t[c].parent;
        while (p)//到根节点为止, 根节点父节点为0
        {
            if (t[p].left == c)
                assit[--cursor] = '0';
            else
                assit[--cursor] = '1';
            c = p;
            p = t[c].parent;//继续向上扫描
        }
        hList[i] = (char *)malloc(sizeof(char)*(n-cursor));
        strcpy(hList[i], &assit[cursor]);
    }
    free(assit);
    return hList;
}
```

# 链表树实现 (传统方法)
- 仅提供代码, 代码不难理解
```c
#define _CRT_SECURE_NO_WARNINGS
//文件压缩-Huffman, 链表树实现(传统方法)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>


#define MAXSIZE 32

struct tnode {					//Huffman树结构
	char c;		
	int weight;					//树节点权重，叶节点为字符和它的出现次数
	struct tnode *left,*right;
} ; 
int Ccount[128]={0};			//存放每个字符的出现次数，如Ccount[i]表示ASCII值为i的字符出现次数 
struct tnode *Root=NULL; 		//Huffman树的根节点
char HCode[128][MAXSIZE]={{0}}; //字符的Huffman编码，如HCode['a']为字符a的Huffman编码（字符串形式） 
FILE *Src, *Obj;
	
void statCount();				//步骤1：统计文件中字符频率
void createHTree();				//步骤2：创建一个Huffman树，根节点为Root 
void makeHCode();				//步骤3：根据Huffman树生成Huffman编码
void atoHZIP(); 				//步骤4：根据Huffman编码将指定ASCII码文本文件转换成Huffman码文件

int main()
{
	if((Src=fopen("input.txt","r"))==NULL) {
		fprintf(stderr, "%s open failed!\n", "input.txt");
		return 1;
	}
	if((Obj=fopen("output.txt","w"))==NULL) {
		fprintf(stderr, "%s open failed!\n", "output.txt");
		return 1;
	}
	
	statCount();
	createHTree();
	makeHCode();
	atoHZIP();

	fclose(Src);
	fclose(Obj);

    return 0;
} 

//【实验步骤1】开始 
void statCount()
{
	char str[500];
	int len, i;
	while (fgets(str, 500, Src) != NULL)
	{
		len = strlen(str);
		if (str[len-1]=='\n')
			str[len-1] = 0;
		for (i = 0; i < len; i++)
		{
			if (str[i]>0)
				Ccount[str[i]]++;
		}
	}
	Ccount[0]= 1;
}
//【实验步骤1】结束

//【实验步骤2】开始
int Cmp(const void *A, const void *B)//局部函数--快排用比较函数
{	
	struct tnode a = * *(struct tnode **)A, 
		b = * *(struct tnode **)B;
	if (a.weight > b.weight)
		return 1;
	else if (a.weight < b.weight)
		return -1;
	else
	{
		if (a.c > b.c)//权重相同则按字典序排序
			return 1;
		else//not equal
			return -1;
	}
}
void Bubble(struct tnode **p, int len)//比较函数--针对新增节点的单词冒泡调序
{
	//这里排序稳定性优点小坑, 记住只不断移动第一颗树即可
	int i;
	struct tnode *tmp;
	for (i=0; i < len-1; i++)
	{
		//仅一直追踪新增节点进行排序, 其他节点保持原来相对顺序
		if ((*(p))->weight >= (*(p+1))->weight)//相同权重时, 新节点排在老节点后
		{
			tmp = *(p);
			*(p) = *(p+1);
			*(p+1) = tmp;
			p++;
		}
		else 
			break;
	}
}

void createHTree()
{
	int size = 0, i, j=0;
	for (i=0; i<128; i++)
		if (Ccount[i]>0)
			size++;
	struct tnode *p;
	struct tnode ** F = 
		(struct tnode**)malloc(sizeof(struct tnode*)*size);
	for(i=0; i<128; i++)
		if(Ccount[i]>0){
			p = (struct tnode *)malloc(sizeof(struct tnode));
			p->c = i; p->weight = Ccount[i];
			p->left = p->right = NULL;
			F[j++] = p;
		}
	qsort(F, size, sizeof(struct tnode *), Cmp);
	i = 0;
	while (i < size-1)
	{
		p = (struct tnode *)malloc(sizeof(struct tnode));
		p->weight = F[i]->weight + F[i+1]->weight;
		p->left = F[i], p->right = F[i+1];
		F[++i] = p;
		Bubble(&F[i], size-i);
	}
	Root = F[size-1];
}
//【实验步骤2】结束

//【实验步骤3】开始

//先序遍历, 同时利用静态变量记录路径
void PreVisit(struct tnode * t)//局部函数
{
	static int len = 0;
	static char str[MAXSIZE];
	//需要确保第一个传进来的不是null
	if (t->right!=NULL){
		str[len++] = '1';
		PreVisit(t->right);
		str[len++] = '0';
		PreVisit(t->left);	
		len--;
	}//注意哈夫曼树不存在度为1情况, 因此也只用判断一个指针
	else{
		str[len] = 0;
		strcpy(HCode[t->c], str);
		len--;
	}
}

void makeHCode()
{
	PreVisit(Root);
} 

//【实验步骤3】结束

//【实验步骤4】开始
void Turn2HCode(char * src, char * des)//局部函数, 将文本转化为字符串版二进制编码
{
	int i, j, k;
	for (i=0, j=0; src[i]!=0; i++)
	{
		for (k=0; HCode[src[i]][k]!=0; k++)
			des[j++] = HCode[src[i]][k]; 
	}
	for (k=0; HCode[0][k]!=0; k++)
			des[j++] = HCode[src[i]][k]; 
	des[j] = 0;
}

void atoHZIP()
{
	char str[500], hc, code[2000];
	int len, i;
	fclose(Src);
	Src = fopen("input.txt","r");
	while (fgets(str, 500, Src) != NULL)
	{
		len = strlen(str);
		if (str[len-1]=='\n')
			str[len-1] = 0;
	}
	//转字符串版huffman码
	Turn2HCode(str, code);

	//转字节版huffman码, 这是由于c语言只允许以byte为最小单位向文件输出
	for(i=0; code[i] != 0; i++) {
		hc = (hc << 1) | (code[i]-'0');
		if((i+1)%8 == 0) {
			fputc(hc, Obj);     //输出到目标（压缩）文件中
			printf("%x",(unsigned char)hc);   //按十六进制输出到屏幕上
		}
	}

	//处理文件结束符(0)
	if ((i+1)%8 != 1){
		for (; (i+1)%8!=1; i++)
			hc = ( hc << 1 );
		fputc(hc, Obj);
		printf("%x",(unsigned char)hc); 
	}
}

//【实验步骤4】结束
```