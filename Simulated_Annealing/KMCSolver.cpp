//文件名：KMCSolver.cpp
//KMCSolver类的完整实现 
#include <iostream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include "KMCSolver.h"
#include <cstdlib>
#include <cstring>
using namespace std;
typedef unsigned long long ull;

const int KMCSolver::Method::dx[4] = { -2,2,0,0 };
const int KMCSolver::Method::dy[4] = { 0,0,-2,2 };

KMCSolver::Method::Method() 
{
	srand(time(NULL));
	for (int i = 0; i < 3 * SIZE; i += 3) {
		sol[i] = rand() % 7;
	}
	for (int i = 1; i < 3 * SIZE; i += 3) {
		sol[i] = rand() % 7;
	}
	for (int i = 2; i < 3 * SIZE; i += 3) {
		sol[i] = rand() % 4;
	}

	//仅用于调试
	/*sol[0] = 3;
	sol[1] = 2;
	sol[2] = 3;
	sol[3] = 5;
	sol[4] = 2;
	sol[5] = 0;
	sol[6] = 2;
	sol[7] = 4;
	sol[8] = 1;*/
}

KMCSolver::Method KMCSolver::Method::change()
{
	KMCSolver::Method m;
	memcpy(m.sol, sol, sizeof sol);
	srand(time(NULL));
	int pos = 3 * (rand() % SIZE);
	m.sol[pos] = rand() % 7;
	m.sol[pos + 1] = rand() % 7;
	m.sol[pos + 2] = rand() % 4;

	return m;
}

int KMCSolver::Method::cost(ull board, int left_chess)
{
	for (int i = 0; i < 3 * SIZE; i += 3) {
		if (valid(sol[i], sol[i + 1])) {
			int xx = sol[i] + dx[sol[i + 2]];
			int yy = sol[i + 1] + dy[sol[i + 2]];
			int midx = (sol[i] + xx) >> 1;
			int midy = (sol[i + 1] + yy) >> 1;

			/*if (sol[i] == 2 && sol[i + 1] == 4) {
				system("pause");
			}*/

			if (valid(xx, yy)) {
				if (can_move(sol[i], sol[i + 1], xx, yy, board)) {
					board = board + ((ull)1 << (7 * xx + yy)) - ((ull)1 << (7 * sol[i] + sol[i + 1])) - ((ull)1 << (7 * midx + midy));
					--left_chess;
					if (left_chess == 1) break;
				}
			}
		}
	}
	//cerr << board << "\n";
	return left_chess;
}

bool can_move(int x, int y, int xx, int yy, ull sign)
{
	bool sign1 = (sign >> (7 * x + y)) % 2 == 1;
	bool sign2 = (sign >> (7 * xx + yy)) % 2 == 0;
	bool sign3 = (sign >> (7 * (xx + x) / 2 + (y + yy) / 2)) % 2 == 1;
	return sign1 && sign2 && sign3;
}

inline bool valid(int x, int y) 
{
	return (2 <= x && x <= 4 && 0 <= y && y <= 6) || (2 <= y && y <= 4 && 0 <= x && x <= 6);
}

bool KMCSolver::accept(double delta, double temper)
{
	srand(time(NULL));
	if (delta <= 0) return true;
	return rand() <= exp((-delta) / temper) * RAND_MAX;
}

void KMCSolver::input()
{
	ull chess;
	cout << "孔明棋解法生成器：" << "\n";
	for (int i = 0; i < 7; ++i){
		for (int j = 0; j < 7; ++j){
			cin >> chess;
			if (valid(i, j)) {
				board_condition += chess << (7 * i + j);
				if (chess) {
					++mem_of_chess;
				}
			}
		}
	}
}

void KMCSolver::work()
{
	const double max_temper = 10000;
	const double dec = 0.9999;
	double temp = max_temper;
	Method m;
	while (temp > 0.01) {
		Method m2 = m.change();
		if (accept(m2.cost(board_condition, mem_of_chess) - m.cost(board_condition, mem_of_chess), temp)) {
			m = m2;
		}
		temp *= dec;
	}
	answer = m;
}

void KMCSolver::output()
{
	int left_chess = mem_of_chess;
	ull board = board_condition;
	for (int i = 0; i < 3 * Method::SIZE; i += 3) {
		if (valid(answer.sol[i], answer.sol[i + 1])) {
			int xx = answer.sol[i] + Method::dx[answer.sol[i + 2]];
			int yy = answer.sol[i + 1] + Method::dy[answer.sol[i + 2]];
			int midx = (answer.sol[i] + xx) >> 1;
			int midy = (answer.sol[i + 1] + yy) >> 1;
			if (can_move(answer.sol[i], answer.sol[i + 1], xx, yy, board)) {

				board = board + ((ull)1 << (7 * xx + yy)) - ((ull)1 << (7 * answer.sol[i] + answer.sol[i + 1]))
					- ((ull)1 << (7 * midx + midy));
				--left_chess;

				cout << "(" << answer.sol[i] << "," << answer.sol[i + 1] << ")->"
					<< "(" << xx << "," << yy << ")\n";

				if (left_chess == 1) break;
			}
		}
	}

	cout << "left chess: " << left_chess << "\n";
}
