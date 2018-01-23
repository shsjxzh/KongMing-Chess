#include <iostream>
#include <map>
#include <algorithm>
//#include <utility>
using namespace std;

typedef unsigned long long ull;
typedef unsigned char uc;
//typedef map<ull, node> MAP;
//�洢��ʼ�������
int mem_of_chess = 0;

//������
//int node_count = 0;

//�����洢������ÿ����Ȩֵ��2ȡ����
//��Ҫ�����޸�Ȩֵ
const int wei_log2[7][7] = {
	{  0, 0,25,27,23, 0, 0 },
	{  0, 0,24,26,22, 0, 0 },
	{ 16,15,31,21,30,10,11 },
	{ 20,19,14,32, 7,12,13 },
	{ 18,17,28, 0,29, 8, 9 },
	{  0, 0, 1, 5, 3, 0, 0 },
	{  0, 0, 2, 6, 4, 0, 0 }
};

//�ڼ���hash��ʱ�򷽱�
//�ϡ����ҡ��£�0��1��2��3
const int UP = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int DOWN = 3;

const int MID = 0;
const int L_UP = 1;
const int R_UP = 2;
const int R_DOWN = 3;
const int L_DOWN = 4;


//�ƶ���ʽ
const int dx[4] = { -2,2,0,0 };
const int dy[4] = { 0,0,-2,2 };

struct step {
	//����Ҳ��
	step* pre;	   //ǰһ��
	uc context;    //ÿһ���ľ�������
	uc nxt_count;   //���滹ʣ���ٸ���֧
	step(uc _con = 255, uc _nxt_count = 0, step* _pre = NULL) :context(_con), nxt_count(_nxt_count), pre(_pre) { ; }
};

struct node {
	ull board_num;  //��һ������¼������������
	step* this_step;     //������¼����
	node(ull _b = 0, step* _this = NULL) :board_num(_b), this_step(_this) { ; }
};

map<ull, node> map_state[2];

//�������ڵ����Ŀ���״̬

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
	
	//����ת��Ϊ���ܳ�����ĵط�
	int block[4];  //���Ŀ�
	for (int i = 0; i < 4; ++i) {
		//�ϡ����ҡ��£�0��1��2��3
		block[i] = (hash_ans >> (7 * (3 - i))) & 0x7f;
	}

	int piece[5];  //��С�Ŀ�
	for (int i = 0; i < 5; ++i) {
		//�С����ϡ����ϡ����¡�����
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
		tmp = block[LEFT];//��
		block[LEFT] = block[DOWN]; block[DOWN] = block[RIGHT]; block[RIGHT] = block[UP]; block[UP] = tmp;
		//pieces
		tmp = piece[L_UP];
		piece[L_UP] = piece[L_DOWN]; piece[L_DOWN] = piece[R_DOWN]; piece[R_DOWN] = piece[R_UP]; piece[R_UP] = tmp;
		break;
	case RIGHT:
		tmp = block[LEFT];//��
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
	//�����￪ʼ��
	ull chess;
	ull board_condition = 0;
	cout << "������ⷨ��������" << "\n";
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
	//��������ȷ���ĸ�map�����ڹ����ģ��ĸ���Ҫ��������ݵ�
	int work_on = 0;
	int stor = 1;
	//����map�õ��ĵ�����
	map<ull, node>::iterator itr;
	map<ull, node>::iterator itr2;
	//��¼�����Ժ���������
	int board[7][7] = { 0 };

	//�ж�ʲôʱ��ͣ��
	//bool flag = false;
	int k;
	bool change_state = false;
	for (k = 0; k < mem_of_chess - 1; ++k) {
		cout << "k = " << k << ";\n";
		
		for (itr = map_state[work_on].begin(); itr != map_state[work_on].end(); ++itr) {

			if (change_state) {
				//��ʱ�Ѿ��ҵ��𰸣���������ڴ�Ľ׶�
				Del((itr->second).this_step);
			}
			else {
				//ˢ������
				ull board_num = (itr->second).board_num;
				for (int i = 0; i < 7; ++i) {
					for (int j = 0; j < 7; ++j) {
						if (valid(i, j)) {
							board[i][j] = int((board_num >> wei_log2[i][j]) & (ull)1);
						}
					}
				}

				//�ýڵ�ĵ�ַ
				step* origin = (itr->second).this_step;
				//��ʼö�����
				int xx, yy;
				ull new_board_num;
				ull new_hash_num;
				//����ɾ��
				bool use_flag = false;
				//���ڷ�ֹ�ڴ�й©
				bool stop_flag = false;
				for (int i = 0; i < 7; ++i) {
					for (int j = 0; j < 7; ++j) {
						//����Ƿ�����õ���һ·��
						if (valid(i, j)) {
							for (int dir = 0; dir < 4; ++dir) {
								xx = i + dx[dir];
								yy = j + dy[dir];

								if (valid(xx, yy)) {
									if (can_move(i, j, xx, yy, board)) {
										//do sth
										new_board_num = board_num - (ull(1) << wei_log2[i][j]) - (ull(1) << wei_log2[(i + xx) >> 1][(j + yy) >> 1]) + (ull(1) << wei_log2[xx][yy]);
										new_hash_num = hash_produce(new_board_num);

										//�洢�µ������
										step* tmp = new step(((i << 5) + (j << 2) + dir), 0, origin);
										bool success_insert = (map_state[stor].insert(pair<ull, node>(new_hash_num, node(new_board_num, tmp)))).second;
										if (success_insert) {
											use_flag = true;
											++origin->nxt_count;

											//���ҵ����Ĵ�֮�󣬿�ʼ��������ʣ�µ�����
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
		//���map�Խ�ʡ�ڴ�
		map_state[work_on].clear();
		//ȷ��ÿ�εĹ���map������������map
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
