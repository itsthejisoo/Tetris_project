#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <conio.h> 
#include <time.h>
#include <stdlib.h>


#define LEFT 75   //←  
#define RIGHT 77  //→
#define UP 72     //회전
#define DOWN 80   //soft drop
#define SPACE 32  //hard drop
#define p 112     //일시정지
#define P 80      //일시정지
#define ESC 27    //게임종료

#define false 0
#define true 1 

#define ACTIVE_BLOCK -2 
#define CEILLING -1
#define EMPTY 0
#define WALL 1
#define INACTIVE_BLOCK 2

#define MAIN_X 11
#define MAIN_Y 23
#define MAIN_X_ADJ 3
#define MAIN_Y_ADJ 1

#define STATUS_X MAIN_X_ADJ+MAIN_X+1

int STATUS_Y_GOAL; //goal 정보 위치 y좌표 저장
int STATUS_Y_LEVEL; //레벨 정보 위치 y좌표 저장
int STATUS_Y_SCORE; //점수 정보 위치 y좌표 저장

int blocks[7][4][4][4] = {
	{
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		}
	},
	{
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{1,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},
	{
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	}
}; //블록 종류

int b_type;
int b_rotation;
int b_type_next;

int main_frontbuffer[MAIN_Y][MAIN_X];
int main_backbuffer[MAIN_Y][MAIN_X];

int bx, by; //이동중인 블록의 게임판상의 x,y좌표를 저장
int key;
int speed; //게임진행속도
int level; //현재 레벨
int level_goal; //다음레벨로 넘어가기 위한 목표점수
int cnt; //현재 레벨에서 제거한 줄 수를 저장
int score; //현재 점수
int last_score = 0; //마지막게임점수
int best_score = 0; //최고득점점수

int new_block_on = 0;
int crush_on = 0;
int level_up_on = 0;
int space_key_on = 0;

void title(void); //타이틀 화면 
void reset(void);
void reset_main(void);
void reset_main_backbuffer(void);
void draw_map(void);
void draw_main(void); //게임판을 그림
void new_block(void); //새로운 블록을 하나 만듦
void check_key(void); //키 입력받음
void drop_block(void); //블록을 아래로 떨어트림
int check_crush(int bx, int by, int rotation);
void move_block(int dir);
void check_line(void); //줄이 가득찼는지를 판단하고 지움
void check_level_up(void); //레벨목표 달성 확인 후 레벨업
void check_game_over(void); //게임오버
void pause(void);//게임 일시정지

void gotoxy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView(char show) {
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}//커서 숨기기

int main() {
	int i;

	srand((unsigned)time(NULL)); //난수표 생성
	CursorView(0); //커서 없앰 

	title();
	reset();

	while (1) {
		for (i = 0; i < 5; i++) {
			check_key();
			draw_main();
			Sleep(speed);
			if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
			if (space_key_on == 1) {
				space_key_on = 0;
				break;
			}
		}
		drop_block();
		check_level_up(); //레벨업 체크 
		check_game_over(); //게임오버 체크 
		if (new_block_on == 1) new_block();
	}
}

void title(void) {
	int x = 5;
	int y = 4;
	int cnt;

	gotoxy(x + 5, y + 2); printf("T E T R I S"); Sleep(100);
	gotoxy(x, y + 7); printf(" ENTER : 게임시작"); Sleep(100);
	gotoxy(x, y + 9); printf("  ↑  : Rotaion");
	gotoxy(x, y + 10); printf(" ←/ → : Left / Right");
	gotoxy(x, y + 11); printf("  ↓   : Soft Drop");
	gotoxy(x, y + 8); printf(" P     : 게임 멈추기"); Sleep(100);
	gotoxy(x, y + 9); printf(" ESC   : 끝내기 "); Sleep(100);

	for (cnt = 0;; cnt++) {
		if (_kbhit()) break;
		Sleep(10);
	}

	while (_kbhit()) _getch(); //버퍼에 기록된 키값을 버림 
} //시작화면

void reset(void) {
	FILE* file = fopen("score.dat", "rt"); //score.dat파일을 연결 
	if (file == 0) { best_score = 0; }
	else {
		fscanf(file, "%d", &best_score); //파일이 열리면 최고점수 불러옴 
		fclose(file); //파일 닫기 
	}

	level = 1; //각종변수를 초기화
	score = 0;
	level_goal = 1000;
	key = 0;
	cnt = 0;
	speed = 100;

	system("cls");
	reset_main(); //main_org를 초기화 
	draw_map(); //게임화면을 그림 
	draw_main(); //게임판을 그림 
	b_type_next = rand() % 7; //다음번에 나올 블록 종류를 랜덤하게 생성 
	new_block(); //새로운 블록을 하나 만듦  
}

void reset_main(void) { //게임판 초기화 
	int i, j;


	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			main_frontbuffer[i][j] = 0;
			main_backbuffer[i][j] = 100;
		}
	}
	for (j = 1; j < MAIN_X; j++) //y값이 3인 위치에 천장을 만듬 
		main_frontbuffer[3][j] = CEILLING;


	for (i = 1; i < MAIN_Y - 1; i++) { //좌우 벽을 만듦
		main_frontbuffer[i][0] = WALL;
		main_frontbuffer[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j < MAIN_X; j++) { //바닥벽 
		main_frontbuffer[MAIN_Y - 1][j] = WALL;
	}
}

void draw_map(void) {
	int y = 3;

	gotoxy(STATUS_X, STATUS_Y_LEVEL = y); printf("LEVEL : %5d", cnt);
	gotoxy(STATUS_X, STATUS_Y_GOAL = y + 1); printf("GOAL : %5d", 10 - cnt);
	gotoxy(STATUS_X, y + 2); printf("+-  N E X T  -+ ");
	gotoxy(STATUS_X, y + 3); printf("|             | ");
	gotoxy(STATUS_X, y + 4); printf("|             | ");
	gotoxy(STATUS_X, y + 5); printf("|             | ");
	gotoxy(STATUS_X, y + 6); printf("|             | ");
	gotoxy(STATUS_X, y + 7); printf("+-- -  -  - --+ ");
	gotoxy(STATUS_X, y + 8); printf(" YOUR SCORE :");
	gotoxy(STATUS_X, STATUS_Y_SCORE = y + 9); printf("        %6d", score);
	gotoxy(STATUS_X, y + 10); printf(" LAST SCORE :");
	gotoxy(STATUS_X, y + 11); printf("        %6d", last_score);
	gotoxy(STATUS_X, y + 12); printf(" BEST SCORE :");
	gotoxy(STATUS_X, y + 13); printf("        %6d", best_score);
	gotoxy(STATUS_X, y + 15); printf("   ↑   : Rotation   SPACE : Hard Drop");
	gotoxy(STATUS_X, y + 16); printf(" ←  → : Left / Right   P   : Pause");
	gotoxy(STATUS_X, y + 17); printf("   ↓   : Soft Drop     ESC  : Quit");
} //게임 상태 표시를 나타내는 함수 


void draw_main(void) { //게임판 그리는 함수
	int i, j;

	for (j = 1; j < MAIN_X - 1; j++) {
		if (main_frontbuffer[3][j] == EMPTY) main_frontbuffer[3][j] = CEILLING;
	}

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			if (main_backbuffer[i][j] != main_frontbuffer[i][j]) {

				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_frontbuffer[i][j]) {
				case EMPTY: //빈칸모양 
					printf("  ");
					break;
				case CEILLING: //천장모양 
					printf(". ");
					break;
				case WALL: //벽모양 
					printf("▣");
					break;
				case INACTIVE_BLOCK: //굳은 블럭 모양  
					printf("□");
					break;
				case ACTIVE_BLOCK: //움직이고있는 블럭 모양  
					printf("■");
					break;
				}
			}
		}
	}
	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			main_backbuffer[i][j] = main_frontbuffer[i][j];
		}
	}
} //게임판을 그린 후 main_backbuffer에 복사

void new_block(void) {
	int i, j;

	bx = (MAIN_X / 2) - 1; //블록 생성 위치x좌표(게임판의 가운데) 
	by = 0;  //블록 생성위치 y좌표(제일 위) 
	b_type = b_type_next; //다음블럭값을 가져옴 
	b_type_next = rand() % 7; //다음 블럭을 만듦 
	b_rotation = 0;

	new_block_on = 0;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j] = ACTIVE_BLOCK;
		}
	} //게임판 bx, by위치에 블럭생성

	for (i = 1; i < 3; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X + 2 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(STATUS_X + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}//새로운 블록 생성

void drop_block(void) {
	int i, j;

	if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
	if (crush_on && check_crush(bx, by + 1, b_rotation) == false) {
		for (i = 0; i < MAIN_Y; i++) {
			for (j = 0; j < MAIN_X; j++) {
				if (main_frontbuffer[i][j] == ACTIVE_BLOCK) main_frontbuffer[i][j] = INACTIVE_BLOCK;
			}
		} //현재 조작중인 블럭을 굳힘
		crush_on = 0;
		check_line(); //라인체크를 함 
		new_block_on = 1;
		return; //함수 종료 
	}
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;
}

void check_key(void) {
	key = 0;

	if (_kbhit()) {
		key = _getch();
    
		if (key == 224) {
			do { key = _getch(); } while (key == 224);
			switch (key) {
			case LEFT: //왼쪽키 눌렀을때  
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case RIGHT: //오른쪽 방향키 눌렀을때 
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case DOWN: //아래쪽 방향키 눌렀을때 
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case UP: //위쪽 방향키 눌렀을때 
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
			}
		}
    
		else {
			switch (key) {
			case SPACE: //스페이스키 눌렀을때 
				space_key_on = 1;
          
				while (crush_on == 0) {
					drop_block();
					score += level; //hard drop 점수 보너스
					gotoxy(STATUS_X, STATUS_Y_SCORE); printf("        %6d", score); //점수 표시  
				}
				break;
			case P: //P(대문자) 눌렀을때 
			case p: //p(소문자) 눌렀을때 
				pause(); //일시정지 
				break;
			case ESC: //ESC눌렀을때 
				system("cls");
				exit(0); //게임종료 
			}
		}
	}
	while (_kbhit()) _getch();
}

void check_line(void) {
	int i, j, k, l;
	int block_amount;
	int combo = 0;

	for (i = MAIN_Y - 2; i > 3;) {
		block_amount = 0;
		for (j = 1; j < MAIN_X - 1; j++) {
			if (main_frontbuffer[i][j] > 0) block_amount++;
		}

		if (block_amount == MAIN_X - 2) {
			if (level_up_on == 0) {
				score += 100 * level;
				cnt++;
				combo++;
			}
      
			for (k = i; k > 1; k--) {
				for (l = 1; l < MAIN_X - 1; l++) {
					if (main_frontbuffer[k - 1][l] != CEILLING) main_frontbuffer[k][l] = main_frontbuffer[k - 1][l];
					if (main_frontbuffer[k - 1][l] == CEILLING) main_frontbuffer[k][l] = EMPTY;
				}
			}
		} //블록이 가득 찬 경우
		else i--;
	}
  
	if (combo) {
		if (combo > 1) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
			Sleep(500);
			score += (combo * level * 100);
			reset_main_backbuffer();
		}
		gotoxy(STATUS_X, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
		gotoxy(STATUS_X, STATUS_Y_SCORE); printf("        %6d", score);
	}
}

int check_crush(int bx, int by, int b_rotation) {
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1 && main_frontbuffer[by + i][bx + j] > 0) return false;
		}
	}
	return true;
};

void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT: //왼쪽방향 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--;
		break;

	case RIGHT: //오른쪽 방향
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;

	case DOWN: //아래쪽 방향
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;

	case UP: //위쪽 방향키 눌렀을때 회전시킴 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 100:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_frontbuffer[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;
	}
} //블록을 이동시킴

void check_level_up(void) {
	int i, j;

	if (cnt >= 10) {
		draw_main();
		level_up_on = 1;
		level += 1;
		cnt = 0;

		for (i = 0; i < 4; i++) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("             ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("             ");
			Sleep(200);

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("LEVEL UP!");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("SPEED UP!");
			Sleep(200);
		}
		reset_main_backbuffer();

		for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) {
			for (j = 1; j < MAIN_X - 1; j++) {
				main_frontbuffer[i][j] = INACTIVE_BLOCK;
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				printf("★");
				Sleep(20);
			}
		}
		Sleep(100);
		check_line();

		switch (level) {
		case 2:
			speed = 80;
			break;
		case 3:
			speed = 70;
			break;
		case 4:
			speed = 60;
			break;
		case 5:
			speed = 25;
			break;
		case 6:
			speed = 20;
			break;
		case 7:
			speed = 20;
			break;
		case 8:
			speed = 15;
			break;
		case 9:
			speed = 10;
			break;
		case 10:
			speed = 5;
			break;
		}
		level_up_on = 0;

		gotoxy(STATUS_X, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //레벨
		gotoxy(STATUS_X, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); //레벨목표

	}
}

void reset_main_backbuffer(void) {
	int i, j;

	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			main_backbuffer[i][j] = 100;
		}
	}
}

void check_game_over(void) {
	int i;
	int x = 5;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++) {
		if (main_frontbuffer[3][i] > 0) {
			gotoxy(x, y + 2); printf("  +-----------------------+   ");
			gotoxy(x, y + 3); printf("  |  G A M E  O V E R..   |   ");
			gotoxy(x, y + 4); printf("  +-----------------------+   ");
			gotoxy(x, y + 5); printf("   YOUR SCORE: %6d         ", score);
			gotoxy(x, y + 7); printf("\n  Press any key to restart..  ");
			last_score = score;

			if (score > best_score) {
				FILE* file = fopen("score.dat", "wt");

				gotoxy(x, y + 6); printf("  ★★★ BEST SCORE! ★★★   ");

				if (file == 0) {
					gotoxy(0, 0);
					printf("---------");
				} //기록 없을때
				else {
					fprintf(file, "%d", score);
					fclose(file);
				}
			} //최고기록 갱신
			Sleep(1000);
			while (_kbhit()) _getch();
			key = _getch();
			reset();

		}
	}
}

void pause(void) {
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++) {
		gotoxy(x, y + 2); printf("  +-----------------------+   ");
		gotoxy(x, y + 3); printf("  |       P A U S E       |   ");
		gotoxy(x, y + 4); printf("  +-----------------------+   ");
		gotoxy(x, y + 5); printf("  Press any key to resume..   ");
	}
	_getch();

	system("cls");
	reset_main_backbuffer();
	draw_main();
	draw_map();

	for (i = 1; i < 3; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("  ");
			}
		}
	}
} //게임 일시정지 함수
