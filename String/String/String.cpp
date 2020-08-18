// String.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>

typedef struct {
	char* ch;
	int length;
}Hstring;

void StrAssign(Hstring& string, char* chars)
{
	if (string.ch) free(string.ch);
	int len = 0;
	char* mid;
	for (mid = chars; *mid; len++, mid++)        //求传入chars的长度
		if (len == 0)        //chars为空的情况
		{
			string.ch = NULL;
			string.length = 0;
		}
		else        //chars不为空时，将传入chars的地址赋给string.ch
		{
			string.ch = (char*)malloc((len)*sizeof(char));
			string.ch = chars;
			string.length = len;
		}
}

void next(Hstring S, int* c)
{
	int i = 0, j = 1, max = 0, k = 0;    //i,k用于循环计数，j用于表示对应的字符位置,max用于返回对应next值
	int* p = c;        //p代替c进行操作
	*p = 0;
	p++;
	while (j < S.length)
	{
		max = (j + 1) / 2;
		i = 0;
		k = max;
		while (k > 0)
		{
			if (*(S.ch + i) == *(S.ch + j - k))
			{
				k--;
				i++;
			}        //依次判断字符是否相等
			else
			{
				max--;
				k = max;
				i = 0;
			}        //，若在判断过程中发现有不等的情况，从头开始查找判断max
			if (max == 0)
			{
				max = 1;
				break;
			}        //如果没有相同的子串，对应next的值返回1
		}
		*p = max;
		p++;
		j++;
	}
}

int index_KMP(Hstring s, Hstring subs, int *c)        //执行对比定位操作
{
	int i = 1, j = 1;
	s.ch = s.ch - 1;
	subs.ch = subs.ch - 1;        //使数组从零开始的特性适应于next数组
	while (i <= s.length && j <= subs.length)
	{
		if (j==0||*(s.ch + i) == *(subs.ch + j))
		{
			i++;
			j++;
		}        //继续比较后继字符
		else
		{
			j = *(c + j - 1);
		}        //模式串向后移动
	}
	if (j > subs.length)
	{
		return i - subs.length;
	}        //匹配成功
	else return -1;
}


int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("ERROR_01");
		exit(-1);
	}
	Hstring substring, string;
	int* nextlist,result;
	substring.ch = NULL;
	string.ch = NULL;
	StrAssign(string, argv[1]);
	StrAssign(substring, argv[2]);
	nextlist = (int*)malloc(substring.length * sizeof(int));
	next(substring, nextlist);
	result = index_KMP(string, substring, nextlist);
	printf("%d", result);
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
