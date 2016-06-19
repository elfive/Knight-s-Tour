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
	KNIGHT(int width); //���ó�ʼ״̬ 
	void print();    //��ӡ����Ľ�  
	boolean travel(int start_x, int start_y);    //���������ʼλ��(start_x, start_y)ʹ�û����㷨����ʿ���������һ���� 
protected:
	void init_direction();   //��ʼ����¼��ѡ��������飬��ÿ��ֵ��ΪMAX  
	void init_chessboard();   //��ʼ����¼���ڵڼ�������ÿ��λ�õ����飬��ÿ��ֵ��Ϊ0 
	void set_start(int x, int y);   // ���ó�ʼ״̬��������ʼ������������������飬��������ĳ�ʼλ��
	virtual boolean select_direction();  //�ڵ�ǰλ��ѡ��һ���������ƽ�����һλ�� 
	virtual void backward();   //�ӵ�ǰλ�û��ݵ���һλ�� 
	virtual void forward(); //�ӵ�ǰλ���ƽ�����һλ�� 
	boolean is_legal(int x, int y);  //�ж����Ƿ��ܹ�����λ��(x, y)�� 
	boolean back_to_start();  //�ж��Ƿ���ݵ���ʼ״̬ 
	boolean is_end();  //�ж��Ƿ�����������λ�� 
					   // ������������������סѡ��ĳ��������ƽ�����һλ��ʱx�����y�����ֵ�ı仯 
	int var_x[MAX];
	int var_y[MAX];
	int chessboard[TableWidth][TableWidth]; // ��¼��ڼ�������ĳ��λ�õ��������� 
	int direction[TableWidth][TableWidth];  // ��¼����ĳ��λ��������һλ��ѡ��ڼ������򵽴�� 
	int width; // ���̿�� 
	int curr_x, curr_y; // ��ĵ�ǰλ�� 
	int step; // �Ѿ������Ĳ��� 
	int last_direction; // ��һλ�õ���ǰλ����ѡ�ķ��� 
};


class FAST_KNIGHT : public KNIGHT
{
public:
	FAST_KNIGHT(int width);

protected:

	void init_cannot();   //��ʼ��cannot����
	virtual boolean select_direction();    //�ڵ�ǰλ�ø�������ʽ����ѡ��һ���������ƽ�����һλ��
	virtual void backward();   //�ӵ�ǰλ�û��ݵ���һλ�ã�ע��ά��cannot����
	virtual void forward();  //�ӵ�ǰλ�ø�����ѡ��ķ����ƽ�����һλ��
	boolean cannot[TableWidth][TableWidth][MAX];    // ������סĳ��λ��ĳ�������Ƿ��Ѿ���̽��
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
	// last_directionΪMAX��ʾ��ǰλ����һ���µ�λ�ã������ƽ���ĳ��λ��(curr_x, curr_y)ʱ�� 
	// direction[curr_x][curr_y]���¼��һλ�õ�(curr_x, curr_y)ʱ��ѡ��ķ�����ʱ 
	// last_direction��ΪMAX������Ǹ�λ�������ƽ���λ�á� 
	if (last_direction == MAX)
		last_direction = 0;
	else
		last_direction = last_direction + 1;
	while (last_direction < MAX)
	{
		// �����һ���ƽ����ĸ�λ���ǺϷ��ģ����Ϸ���ѡ��÷��� 
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
	// ��last_direction��Ϊ��һλ�õ�(curr_x, curr_y)��ѡ��ķ��� 
	last_direction = direction[curr_x][curr_y];
	// �������������ݵ���һλ�ã�ͬʱ���ݵ���һλ��֮������һλ������̽ʱӦ�ô� last_direction+1�ķ���ʼ��
	curr_x = curr_x - var_x[last_direction];
	curr_y = curr_y - var_y[last_direction];
}

void KNIGHT::forward()
{
	// ���ƽ�ʱlast_direction�ǵ�ǰλ����ѡ��ķ��� 
	curr_x = curr_x + var_x[last_direction];
	curr_y = curr_y + var_y[last_direction];
	chessboard[curr_x][curr_y] = step;
	step = step + 1;
	direction[curr_x][curr_y] = last_direction;
	last_direction = MAX;   // last_direction��ֵ�Ѿ�����¼����ʱ��ΪMAX��ʾ��ǰλ��Ϊ���ƽ���λ�� 
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
	//��������  
	location Steps[Stepcount];
	printf("��  ��\t");
	fflush(stdout);
	for (y = 0; y < TableWidth; y++) { printf("��%d��\t", y + 1); fflush(stdout);; }
	printf("\n");
	for (x = 0; x < width; x++)
	{
		printf("��%d��\t", x + 1);
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

	printf("��������ϸ���裺\n");
	for (x = 0; x < Stepcount; x++)
	{
		printf("��%d����%d�� %d��\n", x + 1, Steps[x + 1].x, Steps[x + 1].y);
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
		// ѡ��һ������ʹ�ø��ݸ÷����ƽ�����һλ��ʱ���ڸ�λ�ÿ�ѡ�ķ�������
		try_x = curr_x + var_x[dir_index];
		try_y = curr_y + var_y[dir_index];

		if (is_legal(try_x, try_y) && !cannot[curr_x][curr_y][dir_index]) {
			// ���λ����Ϊ��һλ���ǺϷ�����ô�����λ�ÿ�ѡ�ķ���
			count = 0;
			for (next_index = 0; next_index < MAX; next_index++) {
				next_x = try_x + var_x[next_index];
				next_y = try_y + var_y[next_index];
				if (is_legal(next_x, next_y)) count = count + 1;
			}
			if (count < min_dir) {
				// ��¼�������ٿ�ѡ�������һλ��
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

	// ��λ��(curr_x, curr_y)���ݣ���ô��һ���ٵ����λ��ʱ���з�����Ҫ������̽
	for (dir = 0; dir < MAX; dir = dir + 1) cannot[curr_x][curr_y][dir] = FALSE;

	last_direction = direction[curr_x][curr_y];
	curr_x = curr_x - var_x[last_direction];
	curr_y = curr_y - var_y[last_direction];
}

void FAST_KNIGHT::forward()
{
	// ��λ�õ���������Ѿ���̽����
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
		printf("��������ʼ���꣨x��y��:\n");
		rewind(stdin);
		scanf_s("%d,%d", &x, &y);
		struct timeb startTime, endTime;
		printf("��ʼ����.........\n");
		ftime(&startTime);
		if (knight.travel(x - 1, y - 1))
		{
			printf("һ�����Ϊ��\n");
			knight.print();

		}
		else
		{
			printf("\n�޽�\n");
			printf("��ʼ����Ϊ��%d��%d��:\n", x, y);
			printf("���̿��Ϊ%d:\n", TableWidth);
		}
		ftime(&endTime);
		printf("����������ܹ���ʱ��%ld����\n", (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
		printf("����c������һ�����㣬����q�˳�\n");
		rewind(stdin);
		char *cmd = "";
		fflush(stdin);
		scanf_s("%c", &cmd);
	} while (cmd == "q");
	return;
}

