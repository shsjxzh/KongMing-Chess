//文件名：KMCSolver.h
//KMCSolver类的完整声明 
#ifndef _KMCSolver_h
#define _KMCSolver_H
#include <iostream>
using namespace std;

typedef unsigned long long ull;

class KMCSolver{
private:
	//具体私用函数
	struct Method {
		static const int SIZE = 2000;
		static const int dx[4];
		static const int dy[4];

		short sol[3 * SIZE] = { 0 };  //3个为一组。前两个表示要移动的棋子的位置，后一个表示移动的方式：0，1，2，3分别代表上、下、左、右。

		Method();
		Method change();
		int cost(ull board, int left_chess);
		
		friend inline bool valid(int x, int y);
		friend bool can_move(int x, int y, int xx, int yy, ull sign);
	};
	bool accept(double delta, double temper);

private:
	ull board_condition;
	Method answer;
	int mem_of_chess;

public:
	//构造函数 
	KMCSolver() { board_condition = 0; mem_of_chess = 0; };
	//具体工作 
	void input();
	void work();
	void output();
	
	friend inline bool valid(int x, int y);
	friend bool can_move(int x, int y, int xx, int yy, ull sign);
};

inline bool valid(int x, int y);
bool can_move(int x, int y, int xx, int yy, ull sign);
#endif
