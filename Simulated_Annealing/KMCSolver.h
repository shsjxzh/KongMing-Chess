//�ļ�����KMCSolver.h
//KMCSolver����������� 
#ifndef _KMCSolver_h
#define _KMCSolver_H
#include <iostream>
using namespace std;

typedef unsigned long long ull;

class KMCSolver{
private:
	//����˽�ú���
	struct Method {
		static const int SIZE = 2000;
		static const int dx[4];
		static const int dy[4];

		short sol[3 * SIZE] = { 0 };  //3��Ϊһ�顣ǰ������ʾҪ�ƶ������ӵ�λ�ã���һ����ʾ�ƶ��ķ�ʽ��0��1��2��3�ֱ�����ϡ��¡����ҡ�

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
	//���캯�� 
	KMCSolver() { board_condition = 0; mem_of_chess = 0; };
	//���幤�� 
	void input();
	void work();
	void output();
	
	friend inline bool valid(int x, int y);
	friend bool can_move(int x, int y, int xx, int yy, ull sign);
};

inline bool valid(int x, int y);
bool can_move(int x, int y, int xx, int yy, ull sign);
#endif
