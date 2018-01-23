#include <iostream>
#include <map>
#include <algorithm>
//#include <utility>
using namespace std;

typedef unsigned long long ull;
typedef unsigned char uc;
//typedef map<ull, node> MAP;
//存储初始棋盘情况
int mem_of_chess = 0;

//调试用
//int node_count = 0;

//用来存储棋盘上每个点权值对2取对数
//需要继续修改权值
const int wei_log2[7][7] = {
	{  0, 0,25,27,23, 0, 0 },
	{  0, 0,24,26,22, 0, 0 },
	{ 16,15,31,21,30,10,11 },
	{ 20,19,14,32, 7,12,13 },
	{ 18,17,28, 0,29, 8, 9 },
	{  0, 0, 1, 5, 3, 0, 0 },
	{  0, 0, 2, 6, 4, 0, 0 }
};

//在计算hash的时候方便
//上、左、右、下：0，1，2，3
const int UP = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int DOWN = 3;

const int MID = 0;
const int L_UP = 1;
const int R_UP = 2;
const int R_DOWN = 3;
const int L_DOWN = 4;


//移动方式
const int dx[4] = { -2,2,0,0 };
const int dy[4] = { 0,0,-2,2 };

struct step {
	//这里也改
	step* pre;	   //前一步
	uc context;    //每一步的具体内容
	uc nxt_count;   //保存还剩多少个分支
	step(uc _con = 255, uc _nxt_count = 0, step* _pre = NULL) :context(_con), nxt_count(_nxt_count), pre(_pre) { ; }
};

struct node {
	ull board_num;  //用一个数记录具体的棋盘情况
	step* this_step;     //用来记录步骤
	node(ull _b = 0, step* _this = NULL) :board_num(_b), this_step(_this) { ; }
};

map<ull, node> map_state[2];

//放置用于迭代的可能状态

inline bool valid(int x, int y)
{
	return (2 <= x && x <= 4 && 0 <= y && y <= 6) || (2 <= y && y <= 4 && 0 <= x && x <= 6);
}

inline bool can_move(int x,int y,int xx,int yy, int board[][7])
{
	return board[x][y] && board[(x + xx) >> 1][(y + yy) >> 1] && !board[xx][yy];
}

inline bool sym(int x)
{
	return ((x >> 1) & 0x3) == ((x >> 3) & 0x3);
}

ull hash_produce(ull hash_ans)
{
	//ull hash_ans = hash_boar;
	
	//进制转换为可能出问题的地方
	int block[4];  //存大的块
	for (int i = 0; i < 4; ++i) {
		//上、左、右、下：0，1，2，3
		block[i] = (hash_ans >> (7 * (3 - i))) & 0x7f;
	}

	int piece[5];  //存小的块
	for (int i = 0; i < 5; ++i) {
		//中、左上、右上、右下、左下
		piece[i] = (hash_ans >> (32 - i)) & 1;
	}

	int min = block[0];
	int min_loca = 0;
	for (int i = 1; i < 4; ++i) {
		if (block[i] < min) {
			min = block[i];
			min_loca = i;
		}
	}
	
	int tmp;
	switch (min_loca)
	{
	case UP:break;
	case LEFT:
		//blocks
		tmp = block[LEFT];//左
		block[LEFT] = block[DOWN]; block[DOWN] = block[RIGHT]; block[RIGHT] = block[UP]; block[UP] = tmp;
		//pieces
		tmp = piece[L_UP];
		piece[L_UP] = piece[L_DOWN]; piece[L_DOWN] = piece[R_DOWN]; piece[R_DOWN] = piece[R_UP]; piece[R_UP] = tmp;
		break;
	case RIGHT:
		tmp = block[LEFT];//左
		block[LEFT] = block[UP]; block[UP] = block[RIGHT]; block[RIGHT] = block[DOWN]; block[DOWN] = tmp;
		tmp = piece[L_UP];
		piece[L_UP] = piece[R_UP]; piece[R_UP] = piece[R_DOWN]; piece[R_DOWN] = piece[L_DOWN]; piece[L_DOWN] = tmp;
		break;
	case DOWN:
		swap(block[UP], block[DOWN]);
		swap(block[LEFT], block[RIGHT]);
		swap(piece[L_UP], piece[R_DOWN]);
		swap(piece[L_DOWN], piece[R_UP]);
		break;
	}

	if (sym(block[UP]) && sym(block[DOWN]) && block[LEFT] > block[RIGHT]) {
		swap(block[LEFT], block[RIGHT]);
		swap(piece[L_UP], piece[R_UP]);
		swap(piece[L_DOWN], piece[R_DOWN]);
	}

	hash_ans = 0;
	for (int i = 0; i < 5; ++i) {
		hash_ans |= (ull)piece[i] << (32 - i);
	}
	for (int i = 0; i < 4; ++i) {
		hash_ans |= (ull)block[i] << (7 * (3 - i));
	}
	return hash_ans;
}

void input()
{
	//从这里开始改
	ull chess;
	ull board_condition = 0;
	cout << "孔明棋解法生成器：" << "\n";
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			cin >> chess;
			if (valid(i, j)) {
				board_condition |= (chess << wei_log2[i][j]);
				if (chess) {
					++mem_of_chess;
				}
			}
		}
	}
	ull hash_initial = hash_produce(board_condition);
	step* tmp = new step(255, 0, NULL);
	map_state[0][hash_initial] = node(board_condition, tmp);
}

void Del(step* aim)
{
	//delete aim;
	if (aim->pre != NULL) {
		--aim->pre->nxt_count;
		if (aim->pre->nxt_count == 0) {
			Del(aim->pre);
		}
	}
	delete aim;
}

int work()
{
	//这是用来确定哪个map是正在工作的，哪个是要往里存数据的
	int work_on = 0;
	int stor = 1;
	//遍历map用到的迭代器
	map<ull, node>::iterator itr;
	map<ull, node>::iterator itr2;
	//记录解码以后的棋盘情况
	int board[7][7] = { 0 };

	//判断什么时候停下
	//bool flag = false;
	int k;
	bool change_state = false;
	for (k = 0; k < mem_of_chess - 1; ++k) {
		cout << "k = " << k << ";\n";
		
		for (itr = map_state[work_on].begin(); itr != map_state[work_on].end(); ++itr) {

			if (change_state) {
				//此时已经找到答案，进入回收内存的阶段
				Del((itr->second).this_step);
			}
			else {
				//刷新棋盘
				ull board_num = (itr->second).board_num;
				for (int i = 0; i < 7; ++i) {
					for (int j = 0; j < 7; ++j) {
						if (valid(i, j)) {
							board[i][j] = int((board_num >> wei_log2[i][j]) & (ull)1);
						}
					}
				}

				//该节点的地址
				step* origin = (itr->second).this_step;
				//开始枚举情况
				int xx, yy;
				ull new_board_num;
				ull new_hash_num;
				//用于删除
				bool use_flag = false;
				//用于防止内存泄漏
				bool stop_flag = false;
				for (int i = 0; i < 7; ++i) {
					for (int j = 0; j < 7; ++j) {
						//检查是否真的用到这一路径
						if (valid(i, j)) {
							for (int dir = 0; dir < 4; ++dir) {
								xx = i + dx[dir];
								yy = j + dy[dir];

								if (valid(xx, yy)) {
									if (can_move(i, j, xx, yy, board)) {
										//do sth
										new_board_num = board_num - (ull(1) << wei_log2[i][j]) - (ull(1) << wei_log2[(i + xx) >> 1][(j + yy) >> 1]) + (ull(1) << wei_log2[xx][yy]);
										new_hash_num = hash_produce(new_board_num);

										//存储新的情况。
										step* tmp = new step(((i << 5) + (j << 2) + dir), 0, origin);
										bool success_insert = (map_state[stor].insert(pair<ull, node>(new_hash_num, node(new_board_num, tmp)))).second;
										if (success_insert) {
											use_flag = true;
											++origin->nxt_count;

											//当找到最后的答案之后，开始清理其余剩下的棋子
											if (new_hash_num == ((ull)1 << wei_log2[3][3])) {
												for (itr2 = map_state[stor].begin(); itr2 != map_state[stor].end(); ++itr2) {
													if (itr2->first != ((ull)1 << wei_log2[3][3])) {
														Del((itr2->second).this_step);
													}
												}
												stop_flag = true;
												change_state = true;
												break;
											}
											//if (stop_flag) break;
										}
										else {
											delete tmp;
										}
									}
								}
							}
							if (stop_flag) break;
						}
					}
					if (stop_flag) break;
				}
				if (!use_flag) {
					Del(origin);
				}
				//if (flag) break;
			}
		}

		//if (flag) break;
		//清空map以节省内存
		map_state[work_on].clear();
		//确定每次的工作map和用来存结果的map
		std::swap(work_on, stor);
	}

	//return k;
	return work_on;
}

void Forward_out(step* out)
{
	if (out->pre == NULL) {
		delete out;
		return;
	}
	//start here
	else {
		Forward_out(out->pre);
		int x, y, dir;
		x = ((out->context) >> 5) & 0x7;
		y = ((out->context) >> 2) & 0x7;
		dir = ((out->context)) & 0x3;
		cout << "(" << x << "," << y << ")"
			<< "->(" << x + dx[dir] << "," << y + dy[dir] << ")\n";
		delete out;
	}
}

void output(int loca)
{
	//map<ull, node>::iterator out = map_state[loca].find((ull)1 << wei_log2[3][3]);
	step* out = (map_state[loca].find((ull)1 << wei_log2[3][3])->second).this_step;
	Forward_out(out);
}

int main()
{
	input();

	int loca = work();
	
	output(loca);
 	
	system("pause");
	return 0;
}
