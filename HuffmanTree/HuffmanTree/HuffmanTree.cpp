/*该文件由命令行参数输入一系列待编码字符串和待译码的字符串，根据霍夫曼编码的原理根据待编码字符串生成霍夫曼编码，
并根据此编码将待译码的字符串译码，并输出霍夫曼树的各个结点的元素、权值、所在层数、度以及编码和译码的结果*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//使用整型栈实现先序遍历HT树 
typedef int SElemtype;
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
#define ERROR_00 -1
#define ERROR_02 0  
#define ERROR 0 
#define OK 1  
#define MAXSIZE 100

typedef struct {
	SElemtype* base;
	SElemtype* top;
	int stacksize;
}SqStack;

int InitStack(SqStack& S)        //初始化栈
{
	S.base = (SElemtype*)malloc(STACK_INIT_SIZE * sizeof(SElemtype));
	if (!S.base) return ERROR;
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}

int StackEmpty(SqStack S)        //判断栈是否为空
{
	if (S.base == S.top) return true;
	else return false;
}

int Pop(SqStack& S, SElemtype& e)        //取出栈顶元素
{
	if (S.top == S.base) return ERROR;
	e = *--S.top;
	return OK;
}

int Push(SqStack& S, SElemtype e)        //元素入栈
{
	if (S.top - S.base > S.stacksize)
	{
		S.base = (SElemtype*)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemtype));
		if (!S.base) return ERROR;
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top = e;
	S.top++;
	return OK;
}


//霍夫曼树和霍夫曼编码的存储表示 
typedef struct HTNode
{
	int weight;
	int parent;
	int lchild;
	int rchild;
}HTnode, * HuffmanTree;
typedef char** Huffmancode;

int In(char b[], char a)        //判断a是否在b[]中出现，若返回值-1，则未出现；若返回值i，则为出现的下标  
{
	int i = 0;
	for (i = 0; i < strlen(b); i++)
	{
		if (a == b[i])
			return i;
	}
	return -1;
}

void MakeWeightString(int* W, char* W_elem, char* s, int& n)        //给每个字符赋权重的函数,W存储权重，W_elem存储W中每个权重对应的字符，n返回元素个数
{
	int i = 0, j = 0, t = 0, len_s = strlen(s);
	char t1;
	int t2 = 0;
	W_elem[0] = '\0';  //W_elem数组存放每个weight值对应的字符元素
	for (i = 0, j = 0; i < len_s; i++)
	{
		t = In(W_elem, s[i]); //判断数组s中的某元素是否在W_elem数组中出现
		if (t == -1)
		{
			W_elem[j] = s[i];
			W[j] = 0;
			W[j]++;
			W_elem[++j] = '\0';
		}
		else
		{
			W[t]++;
		}
	}
	n = strlen(W_elem);        //共有多少种字符
	for (i = 1; i < n; i++)        //按ASCII顺序排列
	{
		for (j = 0; j < n - i; j++)
		{
			if (W_elem[j] > W_elem[j + 1])
			{
				t1 = W_elem[j]; W_elem[j] = W_elem[j + 1]; W_elem[j + 1] = t1;
				t2 = W[j]; W[j] = W[j + 1]; W[j + 1] = t2;
			}
		}
	}
}

void Select(HuffmanTree& HT, int num, int& child1, int& child2)        //找出HT中从1到num为止，父节点为0的最小的两个孩子
{
	child1 = 0;        //参数初始化
	child2 = 0;
	int w1 = 0;
	int w2 = 0;
	int t = 0;
	for (int i = 1; i <= num; i++)
	{
		if (HT[i].parent == 0)        //如果child1还没有赋值，即第一次找最小
		{ 
			if (child1 == 0)
			{
				child1 = i;
				w1 = HT[i].weight;
				continue;
			}
			if (w1 > HT[i].weight)        //找最小的孩子，更新w1,child1
			{
				w1 = HT[i].weight;
				child1 = i;
				continue;
			}
		}
	}
	for (int i = 1; i <= num; i++)
	{
		if (HT[i].parent == 0 && i != child1)  //不再使用child1的值，找除了child1值以外的最小孩子 
		{
			if (child2 == 0)
			{
				child2 = i;
				w2 = HT[i].weight;
				continue;
			}
			if (w2 > HT[i].weight)        //找最小的孩子，更新w2,child2
			{
				w2 = HT[i].weight;
				child2 = i;
				continue;
			}
		}
	}
	if (w1 == w2 && child1 > child2)    //当二者权值相同时，保证child1的值比child2小，即满足ASCII码的顺序，确保生成Huffman树相同
	{ 
		t = child2;
		child2 = child1;
		child1 = t;
	}
}

void PreOrderTraverse(HuffmanTree& HT, int n, char a[])        //先序遍历二叉树并输出结点的度、层次、权值 n表示元素个数，a表示节点对应元素
{
	SqStack S;
	InitStack(S);
	int p = 2 * n - 1, t = 0;
	int* Degree;  //存储结点的度 
	int* Level;   //存储结点的层次 
	Degree = (int*)malloc((2 * n - 1) * sizeof(int));
	Level = (int*)malloc((2 * n - 1) * sizeof(int));
	for (int i = 0; i < 2 * n - 1; i++)
	{
		Level[i] = 1;        //先初始化所有节点层次为1
		t = HT[i + 1].parent;
		while (t)        //如果存在双亲节点，则层次加一
		{
			Level[i]++;
			t = HT[t].parent;
		}
	}
	for (int i = 0; i < 2 * n - 1; i++)        //计算节点的度
	{
		Degree[i] = 0;
		if (HT[i + 1].lchild) Degree[i]++;
		if (HT[i + 1].rchild) Degree[i]++;
	}
	printf("TNode    weight degree level\n");
	while (p || !StackEmpty(S))        //先序遍历
	{
		if (p)
		{
			Push(S, p);
			if (p <= n)
				printf("%2d    %c    %2d    %2d    %2d\n", p, a[p - 1], HT[p].weight, Degree[p - 1], Level[p - 1]);
			else
				printf("%2d NULL    %2d    %2d    %2d\n", p, HT[p].weight, Degree[p - 1], Level[p - 1]);
			p = HT[p].lchild;
		}
		else {
			Pop(S, p);
			p = HT[p].rchild;
		}
	}
	free(Degree);
	free(Level);
}
 
int HuffmanCoding(HuffmanTree& HT, Huffmancode& HC, int* w, char* w_elem, int n)        //Huffman编码，HT为Huffman树，HC为生成Huffman编码，w与w_elem意义同上，n为节点个数
{
	int m = 0, i = 0, p = 0;
	int child1 = 0, child2 = 0;
	char* cd;
	if (n <= 1)        //当节点数小于一时不合要求
	{
		return ERROR_02;
	}
	m = n * 2 - 1;
	if (!(HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode))))        //若开辟地址不成功
	{
		printf("OVERFLOW");
		return ERROR_00;
	}
	for (i = 1; i <= n; i++, w++)        //0号结点不用，i=1开始遍历 
	{
		HT[i].lchild = 0;
		HT[i].rchild = 0;
		HT[i].parent = 0;
		HT[i].weight = *w;
	}
	for (; i <= m; i++)        //初始化剩余节点
	{
		HT[i].lchild = 0;
		HT[i].parent = 0;
		HT[i].rchild = 0;
		HT[i].weight = 0;
	}
	for (i = n + 1; i <= m; i++)        //更新节点
	{
		Select(HT, i - 1, child1, child2);  //select函数更新结点 
		HT[child1].parent = i;
		HT[child2].parent = i;
		HT[i].lchild = child1;
		HT[i].rchild = child2;
		HT[i].weight = HT[child1].weight + HT[child2].weight;
	}
	PreOrderTraverse(HT, n, w_elem);        //先序非递归遍历二叉树T
	if (!(HC = (char**)malloc((n + 1) * sizeof(char*))))        //为Huffman编码开辟空间
	{
		printf("OVERFLOW");
		return ERROR_00;
	}
	if (!(cd = (char*)malloc((n + 1) * sizeof(char))))        //为cd暂时存放编码开辟空间
	{
		printf("OVERFLOW");
		return ERROR_00;
	}

	p = m;
	int cdlen = 0;
	for (i = 1; i <= m; i++) HT[i].weight = 0;        //遍历赫夫曼树时用作节点状态标志
	//无栈非递归求霍夫曼编码 
	while (p)        //从根结点开始求Huffman编码
	{
		if (HT[p].weight == 0)        //向左
		{
			HT[p].weight = 1;
			if (HT[p].lchild != 0)
			{
				p = HT[p].lchild;
				cd[cdlen++] = '0';
			}
			else if (HT[p].rchild == 0)        //登记叶子节点的字符编码
			{
				HC[p] = (char*)malloc((cdlen + 1) * sizeof(char));
				cd[cdlen] = '\0';
				for (i = 0; i <= cdlen; i++)        //复制编码
				{
					HC[p][i] = cd[i];
				}
			}
		}
		else if (HT[p].weight == 1)        //向右
		{
			HT[p].weight = 2;
			if (HT[p].rchild != 0)
			{
				p = HT[p].rchild;
				cd[cdlen++] = '1';
			}
		}
		else        //遇到父节点，编码长度减一
		{
			HT[p].weight = 0;
			p = HT[p].parent;
			--cdlen;
		}
	}
	free(cd);
	return OK;
}
 
void PrintCoding(char s[], int n, char a[], Huffmancode HC)        //打印s[]对应的赫夫曼编码，a为已知编码的元素序列
{
	int i = 0, j = 0;
	for (int i = 0; i < strlen(s); i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (s[i] == a[j - 1])
				printf("%s", HC[j]);
		}
	}
}

int HuffmanDeCoding(char s[], HuffmanTree& HT, int n, char a[], char a_[])        //赫夫曼译码，s[]为待译串，n为节点个数，a[]为已知编码的元素序列，a_[]为翻译结果
{
	int m = 2 * n - 1, flag = 0, p = m, j = 0;        //flag用于判断是否找到叶子结点
	int len_s = strlen(s);
	for (int i = 0; i < len_s; i++)
	{
		if (s[i] == '1')        //数字为1代表找右子树
		{
			p = HT[p].rchild;
			if (HT[p].lchild == 0 && HT[p].rchild == 0)        //判断是否找到叶子结点
			{
				a_[j++] = a[p - 1];
				a_[j] = '\0';
				flag = 1;
			}
			if (p == 0)
			{
				return ERROR;
			}
		}
		else if (s[i] == '0')        //数字为0代表找左子树
		{
			p = HT[p].lchild;
			if (HT[p].lchild == 0 && HT[p].rchild == 0)
			{
				a_[j++] = a[p - 1];
				a_[j] = '\0';
				flag = 1;
			}
			if (p == 0)
			{
				return ERROR;
			}
		}
		if (i == len_s - 1 && flag == 0)        //s没有对应译码
		{
			return ERROR;
		}
		if (flag == 1)        //参数重新初始化
		{
			flag = 0;
			p = m;
		}
	}
	return OK;
}



int main(int argc, char* argv[])
{
	char s[MAXSIZE];  //存命令行输入的字符串 
	int k = 0, i = 0;
	int t = 0, m = 0;
	char s_[MAXSIZE];  //命令行需译出的字符串 

	if (argc < 3)        //命令行参数不对，ERROR_01 
	{
		printf("ERROR_01");
		return 0;
	}
	else if (argc > 3)        //将命令行输入的字符串放进s[]
	{
		for (int i = 1; i < argc - 1; i++)
		{
			for (int j = 0; j < strlen(argv[i]); j++)
			{
				s[k++] = argv[i][j];
			}
			s[k++] = ' ';
		}
		s[--k] = '\0';
	}
	else        //将命令行参数存入s[]
	{
		for (int i = 0; i < strlen(argv[1]); i++)
			s[k++] = argv[1][i];
		s[k] = '\0';
	}
	if (strlen(s) < 20)        //输入字符串长度应>=20
	{
		printf("ERROR_02");
		return 0;
	}
	char W_elem[MAXSIZE] = { 0 };
	int W[MAXSIZE], n = 0;
	HuffmanTree HT;
	Huffmancode HC;
	MakeWeightString(W, W_elem, s, n);
	m = HuffmanCoding(HT, HC, W, W_elem, n);        //返回编码状态
	if (m == 0)
	{
		printf("ERROR_02");
		return 0;
	}
	if (m == -1)
	{
		printf("OVERFLOW");
		return 0;
	}
	for (int j = 1; j <= n; j++)        //输出每个元素及其对应的编码 
	{
		printf("%c %s\n", W_elem[j - 1], HC[j]);
	}
	t = HuffmanDeCoding(argv[argc - 1], HT, n, W_elem, s_);        //判断译码状态
	if (t == 0)
	{
		printf("ERROR_03");
		return 0;
	}
	PrintCoding(s, n, W_elem, HC);
	printf(" ");
	printf("%s", s_);
	for (i = 1; i <= n; i++)
		free(HC[i]);
	free(HT);
	return 0;
}
