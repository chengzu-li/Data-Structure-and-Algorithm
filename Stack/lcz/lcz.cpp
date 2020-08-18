// lcz.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	double* base;
	double* top;
	int stacksize;
}Sqstacknum;      //用于存储数字

typedef struct {
	char* base;
	char* top;
	int stacksize;
}Sqstackopt;     //用于存储运算符

#define error -1
#define ok 1
#define overflow -2
#define stackincre 10
typedef int Status;

Status popnum(Sqstacknum* S, double* e)    //pop数字
{
	if ((*S).top == (*S).base) {
		printf("ERROR_02");
		exit(error);
	}
	(*S).top--;
	*e = *(*S).top;
	return ok;
}

Status popopt(Sqstackopt* S, char* e)      //pop字符
{
	if ((*S).top == (*S).base) return error;
	(*S).top--;
	*e = *(*S).top;
	return ok;
}

Status pushnum(Sqstacknum* S, double e)    //push数字
{
	if ((*S).top - (*S).base >= (*S).stacksize)
	{
		(*S).base = (double*)realloc((*S).base, ((*S).stacksize + stackincre) * sizeof(double));
		if (!(*S).base) exit(overflow);
		(*S).top = (*S).base + (*S).stacksize;
		(*S).stacksize = (*S).stacksize + stackincre;
	}
	*(*S).top = e;
	(*S).top++;
	return ok;
}

Status pushopt(Sqstackopt* S, char e)    //push字符
{
	if ((*S).top - (*S).base >= (*S).stacksize)
	{
		(*S).base = (char*)realloc((*S).base, ((*S).stacksize + stackincre) * sizeof(char));
		if (!(*S).base) exit(overflow);
		(*S).top = (*S).base + (*S).stacksize;
		(*S).stacksize = (*S).stacksize + stackincre;
	}
	*(*S).top = e;
	(*S).top++;
	return ok;
}

Status Initstacknum(Sqstacknum* S)    //初始化数字站
{
	(*S).base = (double*)malloc(100 * sizeof(double));
	if (!(*S).base)  exit(overflow);
	(*S).stacksize = 100;
	(*S).top = (*S).base;
	return ok;
}

Status Initstackopt(Sqstackopt* S)    //初始化字符栈
{
	(*S).base = (char*)malloc(100 * sizeof(char));
	if (!(*S).base)  exit(overflow);
	(*S).stacksize = 100;
	(*S).top = (*S).base;
	return ok;
}

char precede(char a, char b)    //判断运算符优先级
{
	switch (b) {
	case '+':
	case '-':
		if (a == '(' || a == '#' || a == '[' || a == '{')  return '<';
		else {
			return '>'; break;
		}
	case '*':
	case '/':
		if (a == '*' || a == '/' || a == ')' || a == '^' || a == ']' || a == '}')  return '>';
		else {
			return '<'; break;
		}
	case '(':
		if (a == '(' || a == ')' || a == '}' || a == ']') {
			printf("ERROR_02");
			exit(error);
		}
		else return '<';
	case ')':
		if (a == '(')  return '=';
		else if (a == '#' || a == ']' || a == '}' || a == ')') {
			printf("ERROR_02");
			exit(overflow);
		}
		else if (a == '[' || a == '{')  return '<';
		else return '>';
	case '#':
		if (a == '(') {
			printf("ERROR_02");
			exit(overflow);
		}
		else if (a == '#') return '=';
		else return '>';
	case '^':
		if (a == '^' || a == ')' || a == ']' || a == '}') return '>';
		else return '<';
	case '[':
		if (a == '(' || a == ')' || a == '[' || a == ']' || a == '}')
		{
			printf("ERROR_02");
			exit(error);
		}
		else  return '<';
	case ']':
		if (a == '(' || a == ']' || a == '}' || a == '#')
		{
			printf("ERROR_02");
			exit(error);
		}
		else if (a == '[')  return '=';
		else if (a == '{')  return '<';
		else return '>';
	case '{':
		if (a == '(' || a == ')' || a == '[' || a == ']' || a == '{' || a == '}')
		{
			printf("ERROR_02");
			exit(error);
		}
		else return '<';
	case '}':
		if (a == '(' || a == '[' || a == '}' || a == '#')
		{
			printf("ERROR_02");
			exit(error);
		}
		else if (a == '{')  return '=';
		else return '>';
	}
}

Status In(char c)    //判断是否为运算符
{
	char a[] = { '+','-','/','*','%','(',')','#','[',']','{','}','^' };
	int i = 0;
	for (i = 0; i < 14; i++)
	{
		if (c == a[i]) return ok;
	}
	return error;
}

Status gettopnum(Sqstacknum S, double* e)    //取栈顶元素
{
	if (S.top == S.base)  return error;
	*e = *(S.top - 1);
	return ok;
}

Status gettopopt(Sqstackopt S, char* e)     //取栈顶元素
{
	if (S.top == S.base)  return error;
	*e = *(S.top - 1);
	return ok;
}

double operate(double a, double b, char c)    //运算操作
{
	double output=0;
	if (c == '+')
		output = b + a;
	if (c == '-')
		output = b - a;
	if (c == '*')
		output = b * a;
	if (c == '/')
		output = b / a;
	if (c == '%')
		output = (int)b % (int)a;
	if (c == '^')
		output = pow(b, a);
	return output;
}

int main(int argc, char* argv[])
{
	if (argc == 0)
	{
		printf("no_parameter_input");
		exit(error);
	}    //参数有效性检验
	Sqstackopt OPTR;
	Sqstacknum OPND;
	double num = 0, a, b;
	int i, j, m[10], n[3], tag = 0, t, p, k = 0;   //m, n用于存储小数的整数部分数字与小数部分数字，tag表示是否有小数部分
	Initstackopt(&OPTR);
	Initstacknum(&OPND);       //initialize the stack for operators and numbers
	pushopt(&OPTR, '#');                        //将运算符栈栈底元素定为#
	char c, theta;
	c = argv[1][k];
	while (c != '#' || *(OPTR.top - 1) != '#')
	{
		if (c == '\0')
		{
			c = '#';
		}
		if (c == '#' && *(OPTR.top - 1) == '#')
			break;        //判断何时命令行参数结束
		if (In(c) < 0)    //如果读入不是操作符
		{
			i = 0; j = 0; num = 0;
			while ((c <= '9' && c >= '0') || c == '.')    //是数字部分
			{
				if ((c <= '9' && c >= '0') && tag == 0)
				{
					m[i] = c - '0';
					i = i + 1;
					k = k + 1;
					c = argv[1][k];
				}         //读存整数部分
				else if (c == '.')
				{
					tag = 1;
					k = k + 1;
					c = argv[1][k];
				}        //是否有小数部分
				else if (tag == 1 && (c <= '9' && c >= '0'))
				{
					n[j] = c - '0';
					j++;
					k = k + 1;
					c = argv[1][k];
				}        //读存小数部分
				else {
					printf("ERROR_02");
					exit(error);
				}        //输入其他字符返回ERROR_02
			}
			for (t = i - 1, p = 0; t >= 0, p < i; t--, p++)
			{
				num = num + m[p] * pow(10, t);
			}
			for (t = j; t > 0; t--)
			{
				num = num + n[t - 1] * pow(10, -t);
			}           //计算小数的值赋给num
			if ((c > '9' || c < '0') && c != '.' && c != '\0' && In(c) < 0)
			{
				printf("ERROR_02");
				exit(error);
			}
			pushnum(&OPND, num);
			tag = 0;    //计算完小数后重置tag
		}
		else
		{
			switch (precede(*(OPTR.top - 1), c))
			{
			case '<':
				pushopt(&OPTR, c);
				k = k + 1;
				c = argv[1][k];
				break;
			case '=':         //去括号
				popopt(&OPTR, &c);
				k = k + 1;
				c = argv[1][k];
				break;
			case '>':
				popnum(&OPND, &a); popnum(&OPND, &b);
				popopt(&OPTR, &theta);
				if (theta == '/'&& a == 0)
				{
					printf("ERROR_03");
					exit(error);
				}
				
				pushnum(&OPND, operate(a, b, theta));
				break;
			}
		}
	}
	printf("%g", *(OPND.top - 1));
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
