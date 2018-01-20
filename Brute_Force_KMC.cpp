#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

typedef unsigned long long ull;

//存储初始棋盘情况
int mem_of_chess = 0;

//用来存储棋盘上每个点权值对2取对数
const int wei_log2[7][7] = {
	{ 0,1,2,3,4,5,6 },
	{ 7,8,9,10,11,12,13 },
	{ 14,15,16,17,18,19,20 },
	{ 21,22,23,24,25,26,27 },
	{ 28,29,30,31,32,33,34 },
	{ 35,36,37,38,39,40,41 },
	{ 42,43,44,45,46,47,48 }
};

//移动方式
const int dx[4] = { -2,2,0,0 };
const int dy[4] = { 0,0,-2,2 };

//放置用于迭代的可能状态
map<ull,vector<char>> map_state[2];

inline bool valid(int x, int y)
{
	return (2 <= x && x <= 4 && 0 <= y && y <= 6) || (2 <= y && y <= 4 && 0 <= x && x <= 6);
}

inline bool can_move(int x,int y,int xx,int yy, int board[][7])
{
	return board[x][y] && board[(x + xx) >> 1][(y + yy) >> 1] && !board[xx][yy];
}

ull hash_produce(int board[][7])
{
	ull hash_ans = 0;
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			if (valid(i, j)) {
				hash_ans += ((ull)board[i][j] << wei_log2[i][j]);
			}
		}
	}
	return hash_ans;
}

void input()
{
	ull chess;
	ull board_condition = 0;
	cout << "孔明棋解法生成器：" << "\n";
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			cin >> chess;
			if (valid(i, j)) {
				board_condition += (chess << wei_log2[i][j]);
				if (chess) {
					++mem_of_chess;
				}
			}
		}
	}
	vector<char> ept;
	map_state[0][board_condition] = ept ;
}

int work()
{
	//这是用来确定哪个map是正在工作的，哪个是要往里存数据的
	int work_on = 0;
	int stor = 1;
	//遍历map用到的迭代器
	map<ull, vector<char>>::iterator itr;
	//记录解码以后的棋盘情况
	int board[7][7] = { 0 };

	//判断什么时候停下
	bool flag = false;
	//for (int k = 1; k < mem_of_chess; ++k) {
	while (true) {
		for (itr = map_state[work_on].begin(); itr != map_state[work_on].end(); ++itr) {
			//刷新棋盘
			ull hash_board = itr->first;
			for (int i = 0; i < 7; ++i) {
				for (int j = 0; j < 7; ++j) {
					if (valid(i, j)) {
						board[i][j] = int((hash_board >> wei_log2[i][j]) & (ull)1);
					}
				}
			}

			//开始枚举情况
			vector<char> tmp = itr->second;
			int xx, yy;
			ull hash_num;
			for (int i = 0; i < 7; ++i) {
				for (int j = 0; j < 7; ++j) {
					if (valid(i, j)) {
						for (int dir = 0; dir < 4; ++dir) {
							xx = i + dx[dir];
							yy = j + dy[dir];

							if (valid(xx, yy)) {
								if (can_move(i, j, xx, yy, board)) {
									//do sth
									board[i][j] = 0; board[(i + xx) >> 1][(j + yy) >> 1] = 0; board[xx][yy] = 1;
									hash_num = hash_produce(board);
									tmp.push_back(char(7 * i + j)); tmp.push_back(char(7 * xx + yy));

									//存储新的情况。
									map_state[stor].insert(pair<ull, vector<char>>(hash_num, tmp));
									if (hash_num == (ull(1) << wei_log2[3][3])) {
										flag = true;
										break;
									}

									//undo the operation
									tmp.pop_back(); tmp.pop_back();
									board[i][j] = 1; board[(i + xx) >> 1][(j + yy) >> 1] = 1; board[xx][yy] = 0;
								}
							}
						}
					}
				}
			}
			
			if (flag) break;
		}

		if (flag) break;
		//清空map以节省内存
		map_state[work_on].clear();
		//确定每次的工作map和用来存结果的map
		std::swap(work_on, stor);
	}

	return stor;
}

void output(int loca)
{
	vector<char> out = map_state[loca].find((ull)1 << wei_log2[3][3])->second;
	vector<char>::iterator itr = out.begin();
	for (; itr != out.end(); itr += 2) {
		cout << "(" << *itr / 7 << "," << *itr % 7 << ")"
			<< "->(" << *(itr + 1) / 7 << "," << *(itr + 1) % 7 << ")\n";
	}

}

int main()
{
	input();

	int loca = work();
	
	output(loca);
 	
	return 0;
}

/*To Do:
	can_move
	hash
*/