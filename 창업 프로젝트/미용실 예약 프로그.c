#define _CRT_SECURE_NO_WARNINGS
#define MAX 200 //회원 최대 인원 수
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;
int member_count = 0;
typedef struct { // 회원 구조체
	char name[20];
	char phone[20];
	char gender[5];
	int brith;
	char pw[15];
}member;
member all[MAX];
void HideCursor() { //마우스 커서 숨기는 함수
	CONSOLE_CURSOR_INFO cursor_info = { 1, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void EnableConsoleCursor() { //마우스 커서 보이게 하는 함수
	CONSOLE_CURSOR_INFO cursor_info = { 1, TRUE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void file_read() { // 파일 읽어서 구조체에 저장하는 함수
	char c;
	FILE* member = fopen("member.txt", "r");
	if (member == NULL) {
		return 0;
	}
	else {
		while (c = fgetc(member) != EOF) {

			fseek(member, -1, SEEK_CUR);
			if (feof(member) != 0) {
				break;
			}
			fscanf(member,"%s %s %s %d %s\n", all[member_count].name, all[member_count].phone, all[member_count].gender,&all[member_count].brith,all[member_count].pw);
			member_count++;
		}
	}
}
void file_append() { //파일 이어쓰는 함수
	FILE* member;
	member = fopen("member.txt", "a");
	fprintf(member, "%s %s %s %d %s\n", all[member_count].name, all[member_count].phone, all[member_count].gender, all[member_count].brith, all[member_count].pw);
	fclose(member);
}
int isValidPhone_or_pw_Number(char* str, int check) { // 전화번호 비밀번호 유효한지 체크해주는 함수
	if (check == 1) {
		int length = strlen(str);

		// 전화번호는 11자리여야 함
		if (length != 11)
			return 0;

		// 전화번호는 "010xxxxxxxx" 형식이어야 함
		if (str[0] != '0' || str[1] != '1' || str[2] != '0')
			return 0;

		// 나머지 자리는 숫자여야 함
		for (int i = 3; i < length; i++) {
			if (str[i] < '0' || str[i] > '9')
				return 0;
		}

		// 모든 조건을 만족하는 경우 유효한 전화번호
		return 1;
	}
	else if(check == 2) {
		int length = strlen(str);
		if (length != 4) {
			return 0;
		}
		else {
			for (int i = 0; i < length; i++) {
				if (str[i] < '0' || str[i] > '9') {
					return 0;
				}
				else {
					return 1;
				}
			}
		}
		
	}
}
int isValidDate(int date) { //생년월일 유효한지 체크해주는 함수
	int year = date / 10000;
	int month = (date % 10000) / 100;
	int day = date % 100;

	// 생년월일은 8자리여야 함
	if (date < 10000000 || date > 99999999)
		return 0;

	// 연도, 월, 일의 범위를 확인
	if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31)
		return 0;

	// 모든 조건을 만족하는 경우 유효한 생년월일
	return 1;
}
enum ColorType {  // 글씨 컬러 
	BLACK,  	//0
	darkBLUE,	//1
	DarkGreen,	//2
	darkSkyBlue,    //3
	DarkRed,  	//4
	DarkPurple,	//5
	DarkYellow,	//6
	GRAY,		//7
	DarkGray,	//8
	BLUE,		//9
	GREEN,		//10
	SkyBlue,	//11
	RED,		//12
	PURPLE,		//13
	YELLOW,		//14
	WHITE		//15
} COLOR;
void textcolor(int colorNum) { // 글씨 컬러 바꿔주는 함수
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}
void goto_xy(int x, int y)  // 좌표값
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos); // x값 y값 입력
}
void click(int* xx, int* yy) { // 마우스 클릭 이벤트 
	while (1)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER); // 콘솔창 입력을 받아들임.
		if (rec.EventType == MOUSE_EVENT) {// 마우스 이벤트일 경우
			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) { // 좌측 버튼이 클릭되었을 경우
				int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X값 받아옴
				int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y값 받아옴

				*xx = mouse_x; //x값을 넘김
				*yy = mouse_y; //y값을 넘김
				break;
			}
		}
	}
}
void clearconsole() { //콘솔창 클리어
	printf("Asd");
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100 * 100, Coor, &dw); // 콘솔창 화면을 지운다.
}
void Mouse() { //마우스 활성화 코드
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	DWORD CIN;
	DWORD mode;
	CIN = GetStdHandle(STD_INPUT_HANDLE); //마우스 재활성화
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
	//마우스 입력모드로 바꿈
}
void ExClick() { // 클릭 좌표 값 확인 UI
	int xx, yy, lr;
	while (1) {
		click(&xx, &yy);
		printf("%3d %3d\n", xx, yy);
	}
}
void basic_UI() { // 기본 UI
	int x = 30;
	int y = 3;
	textcolor(DarkYellow);
	goto_xy(x, y);
	printf("┏");
	for (int i = 0; i < 35; i++)
	{
		printf("━");
	}
	printf("┓");
	for (int i = 0; i < 45; i++) {
		y += 1;
		if (i == 3 || i == 40) {
			goto_xy(x, y);
			printf("┣");
			for (int i = 0; i < 35; i++) {
				printf("━");
			}
			printf("┫");
		}
		else {
			goto_xy(x, y);
			printf("┃                                                                      ┃");
		}
	}
	goto_xy(x, y + 1);
	printf("┗");
	for (int i = 0; i < 35; i++)
	{
		printf("━");
	}
	printf("┛");
}
void small_box(int x,int y,int color) { // 체크 박스 UI
	textcolor(color);
	goto_xy(x, y);
	printf("┏");
	for (int i = 0; i < 6; i++)
	{
		printf("━");
	}
	printf("┓");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("┃            ┃");
	}
	goto_xy(x, y + 1);
	printf("┗");
	for (int i = 0; i < 6; i++)
	{
		printf("━");
	}
	printf("┛");

} 
void big_box(int x, int y,int color) { // 체크박스 큰거 UI
	textcolor(color);
	goto_xy(x, y);
	printf("┏");
	for (int i = 0; i < 9; i++)
	{
		printf("━");
	}
	printf("┓");

	for (int i = 0; i < 3; i++) {
		y += 1;
		goto_xy(x, y);
		printf("┃                  ┃");
	}
	goto_xy(x, y + 1);
	printf("┗");
	for (int i = 0; i < 9; i++)
	{
		printf("━");
	}
	printf("┛");
}
void box_clear() { //기본 UI 클리어 해주는 함수
	int x = 62;
	int y = 5;
	goto_xy(62, 5);
	printf("                 ");
	x = 32;
	y = 8;
	for (int i = 0; i < 35; i++) {
		goto_xy(x, y);
		printf("                                                            ");
		y += 1;
	}
	x = 32;
	y = 45;
	for (int i = 0; i < 4; i++) {
		goto_xy(x, y);
		printf("                                                                     ");
		y += 1;
	}

}
void membership() { // 회원가입 함수 
	HideCursor();
	int xx, yy, lr = 0;
	int xx2, yy2, lr2 = 0;
	char name[20] = " ";
	char phone[20] = " ";
	char gender[5] = " ";
	int brith = 0;
	char pw[15] = " ";
	int check = 0;
	goto_xy(62, 5);
	textcolor(6);
	printf("회원가입");
	small_box(50, 10, 6);
	small_box(67, 10, 6);
	small_box(38, 46, 6);
	small_box(80, 46, 6);
	goto_xy(56, 11);
	textcolor(BLUE);
	printf("남자");
	goto_xy(73,11);
	textcolor(RED);
	printf("여자");
	goto_xy(44, 47);
	textcolor(6);
	printf("이전");
	goto_xy(86, 47);
	textcolor(6);
	printf("가입");
	goto_xy(50, 18);
	printf("이름 ex) 홍길동");
	goto_xy(50, 20);
	printf(":");
	goto_xy(50, 24);
	printf("전화번호 11자리 ex) 010xxxxxxxx ");
	goto_xy(50, 26);
	printf(":");
	goto_xy(50, 30);
	printf("생년월일(8자리) ex) 20030728 ");
	goto_xy(50, 32);
	printf(":");
	goto_xy(50, 36);
	printf("비밀번호(숫자 4자리) ex) 0000");
	goto_xy(50, 38);
	printf(":");
	Mouse();
	//ExClick();
	while (1) {
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10);
				textcolor(6);
				goto_xy(44, 47);
				printf("이전");
				Sleep(500);
				break;
			}
		}
		if (xx > 50 && xx < 64) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "남");
				small_box(67, 10, 6);
				goto_xy(73, 11);
				textcolor(RED);
				printf("여자");
				small_box(50, 10, 10);
				goto_xy(56, 11);
				textcolor(BLUE);
				printf("남자");
			}
		}
		if (xx > 67 && xx < 82) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "여");
				small_box(50, 10,6);
				goto_xy(56, 11);
				textcolor(BLUE);
				printf("남자");
				small_box(67, 10, 10);
				goto_xy(73, 11);
				textcolor(RED);
				printf("여자");
			}
		}
	    if (xx > 38 && xx < 80) {
			if (yy > 18 && yy < 22) {
				strcpy(name, " ");
				textcolor(6);
				goto_xy(58, 20);
				printf("                                 ");
				goto_xy(58, 20);
				EnableConsoleCursor();
				gets(name);
				HideCursor();

			}
		}
		if (xx > 50 && xx < 82) {
			if (yy > 25 && yy < 28) {
				textcolor(6);
				goto_xy(58, 26);
				printf("                                 ");
				goto_xy(58, 26);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone,1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(58, 26);
					printf("형식이 올바르지 않습니다..");
				}

			}
		}
		if (xx > 52 && xx < 78) {
			if (yy > 29 && yy < 34) {
				textcolor(6);
				goto_xy(58, 32);
				printf("                                 ");
				goto_xy(58, 32);
				EnableConsoleCursor();
				scanf("%d", &brith);
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(58, 32);
					printf("형식이 올바르지 않습니다..");
				}
			}
		}
		if (xx > 52 && xx < 81) {
			if (yy > 36 && yy < 40) {
				textcolor(6);
				goto_xy(58, 38);
				printf("                                 ");
				goto_xy(58, 38);
				EnableConsoleCursor();
				scanf("%s", pw);
				HideCursor();
				check = isValidPhone_or_pw_Number(pw, 2);
				if (check == 0) {
					strcpy(pw, " ");
					goto_xy(58, 38);
					printf("형식이 올바르지 않습니다..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if(strcmp(name ," ") !=0 && strcmp(gender," ")!=0 && strcmp(pw," ")!=0 && strcmp(phone," ")!=0 && brith != 0) {
					check = 0;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].phone, phone )==0) {
							check = 1;
						}
					}
					box_clear();
					goto_xy(62, 5);
					textcolor(6);
					printf("회원가입");
					small_box(38, 46, 6);
					small_box(80, 46, 6);
					goto_xy(44, 47);
					textcolor(6);
					printf("이전");
					goto_xy(85, 47);
					textcolor(6);
					printf("로그인");
					if (check == 1) {
						goto_xy(52, 24);
						printf("이미 가입된 전화번호 입니다.");
					}
					else {
						goto_xy(54, 24);
						printf("회원가입이 완료되었습니다.");
						strcpy(all[member_count].name, name);
						strcpy(all[member_count].phone, phone);
						strcpy(all[member_count].pw, pw);
						strcpy(all[member_count].gender, gender);
						all[member_count].brith = brith;
						file_append();
					}
					click(&xx2, &yy2);
					while (1) {
						printf("%d %d", xx2, yy2);
						if (xx2 > 38 && xx2 < 53) {
							if (yy2 > 45 && yy2 < 49) {
								small_box(38, 46, 10);
								textcolor(6);
								goto_xy(44, 47);
								printf("이전");
								Sleep(500);
								break;
							}
						}
						else if (xx2 > 80 && xx2 < 95) {
							if (yy2 > 45 && yy2 < 49) {
								printf("%d %d", xx, yy);
								small_box(80, 46, 10);
								textcolor(6);
								goto_xy(85, 47);
								printf("로그인");
								//login();
								Sleep(500);
								break;
							}
						}
					}
					break;
				}
			}
		}		
	}
}
void initial_screen() { // 초기화면
	box_clear();
	HideCursor();
	int xx, yy, lr;
	basic_UI();
	big_box(57,20, 6);
	big_box(57, 27, 6);
	small_box(36, 46, 6);
	small_box(59, 46, 6);
	small_box(82, 46, 6);
	goto_xy(62, 5);
	printf("가든 헤어샵");
	goto_xy(66, 22);
	printf("예약");
	goto_xy(61, 29);
	printf("예약 내역 조회");
	goto_xy(41, 47);
	printf("로그인");
	goto_xy(63, 47);
	printf("회원가입");
	goto_xy(87, 47);
	printf("PW 찾기");
	Mouse();
	while (1) {
		click(&xx, &yy);
		if (xx > 59 && xx < 74) {
			if (yy > 46 && yy < 49) {
				small_box(59, 46, 10);
				goto_xy(63, 47);
				printf("회원가입");
				Sleep(500);
				box_clear();
				membership();
				break;
			}
		}
		if (xx > 36 && xx < 50) {
			if (yy > 46 && yy < 49) {
				small_box(36, 46, 10);
				goto_xy(41, 47);
				printf("로그인");
				Sleep(500);
				box_clear();
				//login();
				break;
			}
		}
		
	}
	initial_screen();
}
int main(void) { //메인함수
	file_read();
	initial_screen();
}
