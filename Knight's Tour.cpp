#include"stdafx.h"
#include "stdlib.h"
#include <string> 
#include <sys/timeb.h>
using namespace std;

struct location
{
	int x;
	int y;
};



enum boolean
{
	TRUE = 1,
	FALSE = 0
};

const int MAX = 8;
const int TableWidth = 7;
class KNIGHT {
public:
	KNIGHT(int width); //设置初始状态 
	void print();    //打印问题的解  
	boolean travel(int start_x, int start_y);    //根据马的起始位置(start_x, start_y)使用回溯算法求骑士游历问题的一个解 
protected:
	void init_direction();   //初始化记录所选方向的数组，将每个值置为MAX  
	void init_chessboard();   //初始化记录马在第几步到达每个位置的数组，将每个值置为0 
	void set_start(int x, int y);   // 设置初始状态，包括初始化方向数组和棋盘数组，并设置马的初始位置
	virtual boolean select_direction();  //在当前位置选择一个方向以推进到下一位置 
	virtual void backward();   //从当前位置回溯到上一位置 
	virtual void forward(); //从当前位置推进到下一位置 
	boolean is_legal(int x, int y);  //判断马是否能够走向位置(x, y)。 
	boolean back_to_start();  //判断是否回溯到初始状态 
	boolean is_end();  //判断是否游历完所有位置 
					   // 下面两个数组用来记住选择某个方向后，推进到下一位置时x方向和y方向的值的变化 
	int var_x[MAX];
	int var_y[MAX];
	int chessboard[TableWidth][TableWidth]; // 记录马第几步到达某个位置的棋盘数组 
	int direction[TableWidth][TableWidth];  // 记录马在某个位置是在上一位置选择第几个方向到达的 
	int width; // 棋盘宽度 
	int curr_x, curr_y; // 马的当前位置 
	int step; // 已经游历的步数 
	int last_direction; // 上一位置到当前位置所选的方向 
};


class FAST_KNIGHT : public KNIGHT
{
public:
	FAST_KNIGHT(int width);

protected:

	void init_cannot();   //初始化cannot数组
	virtual boolean select_direction();    //在当前位置根据启发式规则选择一个方向以推进到下一位置
	virtual void backward();   //从当前位置回溯到上一位置，注意维持cannot数组
	virtual void forward();  //从当前位置根据所选择的方向推进到下一位置
	boolean cannot[TableWidth][TableWidth][MAX];    // 用来记住某个位置某个方向是否已经试探过
};
KNIGHT::KNIGHT(int width)
{
	this->width = width;
	init_direction();
	int total_step = 0;
}

boolean KNIGHT::is_legal(int x, int y)
{
	if (x < 0 || x >= width) return FALSE;
	if (y < 0 || y >= width) return FALSE;
	if (chessboard[x][y] > 0) return FALSE;
	return TRUE;
}

boolean KNIGHT::back_to_start()
{
	if (step == 1) return TRUE;
	else return FALSE;
}

boolean KNIGHT::is_end()
{
	if (step > width * width) return TRUE;
	else return FALSE;
}

void KNIGHT::set_start(int x, int y)
{
	curr_x = x;
	curr_y = y;
	step = 1;
	chessboard[x][y] = step;
	step = step + 1;
	direction[x][y] = MAX;
	last_direction = MAX;
}

boolean  KNIGHT::select_direction()
{
	int try_x, try_y;
	// last_direction为MAX表示当前位置是一个新的位置，在新推进到某个位置(curr_x, curr_y)时， 
	// direction[curr_x][curr_y]会记录上一位置到(curr_x, curr_y)时所选择的方向，这时 
	// last_direction置为MAX用来标记该位置是新推进的位置。 
	if (last_direction == MAX)
		last_direction = 0;
	else
		last_direction = last_direction + 1;
	while (last_direction < MAX)
	{
		// 检测下一步推进到哪个位置是合法的，若合法则选择该方向。 
		try_x = curr_x + var_x[last_direction];
		try_y = curr_y + var_y[last_direction];
		if (is_legal(try_x, try_y)) break;
		last_direction = last_direction + 1;
	}
	if (last_direction == MAX) return FALSE;
	else return TRUE;
}

void KNIGHT::backward()
{
	step = step - 1;
	chessboard[curr_x][curr_y] = 0;
	// 将last_direction置为上一位置到(curr_x, curr_y)所选择的方向 
	last_direction = direction[curr_x][curr_y];
	// 根据这个方向回溯到上一位置，同时回溯到上一位置之后，在上一位置再试探时应该从 last_direction+1的方向开始。
	curr_x = curr_x - var_x[last_direction];
	curr_y = curr_y - var_y[last_direction];
}

void KNIGHT::forward()
{
	// 在推进时last_direction是当前位置所选择的方向 
	curr_x = curr_x + var_x[last_direction];
	curr_y = curr_y + var_y[last_direction];
	chessboard[curr_x][curr_y] = step;
	step = step + 1;
	direction[curr_x][curr_y] = last_direction;
	last_direction = MAX;   // last_direction的值已经被记录，这时置为MAX表示当前位置为新推进的位置 
}

boolean  KNIGHT::travel(int start_x, int start_y)
{
	init_chessboard();
	set_start(start_x, start_y);
	do
	{
		if (select_direction())
			forward();
		else
			backward();
	} while (!back_to_start() && !is_end());
	if (back_to_start())
		return FALSE;
	else
		return TRUE;
}

void KNIGHT::init_direction()
{
	var_x[0] = 2; var_y[0] = 1;
	var_x[1] = 1; var_y[1] = 2;
	var_x[2] = -1; var_y[2] = 2;
	var_x[3] = -2; var_y[3] = 1;
	var_x[4] = -2; var_y[4] = -1;
	var_x[5] = -1; var_y[5] = -2;
	var_x[6] = 1; var_y[6] = -2;
	var_x[7] = 2; var_y[7] = -1;
}

void KNIGHT::init_chessboard()
{
	int x, y;
	for (x = 0; x <width; x++)
	{
		for (y = 0; y <width; y++)
		{
			chessboard[x][y] = 0;
		}
	}
}

void KNIGHT::print()
{
	int x, y, count;
	const int Stepcount = TableWidth*TableWidth;
	count = 0;
	//定义数组  
	location Steps[Stepcount];
	printf("解  法\t");
	fflush(stdout);
	for (y = 0; y < TableWidth; y++) { printf("第%d列\t", y + 1); fflush(stdout);; }
	printf("\n");
	for (x = 0; x < width; x++)
	{
		printf("第%d行\t", x + 1);
		fflush(stdout);
		for (y = 0; y < TableWidth; y++)
		{
			Steps[chessboard[x][y]].x = x + 1;
			Steps[chessboard[x][y]].y = y + 1;
			printf("%d\t", chessboard[x][y]);
			fflush(stdout);
		}
		printf("\n\n");
	}

	printf("以下是详细步骤：\n");
	for (x = 0; x < Stepcount; x++)
	{
		printf("第%d步：%d行 %d列\n", x + 1, Steps[x + 1].x, Steps[x + 1].y);
	}
	//delete []Steps;
}


FAST_KNIGHT::FAST_KNIGHT(int width) : KNIGHT(width)
{
	init_cannot();
}

void FAST_KNIGHT::init_cannot()
{
	int x, y, dir;

	for (x = 0; x < width; x = x + 1)
		for (y = 0; y < width; y = y + 1)
			for (dir = 0; dir < width; dir = dir + 1) cannot[x][y][dir] = FALSE;
}

boolean  FAST_KNIGHT::select_direction()
{
	int try_x, try_y, next_x, next_y;
	int dir_index, next_index;
	int min_dir, count;

	min_dir = MAX; last_direction = MAX;
	for (dir_index = 0; dir_index < MAX; dir_index = dir_index + 1) {
		// 选择一个方向，使得根据该方向推进到下一位置时，在该位置可选的方向最少
		try_x = curr_x + var_x[dir_index];
		try_y = curr_y + var_y[dir_index];

		if (is_legal(try_x, try_y) && !cannot[curr_x][curr_y][dir_index]) {
			// 这个位置作为下一位置是合法，那么计算该位置可选的方向
			count = 0;
			for (next_index = 0; next_index < MAX; next_index++) {
				next_x = try_x + var_x[next_index];
				next_y = try_y + var_y[next_index];
				if (is_legal(next_x, next_y)) count = count + 1;
			}
			if (count < min_dir) {
				// 记录具有最少可选方向的下一位置
				last_direction = dir_index;
				min_dir = count;
			}
		}
	}

	if (last_direction == MAX) return FALSE;
	else return TRUE;
}

void FAST_KNIGHT::backward()
{
	int dir;

	step = step - 1;
	chessboard[curr_x][curr_y] = 0;

	// 从位置(curr_x, curr_y)回溯，那么下一次再到达该位置时所有方向都需要重新试探
	for (dir = 0; dir < MAX; dir = dir + 1) cannot[curr_x][curr_y][dir] = FALSE;

	last_direction = direction[curr_x][curr_y];
	curr_x = curr_x - var_x[last_direction];
	curr_y = curr_y - var_y[last_direction];
}

void FAST_KNIGHT::forward()
{
	// 该位置的这个方向已经试探过了
	cannot[curr_x][curr_y][last_direction] = TRUE;

	curr_x = curr_x + var_x[last_direction];
	curr_y = curr_y + var_y[last_direction];

	chessboard[curr_x][curr_y] = step;
	step = step + 1;
	direction[curr_x][curr_y] = last_direction;
	last_direction = MAX;
}

void main()
{
	char *cmd = "";
	do {
		FAST_KNIGHT knight(TableWidth);
		int x, y;
		x = 1;
		y = 1;
		printf("请输入起始坐标（x，y）:\n");
		rewind(stdin);
		scanf_s("%d,%d", &x, &y);
		struct timeb startTime, endTime;
		printf("开始计算.........\n");
		ftime(&startTime);
		if (knight.travel(x - 1, y - 1))
		{
			printf("一个结果为：\n");
			knight.print();

		}
		else
		{
			printf("\n无解\n");
			printf("起始坐标为（%d，%d）:\n", x, y);
			printf("棋盘宽度为%d:\n", TableWidth);
		}
		ftime(&endTime);
		printf("计算结束，总共耗时：%ld毫秒\n", (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
		printf("输入c继续下一轮运算，输入q退出\n");
		rewind(stdin);
		char *cmd = "";
		fflush(stdin);
		scanf_s("%c", &cmd);
	} while (cmd == "q");
	return;
}

