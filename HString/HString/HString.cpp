// HString.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>

typedef struct
{
	char* ch;
	int length;
}HString;

void StrAssign(HString& string, char* chars)
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

void getnext(HString& string, int *next)        //求next值
{
	next[1] = 0;        //对于j=1时的next值赋值
	int i = 1, j = 0;        //从i=1开始
	while (i < string.length)
	{
		if (j == 0 || *(string.ch + i - 1) == *(string.ch + j - 1))
		{
			i++;
			j++;
			next[i] = j;
		}
		else
		{
			j = next[j];
		}
	}
}


int Index_KMP(HString string, HString substring)        //执行对比定位操作
{
	int i = 1, j = 1;
	int* next;
	next = (int*)malloc(sizeof(int) * substring.length);        //next分配内存
	getnext(substring, next);        //求next数组
	while (i <= string.length && j <= substring.length)
	{
		if (j == 0 || *(string.ch + i - 1) == *(substring.ch + j - 1))        //继续比较字符
		{
			i++;
			j++;
		}
		else        //进行下一位置比较
		{
			j = next[j];
		}
	}
	if (j > substring.length)        //匹配成功
	{
		return(i - substring.length);
	}
	else return -1;        //匹配失败

}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("ERROR_01");
		return(-1);
	}
	int index;
	HString string;
	HString substring;
	string.ch = NULL;
	substring.ch = NULL;o
	StrAssign(string, argv[1]);        //主串赋值
	StrAssign(substring, argv[2]);        //字串赋值
	index = Index_KMP(string, substring);        //对比定位
	printf("%d", index);
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
