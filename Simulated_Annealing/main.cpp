#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;
typedef unsigned long long ull;

class KMCSolver;

int main()
{
	//���벿��
	//board_condition�����洢�������������  
	ull board_condition = input();
	
	//������ 
	KMCSolver solver(board_condition);
	solver.work();
	
	//����� 
	solver.output(); 
	
	return 0;	
} 
