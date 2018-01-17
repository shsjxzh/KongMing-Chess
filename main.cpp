#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
typedef unsigned long long ull;

class KMCSolver;

int main()
{
	//输入部分
	//board_condition用来存储孔明棋棋盘情况  
	ull board_condition = input();
	
	//问题解决 
	KMCSolver solver(board_condition);
	solver.work();
	
	//输出答案 
	solver.output(); 
	
	return 0;	
} 
