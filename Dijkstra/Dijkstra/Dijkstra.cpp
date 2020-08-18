// Dijkstra.cpp : 利用Dijkstra算法求最短路径并输出最短路径
//

#include <iostream>
#include <string.h>

#define INFINITY 1000
#define MAX_VERTEX_NUM 20
#define SIZE 50

typedef enum { DG, DN, UDG, UDN } GraphKind;
typedef struct {
	char* vex[MAX_VERTEX_NUM];
	int arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
	int vexnum, arcnum;
	GraphKind kind;
}MGraph;


void ShortestPath_DIJ(MGraph G, int v0, int** P, int* D, int* final, int path[][MAX_VERTEX_NUM]) {
	int v, w;
	int i, min,j;
	for (v = 0; v < G.vexnum; v++) {
		final[v] = 0;
		D[v] = G.arcs[v0][v];
		D[v0] = 0;
		for (w = 0; w < G.vexnum; w++) {        //设空路径
			P[v][w] = 0;
			path[v][w] = -1;
			if (w == 0)
			{
				path[v][w] = v0;
			}        //将起点加入路径
		}
		if (D[v] < INFINITY)
		{
			P[v][v0] = 1;
			P[v][v] = 1;
			path[v][1] = v;        //将现有路径加入路径，完成路径初始化
		}
	}
	final[v0] = 1;        //初始化，v0顶点属于S集
	//开始主循环，每次求得v0到某个顶点v的最短路径，并加v到S集
	for (i = 1; i < G.vexnum; i++) {        //其余G.vexnum-1个顶点
		min = INFINITY;        //当前所知离v0顶点最近的距离
		for (w = 0; w < G.vexnum; w++) {
			if (!final[w]) {        //w顶点在V-S集中
				if (D[w] < min) {        //w顶点离v0顶点更近
					v = w;
					min = D[w];
				}
			}
		}
		final[v] = 1;        //加入S集
		for (w = 0; w < G.vexnum; w++) {
			if (!final[w] && (min + G.arcs[v][w] < D[w])) {
				D[w] = min + G.arcs[v][w];
				P[w] = P[v];
				for (j = 0; j < G.vexnum; j++)
				{
					path[w][j] = path[v][j];
				}
				for (j = 0; path[v][j] != -1; j++);
				path[w][j] = w;
				P[w][w] = 1;
			}
		}
	}
}

void InitGraph(MGraph& G)        //图初始化
{
	int i = 0, j = 0;
	G.arcnum = 18;
	G.vexnum = 13;
	for (i = 0, j = 0; i < G.vexnum; i++, j++)
	{
		G.arcs[i][j] = -1;
	}
	G.kind = UDN;
	G.vex[0] = "北门";
	G.vex[1] = "饮水思源";
	G.vex[2] = "腾飞塔";
	G.vex[3] = "图书馆";
	G.vex[4] = "教学主楼";
	G.vex[5] = "活动中心";
	G.vex[6] = "南门";
	G.vex[7] = "宪梓堂";
	G.vex[8] = "西迁馆";
	G.vex[9] = "传送门1";
	G.vex[10] = "传送门2";
	G.vex[11] = "传送门3";
	G.vex[12] = "传送门4";
	G.arcs[0][1] = 18;
	G.arcs[0][11] = 22;
	G.arcs[1][2] = 19;
	G.arcs[1][9] = 27;
	G.arcs[2][3] = 23;
	G.arcs[2][11] = 4;
	G.arcs[2][12] = 32;
	G.arcs[3][4] = 15;
	G.arcs[3][9] = 4;
	G.arcs[3][10] = 4;
	G.arcs[4][7] = 8;
	G.arcs[4][5] = 21;
	G.arcs[5][6] = 30;
	G.arcs[6][7] = 14;
	G.arcs[6][8] = 20;
	G.arcs[6][10] = 21;
	G.arcs[7][12] = 4;
	G.arcs[8][10] = 43;
	for (i = 0; i < G.vexnum; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (G.arcs[j][i]<0)
			{
				G.arcs[j][i] = INFINITY;
			}
			if (j < i)
			{
				G.arcs[i][j] = G.arcs[j][i];
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("ERROR_01");
		return -1;
	}
	MGraph G;
	InitGraph(G);
	int i = 0, tag1 = 0, tag2 = 0,j;        //tag用于表示是否找到起点终点
	int start = 0, end = 0;
	int** P, * D, * final, path[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
	P = (int**)malloc(G.vexnum * sizeof(int*));
	for (i = 0; i < G.vexnum; i++)
	{
		P[i] = (int*)malloc(G.vexnum * sizeof(int));
	}
	D = (int*)malloc(G.vexnum * sizeof(int));
	final = (int*)malloc(G.vexnum * sizeof(int));
	for (i = 0; i < G.vexnum; i++)
	{
		if (strcmp(argv[1], G.vex[i])==0)
		{
			start = i;
			tag1 = 1;
		}
		if (strcmp(argv[2], G.vex[i])==0)
		{
			end = i;
			tag2 = 1;
		}
	}
	if (!tag1 || !tag2)
	{
		printf("ERROR_02");
		return -1;
	}
	ShortestPath_DIJ(G, start, P, D, final,path);
	if (!final[end])
	{
		printf("ERROR_02");
	}
	else
	{
		printf("%d", D[end]);
	}
	/*for (i = 0; path[end][i]!=-1 ; i++)
	{
		if (i == 0)
		{
			printf("%s", G.vex[path[end][i]]);
		}
		else
			printf("->%s", G.vex[path[end][i]]);
	}*/
	free(P);
	free(D);
	free(final);
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
