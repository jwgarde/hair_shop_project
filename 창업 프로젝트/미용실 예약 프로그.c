#define _CRT_SECURE_NO_WARNINGS
#define MAX 200 //회원 최대 인원 수
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include"resource.h"
#define MAX_X 174
#define MAX_Y 40
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;
int member_count = 0;
int designer_count = 0;
int design_count = 0;
int take_menu_count = 0;
typedef struct { //디자이너에 따른 메뉴 보여주기 위함
	int index;
	char name[20];
	int price;
	char account[240];
}d_menu;
d_menu D_MENU[20];

// 콘솔 윈도우 창의 핸들값    
static HWND hWnd;
// 프로그램의 인스턴스 핸들값
static HINSTANCE hInst;

typedef struct { // 회원 구조체
	char name[20];
	char phone[20];
	char gender[5];
	int brith;
	char pw[15];
}member;
member all[MAX];
typedef struct {
	char name[20];
	char phone[20];
	char gender[15];
	int brith;
	char n_name[15];
	char introduce[50];
}DESIGNER;
DESIGNER d_all[3];
typedef struct {
	char designer_name[20];
	char sort[15];
	char name[20];
	int price;
	char account[220];
}style;
style STYLE[100];
void long_box_UI(int x, int y) {
	goto_xy(x, y);
	printf("┏");
	for (int i = 0; i < 30; i++)
	{
		printf("━");
	}
	printf("┓");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("┃                                                            ┃");
	}
	goto_xy(x, y + 1);
	printf("┗");
	for (int i = 0; i < 30; i++)
	{
		printf("━");
	}
	printf("┛");
}
int getMaskedInput() { //디자이너 비밀번호 생일 입력 할떄 *로 출력 해주는 함수
	int birth = 0;
	int ch;
	int i = 0;

	while (i < 8) {
		ch = _getch();
		if (ch == 13) { // 엔터를 눌렀을 때
			break; // 입력 종료
		}
		if (ch >= '0' && ch <= '9') {
			putchar('*');
			birth = birth * 10 + (ch - '0');
			i++;
		}
		else if (ch == 8 && i > 0) { // 백스페이스가 눌렸을 때
			putchar('\b'); // 백스페이스 문자 출력
			putchar(' ');  // 공백 문자로 덮어씀
			putchar('\b'); // 백스페이스 문자 출력
			birth = birth / 10; // 가장 마지막 자리 제거
			i--;
		}
	}

	return birth;
}
void clearCurrentChar(int x, int y) {
	goto_xy(x, y);
	printf(" "); // Print a space to clear the character
	goto_xy(x, y); // Move the cursor back to the original position
}

void handleNewline(int* x, int* y) {
	(*y)++;
	goto_xy(*x, *y); // Move the cursor to the new position
}

void handleBackspace(char* str, int* len, int* x, int* y) {
	if (*len > 0) {
		(*len)--;
		clearCurrentChar((*x) + (*len) % MAX_X, *y); //이부분
		str[(*x - 124) + (*len) + (MAX_X - 124) * (*y - 36)] = '\0'; // Remove the deleted character from the string
		goto_xy(*x + (*len) % MAX_X, *y); // Move the cursor back to the original position
	}
	else {
		if (*y > 36) { // Check if y is greater than 0 (not on the first line)
			(*y)--;
			clearCurrentChar(MAX_X - 1, *y); // Clear the last character on the current line
			goto_xy(*x, *y);
			*len = (MAX_X - 124) - 1; // Set len to the last index of the previous line
		}
		else {
			return;
		}
	}
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
			fscanf(member, "%s %s %s %d %s\n", all[member_count].name, all[member_count].phone, all[member_count].gender, &all[member_count].brith, all[member_count].pw);
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
void file_write() { // 디자이너 파일 쓰기
	FILE* member;
	member = fopen("member.txt", "w");
	for (int i = 0; i < member_count; i++) {
		fprintf(member, "%s %s %s %d %s\n", all[i].name, all[i].phone, all[i].gender, all[i].brith, all[i].pw);

	}
	fclose(member);
}
void d_file_read() { // 디자이너 파일 읽기
	char c;
	FILE* designer = fopen("designer.txt", "r");
	if (designer == NULL) {
		return 0;
	}
	else {
		while (c = fgetc(designer) != EOF) {

			fseek(designer, -1, SEEK_CUR);
			if (feof(designer) != 0) {
				break;
			}
			fscanf(designer,"%s %s %s %d %s %[^\n]\n", d_all[designer_count].name, d_all[designer_count].phone, d_all[designer_count].gender, &d_all[designer_count].brith, d_all[designer_count].n_name, d_all[designer_count].introduce);
			designer_count++;
		}
	}
}
void d_file_write() { // 디자이너 파일 쓰기
	FILE* designer = fopen("designer.txt", "w");
	for (int i = 0; i < designer_count; i++) {
		fprintf(designer, "%s %s %s %d %s %s\n", d_all[i].name, d_all[i].phone, d_all[i].gender, d_all[i].brith, d_all[i].n_name, d_all[i].introduce);
	}
	fclose(designer);
}
void design_file_read() { //디자인 파일 읽기
	char c;
	FILE* design = fopen("design.txt", "r");
	if (design == NULL) {
		return 0;
	}
	else {
		while (c = fgetc(design) != EOF) {

			fseek(design, -1, SEEK_CUR);
			if (feof(design) != 0) {
				break;
			}
			fscanf(design,"%s %s %s %d/%[^\n]\n", STYLE[design_count].designer_name, STYLE[design_count].sort, STYLE[design_count].name, &STYLE[design_count].price, &STYLE[design_count].account);
			design_count++;
		}
	}
	fclose(design);
}
void design_file_append() {
	FILE* design;
	design = fopen("design.txt", "a");
	fprintf(design, "%s %s %s %d/%s\n", STYLE[design_count].designer_name, STYLE[design_count].sort, STYLE[design_count].name, STYLE[design_count].price, STYLE[design_count].account);
	fclose(design);
}
void HideCursor() { //마우스 커서 숨기는 함수
	CONSOLE_CURSOR_INFO cursor_info = { 1, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void EnableConsoleCursor() { //마우스 커서 보이게 하는 함수
	CONSOLE_CURSOR_INFO cursor_info = { 1, TRUE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void design_take_menu(int index, char *str) {
	take_menu_count = 0;
	for (int i = 0; i < 20; i++) {
		D_MENU[i].index = 0;
		strcpy(D_MENU[i].name, " ");
		D_MENU[i].price = 0;
		strcpy(D_MENU[i].account," ");
	}
	for (int i = 0; i < design_count; i++) {
		if (strcmp(d_all[index].name, STYLE[i].designer_name) == 0 && strcmp(str, STYLE[i].sort) == 0) {
			D_MENU[take_menu_count].index = i;
			D_MENU[take_menu_count].price = STYLE[i].price;
			strcpy(D_MENU[take_menu_count].name, STYLE[i].name);
			strcpy(D_MENU[take_menu_count].account, STYLE[i].account);
			take_menu_count++;
		}
	}
}
void textcolor(int colorNum) { // 글씨 컬러 바꿔주는 함수
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}
int goto_xy(int x, int y)  // 좌표값
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
void clearconsole() { //콘솔창 클리어
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
	int xx = 0, yy = 0, lr = 0;
	while (1) {
		click(&xx, &yy);
		printf("%3d %3d\n", xx, yy);
	}
}
void add_box_UI(int x, int y, int color, int xx, int yy, char* str) {
	textcolor(color);
	goto_xy(x, y);
	printf("┏");
	for (int i = 0; i < 26; i++)
	{
		printf("━");
	}
	printf("┓");

	for (int i = 0; i < 8; i++) {
		y += 1;
		goto_xy(x, y);
		printf("┃                                                    ┃");
	}
	goto_xy(x, y + 1);
	printf("┗");
	for (int i = 0; i < 26; i++)
	{
		printf("━");
	}
	printf("┛");
	textcolor(4);
	goto_xy(xx, yy);
	printf("%s", str);
}
void basic_UI_DELETE(int x, int y) {
	while(1) {
		goto_xy(x, y);
		printf("                                                                                                            ");
		y++;
		if (y == 50) {
			break;
		}
	}
}
void basic_UI(int x, int y) { // 기본 UI
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
void m_basic_UI() { // 로그인 후 회원 기본 UI
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
		if (i == 3 || i == 7 || i == 40) {
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
void small2_box(int x, int y, int color, int xx, int yy, char* str, int color2) {
	textcolor(color);
	goto_xy(x, y);
	printf("┏");
	for (int i = 0; i < 8; i++)
	{
		printf("━");
	}
	printf("┓");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("┃                ┃");
	}
	goto_xy(x, y + 1);
	printf("┗");
	for (int i = 0; i < 8; i++)
	{
		printf("━");
	}
	printf("┛");
	textcolor(color2);
	goto_xy(xx, yy);
	printf("%s", str);
}
void small_box(int x, int y, int color, int xx, int yy, char* str, int color2) { // 체크 박스 UI
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
	textcolor(color2);
	goto_xy(xx, yy);
	printf("%s", str);

}
void big_box(int x, int y, int color, int xx, int yy, char* str) { // 체크박스 큰거 UI
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
	textcolor(6);
	goto_xy(xx, yy);
	printf("%s", str);
}

void box_clear() { //기본 UI 클리어 해주는 함수
	int x = 62;
	int y = 5;
	goto_xy(60, 5);
	printf("                   ");
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
void Render(int x, int y, int num)
{
	const int pictureWidth = 130;
	const int pictureHeight = 150;
	// DC의 핸들값을 받을 변수를 선언한다.(hDC : 실제화면DC, hMemDC : 이미지를 담을 DC)
	// Device Context는 그래픽에 필요한 모든 옵션을 넣어둔 구조체라고 볼 수 있다.
	// 그림그릴때 그림을 그릴 화폭이라 보면된다.
	HDC hDC, hMemDC;
	// 후면버퍼이다. static 변수는 정적변수로서, 메모리의 Data영역에 저장되는 함수가 끝나도 사라지지 않고 메모리에 남아있는 변수이다.
	static HDC hBackDC;
	// 이미지 비트맵의 핸들값을 저장할 변수이다.
	HBITMAP hBitmap = NULL, hOldBitmap, hBackBitmap;
	// 핸들값으로부터 얻어올 실제 비트맵 구조체.
	BITMAP Bitmap;
	// 현재 윈도우의 Rect값(크기)를 얻어온다. Rect는 왼쪽위, 오른쪽 아래의 값을 가지는 정사각형을 나타내는 구조체이다.
	RECT WindowRect;
	GetWindowRect(hWnd, &WindowRect);

	// 현재 윈도우의 DC 핸들값을 얻어온다. GetWindowDC(hWnd)도 가능하다.
	hDC = GetDC(hWnd);
	// hDC와 호환되는 DC를 메모리에 만들어 핸들값을 반환한다.
	hBackDC = CreateCompatibleDC(hDC);
	hMemDC = CreateCompatibleDC(hDC);

	// 비트맵 메모리를 할당하고 핸들을 반환한다.
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	// 그릴 도화지를 준비한다.
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
	// 비트맵을 로드하여 핸들을 반환한다. resource.h에 정수로 define되어있는 불러온 리소스를 로드한다.
	if (num == 0) {
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	}
	else if (num == 1) {
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	}
	else if (num == 2) {
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	}
	// 크기를 받기위해 비트맵 구조체를 비트맵 핸들로부터 생성한다.
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);

	HBITMAP hResizedBitmap = (HBITMAP)CopyImage(hBitmap, IMAGE_BITMAP, pictureWidth, pictureHeight, LR_COPYDELETEORG);
	GetObject(hResizedBitmap, sizeof(BITMAP), &Bitmap);
	// 읽어온 비트맵 리소스를 메모리 DC에 선택한다.
	SelectObject(hMemDC, hResizedBitmap);

	// hMemDC의 이미지를 hBackDC의 원하는 위치로 고속복사시킨다.(출력하려는 이미지를 후면버퍼에 복사시킨다.)
	BitBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// hBackDC(후면 버퍼)의 완성된 그림을 화면으로 고속복사시킨다.
	BitBlt(hDC, x, y, x + Bitmap.bmWidth, y + Bitmap.bmHeight, hBackDC, 0, 0, SRCCOPY);

	// 메모리와 오브젝트를 해지한다.
	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteObject(hResizedBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);
}
int buid(int num) {
	// 테스트용으로 입력을 받을 버퍼
	char buf[100] = { 0, };
	int i = 0;
	// 콘솔 윈도우 창의 핸들값을 얻어온다.
	hWnd = GetConsoleWindow();
	// 프로그램의 인스턴스 핸들값을 얻어온다.
	hInst = GetModuleHandle(NULL);
	// 루프를 돈다.
	bool isFinished = true;
	while (1) {
		// 그림을 그린다.
		Render(10, 10, num);

		// 그림 그리기 작업이 처음부터 완료되었다고 가정하고 isFinished를 true로 초기화한다.

		if (isFinished) {
			break; // 반복문을 종료한다.
		}
	}
}
int add_design(int index,char* string) {
	basic_UI(108,3);
	int xx, yy = 0;
	char sort[15] = " ";
	char name[20] = " ";
	int price = 0;
	char str[240] = " ";// 문자열을 저장 총 100글자
	int len = 0;
	int x = 124;
	int y = 36; 
	char ch = ' ';
	small_box(158, 46, 6, 164, 47, "등록", 6);
	textcolor(6);
	goto_xy(177, 4);
	printf("[X]");
	goto_xy(140, 5);
	textcolor(6);
	printf("디자인 추가");
	goto_xy(122, 13);
	printf("종류      :");
	strcpy(sort, string);
	goto_xy(143, 13);
	EnableConsoleCursor();
	printf("%s", sort);
	HideCursor();
	goto_xy(122, 19);
	printf("이름      :");
	goto_xy(122, 25);
	printf("가격      :");
	goto_xy(122, 31);
	printf("설명 (입력 후 ENTER) ");
	add_box_UI(122,33,6,143,37,"※최대 100글자");
	textcolor(6);
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 174 && xx < 182) {
			if (yy > 2 && yy < 6) {
				textcolor(4);
				goto_xy(177, 4);
				printf("[X]");
				Sleep(500);
				basic_UI_DELETE(108, 3);
				return;
			}
		}
		if (xx > 132 && xx < 171) {			
			if (yy > 17 && yy < 21) {
				strcpy(name, "");
				goto_xy(140, 19);
				printf("                                   ");
				goto_xy(143, 19);
				EnableConsoleCursor();
				gets(name);
				HideCursor();
				if (strcmp(name, "") == 0) {
					goto_xy(143, 19);
					printf("형식이 올바르지 않습니다.");
				}
			}
			else if (yy > 23 && yy < 27) {
				price = 0;
				char input[30]= "";
				strcpy(input,"");
				goto_xy(140, 25);
				printf("                                   ");
				goto_xy(143, 25);
				EnableConsoleCursor();
				fgets(input, sizeof(input), stdin);
				HideCursor();

				if (input[0] == '\n') {
					price = 0;
				}
				else {
					price = atoi(input);
				}

				if (price == 0) {
					goto_xy(143, 25);
					printf("형식이 올바르지 않습니다.");
				}
			}
		
		}
		if (xx > 122 && xx < 175) {
			if (yy > 32 && yy < 43) {
				strcpy(str," ");
				add_box_UI(122, 33, 6, 143, 37, " ");// 진행시켜 설명 적는거 해야지
				len = 0;
				x = 124;
				y = 36;
				EnableConsoleCursor();
				goto_xy(x, y);
				textcolor(6);
				while (1) {
					 ch = ' ';
					 ch = _getch();
					if (ch == '\r') { // Enter key
						ch = ' ';
						break;
					}
					else if (ch == '\b') { // Backspace key
						if (str[(x - 124) + (len)+(MAX_X - 124) * (y - 36) - 1] & 0x80) {
							if (str[(x - 124) + (len)+(MAX_X - 124) * (y - 36) - 2] & 0x80){
								handleBackspace(str, &len, &x, &y);
								handleBackspace(str, &len, &x, &y);
							}
							else {
								handleBackspace(str, &len, &x, &y);
							}
						}
						else {
							handleBackspace(str, &len, &x, &y);
						}
					}
					else if (len < sizeof(str) - 2) {
						if (len >= MAX_X - 124 && (len % (MAX_X - 124)) == 0) { // Check if the line length is multiple of MAX_X
							if (y >= MAX_Y - 1) {
								continue;
							}
							else {

								handleNewline(&x, &y);
								len = 0;
								str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = ch;
								len++;
								str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = '\0';
								printf("%c", ch);
							}
						}
						else {
							str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = ch;
							len++;
							str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = '\0';
							printf("%c", ch);
						}
					}
				}
				HideCursor();
			}
		}
		if (xx > 158 && xx < 174) {
			if (yy > 45 && yy < 49) {
				if (strcmp(name, " ") != 0 && strcmp(sort, " ") != 0 && price != 0) {
					small_box(158, 46, 10, 164, 47, "등록", 6);
					Sleep(500);
					small_box(158, 46, 6, 164, 47, "등록", 6);
					strcpy(STYLE[design_count].designer_name, d_all[index].name);
					strcpy(STYLE[design_count].name, name);
					strcpy(STYLE[design_count].sort,sort);
					STYLE[design_count].price = price;
					strcpy(STYLE[design_count].account,str);
					design_file_append();
					design_count++;
					clearconsole();
					Sleep(1500);
					return 1;
				}
			}
		}
	}
}
int style_management(int index) {
	char str[15] = "펌";
	int page_count = 1;
	int choice = 1;
	int x = 35, y = 12;
	box_clear();
	int i = 0;
	int count = 6;
	int len = 0;
	while (1) {
		x = 35, y = 12;
		basic_UI(30, 3);
		int xx, yy = 0;
		int check = 0;
		goto_xy(61, 5);
		textcolor(6);
		printf("디자인 관리");
		small_box(35, 8, 6, 41, 9, "커트", 6);
		small_box(51, 8, 6, 58, 9, "펌", 6);
		small_box(67, 8, 6, 73, 9, "컬러", 6);
		small_box(83, 8, 6, 89, 9, "기타", 6);
		goto_xy(61, 39);
		printf("◁");
		goto_xy(66, 39);
		printf("%d", page_count);
		goto_xy(71, 39);
		printf("▷");
		small_box(38, 46, 6, 44, 47, "이전", 6);
		small_box(80, 46, 6, 86, 47, "추가", 6);
		//ExClick();
		if (choice == 1) {
			strcpy(str,"커트");
			small_box(35, 8, 10, 41, 9, "커트", 6);
		}
		else if (choice == 2) {
			strcpy(str, "펌");
			small_box(51, 8, 10, 58, 9, "펌", 6);
		}
		else if (choice == 3) {
			strcpy(str, "컬러");
			small_box(67, 8,10, 73, 9, "컬러", 6);
		}
		else if (choice == 4) {
			strcpy(str, "기타");
			small_box(83, 8, 10, 89, 9, "기타", 6);
		}
		design_take_menu(index,str);
		for (i; i < count*page_count; i++) {
			x = 35;
			if (strcmp(D_MENU[i].name, " ") == 0) {
				break;
			}
			else {
				long_box_UI(x, y);
				x = 39;
				goto_xy(x, y+1);
				printf("%s", D_MENU[i].name);
				goto_xy(x+15, y+1);
				printf("%d", D_MENU[i].price);
				goto_xy(x + 30, y+1);
				if (strcmp(D_MENU[i].account," ")==0) {
					goto_xy(x + 39, y + 1);
					printf("X");
				}
				else {
					len = strlen(D_MENU[i].account);
					for (int k = 0; k < len; k++) {
						if (k > 19) {
							if (D_MENU[i].account[k] & 0x80) {
								printf("%c", D_MENU[i].account[k]);
							}
							printf("...");
							break;
						}
						else {
							printf("%c", D_MENU[i].account[k]);
						}
					}
				}
				y += 4;
			}
		}
		if (i == (count * page_count) - 1) {
			i++;
		}

		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10, 44, 47, "이전", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 59 && xx < 65 && yy > 37 && yy < 41) {
				if (page_count != 1) {
					textcolor(10);
					goto_xy(61, 39);
					printf("◁");
					Sleep(500);
					i = (page_count - 2) * count;
					page_count--;
					break;
				}
			}
			if (xx > 67 && xx < 74 && yy > 37 && yy < 41) {
				if (strcmp(D_MENU[i].name, " ") != 0) {
					textcolor(10);
					goto_xy(71, 39);
					printf("▷");
					Sleep(500);
					page_count++;
					break;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					small_box(80, 46, 10, 86, 47, "추가", 6);
					Sleep(500);
					small_box(80, 46, 6, 86, 47, "추가", 6);
					check = add_design(index,str);
					if (check == 1) {
						i = 0;
						page_count = 1;
						break;
					}
				}
			}
			if (yy > 6 && yy < 11) {
				if (xx > 34 && xx < 50) {
					if (choice != 1) {
						choice = 1;
						i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 50 && xx < 67) {
					if (choice != 2) {
						choice = 2;
						i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 67 && xx < 82) {
					if (choice != 3) {
						choice = 3;
						i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 83 && xx < 98) {
					if (choice != 4) {
						choice = 4;
						i = 0;
						page_count = 1;
						break;
					}
				}
			}

		}
	}
}
void copy() {
	char sourcePath[100];  // 입력 받은 경로를 저장할 변수
	char destinationPath[] = "C:\\Users\\chlwj\\source\\repos\\그만\\그만\\bitmap2.bmp";
	goto_xy(5, 40);
	printf("파일 경로를 입력하세요: ");
	fgets(sourcePath, sizeof(sourcePath), stdin);
	sourcePath[strcspn(sourcePath, "\n")] = '\0';  // 개행 문자 제거

	// 파일 복사 수행
	FILE* sourceFile = fopen(sourcePath, "rb");
	FILE* destinationFile = fopen(destinationPath, "wb");

	if (sourceFile == NULL) {
		printf("입력한 경로에 파일을 찾을 수 없습니다.\n");
		return 1;
	}

	if (destinationFile == NULL) {
		printf("목적지 경로에 파일을 생성할 수 없습니다.\n");
		fclose(sourceFile);
		return 1;
	}

	int bufferSize = 1024;  // 복사할 때 사용할 버퍼 크기
	char* buffer = (char*)malloc(bufferSize);

	if (buffer == NULL) {
		printf("메모리 할당에 실패했습니다.\n");
		fclose(sourceFile);
		fclose(destinationFile);
		return 1;
	}

	size_t bytesRead;
	while ((bytesRead = fread(buffer, 1, bufferSize, sourceFile)) > 0) {
		fwrite(buffer, 1, bytesRead, destinationFile);
	}

	printf("파일이 성공적으로 복사되었습니다.\n");

	fclose(sourceFile);
	fclose(destinationFile);
	free(buffer);

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
int designer_choice(int index) {
	d_file_read();
}
int designer_initial_screen(int index) { //디자이너 초기 화면
	int xx, yy, lr = 0;
	int choice = 0;
	while (1) {
		box_clear();
		m_basic_UI();
		xx = 0, yy = 0;
		goto_xy(63, 5);
		textcolor(6);
		printf("%s님",d_all[index].name);
		big_box(57, 14, 6, 64, 16, "예약 관리");
		big_box(57, 22, 6, 63, 24, "디자인 관리");
		big_box(57, 30, 6, 63, 32, "프로필 관리");
		big_box(57, 38, 6, 64, 40, "리뷰 관리");
		small_box(38, 46, 6, 42, 47, "로그아웃", 6);
		small_box(80, 46, 6, 86, 47, "다음", 6);
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10, 42, 47, "로그아웃", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 21 && yy < 27) {
					big_box(57, 22, 10, 63, 24, "디자인 관리");
					big_box(57, 14, 6, 64, 16, "예약 관리");
					big_box(57, 30, 6, 63, 32, "프로필 관리");
					big_box(57, 38, 6, 64, 40, "리뷰 관리");
					choice = 2;
					xx, yy = 0;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(80, 46, 10, 86, 47, "다음", 6);
						Sleep(500);
						if (choice == 2) {
							style_management(index);
							break;
						}
					}
				}
			}
			xx = 0, yy = 0;
		}
		xx = 0, yy = 0;
	}
}
int designer_login() { //디자이너 로그인 
	d_file_read();
	box_clear();
	goto_xy(60, 5);
	textcolor(6);
	printf("디자이너 로그인");
	small_box(38, 46, 6, 44, 47, "이전", 6);
	small_box(80, 46, 6, 85, 47, "로그인", 6);
	char phone[20] = " ";
	int brith = 0;
	int xx = 0, yy = 0, lr = 0;
	int check = 0;
	goto_xy(53, 19);
	printf("전화번호 (11자리) ex) 010xxxxxxxx ");
	goto_xy(53, 21);
	printf(":");
	goto_xy(53, 28);
	printf("생년월일 (8자리) ex) xxxxxxxx");
	goto_xy(53, 30);
	printf(":");
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10, 44, 47, "이전", 6);
				Sleep(500);
				return;
			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 19 && yy < 23) {
				textcolor(6);
				goto_xy(55, 21);
				printf("                                 ");
				goto_xy(55, 21);
				EnableConsoleCursor();
				gets(phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(55, 21);
					printf("형식이 올바르지 않습니다..");
				}

			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 28 && yy < 32) {
				textcolor(6);
				goto_xy(55, 30);
				printf("                                 ");
				goto_xy(55, 30);
				EnableConsoleCursor();
				brith = getMaskedInput();
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(55, 30);
					printf("형식이 올바르지 않습니다..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (brith != 0 && strcmp(phone, " ") != 0) {
					small_box(80, 46, 10, 85, 47, "로그인", 6);
					check = -1;
					for (int i = 0; i < designer_count; i++) {
						if (strcmp(d_all[i].phone, phone) == 0 && d_all[i].brith == brith) {
							check = i;
							break;
						}
					}
					Sleep(500);
					if (check >= 0) {
						designer_initial_screen(check);
						return 1;
					}
					else {
						small_box(80, 46, 6, 85, 47, "로그인", 6);
						goto_xy(48, 40);
						textcolor(4);
						printf("전화번호 혹은 비밀번호를 다시 입력 해주세요.");
					}
				}

			}

		}
		xx = 0, yy = 0;
	}
}
void modifying_membership(int index) { //회원정보 수정
	int n_len = 0;
	int pw_check = 1;
	int ph_check = 1;
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[15] = " ";
	char pw[15] = " ";
	basic_UI(30,3);
	goto_xy(62, 5);
	textcolor(6);
	printf("회원정보");
	small_box(38, 46, 6, 44, 47, "이전", 6);
	small_box(80, 46, 6, 86, 47, "저장", 6);
	goto_xy(51, 11);
	textcolor(4);
	printf("이름, 전화번호, 비밀번호만 수정 가능");
	textcolor(6);
	goto_xy(51, 17);
	printf("이름 :");
	goto_xy(68, 17);
	printf("%s", all[index].name);
	goto_xy(51, 22);
	printf("전화번호 :");
	goto_xy(68, 22);
	printf("%s", all[index].phone);
	goto_xy(51, 27);
	printf("성별 :");  
	goto_xy(68, 27);
	if (strcmp(all[index].gender, "남") == 0) {\
		printf("남자");
	}
	else {
		printf("여자");
	}
	goto_xy(51, 32);
	printf("생년월일(8자리) :");
	goto_xy(68, 32);
	printf("%d", all[index].brith);
	goto_xy(51, 37);
	printf("비밀번호(4자리) :");
	goto_xy(68, 37);
	printf("%s", all[index].pw);
	strcpy(name, all[index].name);
	strcpy(pw,all[index].pw);
	strcpy(phone,all[index].phone);
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10, 44, 47, "이전", 6);
				Sleep(500);
				return;
			}
		}
		if (xx > 56 && xx < 87) {
			if (yy > 15 && yy < 19) {
				textcolor(6);
				goto_xy(58, 17);
				printf("                                    ");
				goto_xy(68, 17);
				EnableConsoleCursor();
				scanf("%s", name);
				HideCursor();
			}
		}
		if (xx > 60 && xx < 87) {
			if (yy > 20 && yy < 24) {
				textcolor(6);
				goto_xy(65, 22);
				printf("                                 ");
				goto_xy(68, 22);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				ph_check = isValidPhone_or_pw_Number(phone, 1);
				if (ph_check == 0) {
					strcpy(phone, all[index].phone);
					goto_xy(68, 22);
					printf("형식이 올바르지 않습니다..");
				}

			}
		}
		if (xx > 67 && xx < 87) {
			if (yy > 35 && yy < 39) {
				textcolor(6);
				goto_xy(68, 37);
				printf("                                 ");
				goto_xy(68, 37);
				EnableConsoleCursor();
				scanf("%s", pw);
				HideCursor();
				pw_check = isValidPhone_or_pw_Number(pw, 2);
				if (pw_check == 0) {
					strcpy(pw, all[index].pw);
					goto_xy(68, 37);
					printf("형식이 올바르지 않습니다..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (pw_check != 0 && ph_check != 0) {
					small_box(80, 46, 10, 86, 47, "저장", 6);
					strcpy(all[index].name, name);
					strcpy(all[index].pw, pw);
					strcpy(all[index].phone, phone);
					file_write();
					Sleep(500);
					return;
				}
			}
		}
		xx = 0, yy = 0;
	}
	//ExClick();
}
int member_initial_screen(int index) { //로그인 성공시 회원 초기화면
	int xx, yy, lr = 0;
	int choice = 0;
	while (1) {
		box_clear();
		m_basic_UI();
		xx = 0, yy = 0;
		goto_xy(63, 5);
		textcolor(6);
		printf("%s님", all[index].name);
		big_box(57, 18, 6, 66, 20, "예약");
		big_box(57, 25, 6, 61, 27, "예약 내역 조회");
		big_box(57, 32, 6, 63, 34, "정보 수정");
		small_box(38, 46, 6, 42, 47, "로그아웃", 6);
		small_box(80, 46, 6, 86, 47, "다음", 6);
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10, 42, 47, "로그아웃", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 31 && yy < 37) {
					big_box(57, 18, 6, 66, 20, "예약", 6);
					big_box(57, 25, 6, 61, 27, "예약 내역 조회", 6);
					big_box(57, 32, 10, 63, 34, "정보 수정", 6);
					choice = 3;
					xx, yy = 0;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 17 && yy < 22) {
					big_box(57, 18, 10, 66, 20, "예약", 6);
					big_box(57, 25, 6, 61, 27, "예약 내역 조회", 6);
					big_box(57, 32, 6, 63, 34, "정보 수정", 6);
					choice = 1;
					xx, yy = 0;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(80, 46, 10, 86, 47, "다음", 6);
						Sleep(500);
						if (choice == 3) {
							modifying_membership(index);
							break;
						}
						else if (choice == 1) {
							designer_choice(index);
							break;
						}
					}
				}
			}
			xx = 0, yy = 0;
		}
		xx = 0, yy = 0;
	}

}
int member_login() { //회원 로그인 하는 부분
	box_clear();
	goto_xy(62, 5);
	textcolor(6);
	printf("회원 로그인");
	small_box(38, 46, 6,44,47,"이전",6);
	small_box(80, 46, 6,85,47,"로그인",6);
	char phone[20] = " ";
	char pw[15] = " ";
	int xx = 0, yy = 0, lr = 0;
	int check = 0;
	goto_xy(53, 19);
	printf("전화번호(11자리 ex) 010xxxxxxxx ");
	goto_xy(53, 21);
	printf(":");
	goto_xy(53, 28);
	printf("비밀번호(숫자 4자리) ex) 0000");
	goto_xy(53, 30);
	printf(":");
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10,44,47,"이전",6);
				Sleep(500);
				return;
			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 19 && yy < 23) {
				textcolor(6);
				goto_xy(55, 21);
				printf("                                 ");
				goto_xy(55, 21);
				EnableConsoleCursor();
				gets(phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(55, 21);
					printf("형식이 올바르지 않습니다..");
				}

			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 28 && yy < 32) {
				textcolor(6);
				goto_xy(55, 30);
				printf("                                 ");
				goto_xy(55, 30);
				EnableConsoleCursor();
				char ch;
				int i = 0;
				while (1) {
					ch = _getch();

					// Check if Enter key is pressed (ASCII value 13)
					if (ch == 13) {
						pw[i] = '\0'; // Null-terminate the password string
						break;
					}

					// Check if the input is a printable character (ASCII range 32 to 126)
					if (ch == 8 && i > 0) { // Backspace (ASCII value 8)
						i--;
						printf("\b \b"); // Move the cursor back, print space to erase the last character, move the cursor back again
					}
					else if (ch >= 32 && ch <= 126 && i < 4) { // Printable characters (ASCII range 32 to 126)
						printf("*");
						pw[i] = ch; // Store the character in the password array
						i++;
					}
				}
				HideCursor();
				check = isValidPhone_or_pw_Number(pw, 2);
				if (check == 0) {
					strcpy(pw, " ");
					goto_xy(55, 30);
					printf("형식이 올바르지 않습니다..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (strcmp(pw, " ") != 0 && strcmp(phone, " ") != 0) {
					small_box(80, 46, 10,85,47,"로그인",6);
					check = -1;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].phone, phone) == 0 && strcmp(all[i].pw, pw) == 0) {
							check = i;
							break;
						}
					}
					Sleep(500);
					if (check >= 0) {
						member_initial_screen(check);
						return 1;
					}
					else {
						small_box(80, 46, 6,85,47,"로그인",6);
						goto_xy(48, 40);
						textcolor(4);
						printf("전화번호 혹은 비밀번호를 다시 입력 해주세요.");
					}
				}

			}
		}
		xx = 0, yy = 0;
	}

}
void login_menu_choice() { // 로그인 선택 하는 부분 (회원 관리자 디자이너)
	int xx = 0, yy = 0, lr = 0;
	int choice = 0;
	int check = 0;
	while (1) {
		xx = 0, yy = 0;
		box_clear();
		goto_xy(64, 5);
		textcolor(6);
		printf("로그인");
		big_box(57, 15, 6,66,17,"회원",6);
		big_box(57, 23, 6,64,25,"디자이너",6);
		big_box(57, 31, 6,65,33,"관리자",6);
		small_box(38, 46, 6,44,47,"이전",6);
		small_box(80, 46, 6,86,47,"다음",6);
		//ExClick();
		while (1) {
			check = 0;
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10,44,47,"이전",6);
					Sleep(500);
					return;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 14 && yy < 19) {
					big_box(57, 15, 10,66,17,"회원", 6);
					big_box(57, 23, 6,64,25,"디자이너",6);
					big_box(57, 31, 6,65,33,"관리자",6);
					choice = 1;
				}
				else if (yy > 22 && yy < 28) {
					big_box(57, 15, 6, 66, 17, "회원", 6);
					big_box(57, 23, 10, 64, 25, "디자이너", 6);
					big_box(57, 31, 6, 65, 33, "관리자", 6);
					choice = 2;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(80, 46, 10,86,47,"다음", 6);
						Sleep(500);
						if (choice == 1) {
							check = member_login();
							choice = 0;
							break;
						}
						else if (choice == 2) {
							check = designer_login();
							choice = 0;
							break;
						}
					}
				}
			}
			xx = 0, yy = 0;
		}
		if (check == 1) {
			break;
		}
		xx = 0, yy = 0;
	}

} // 비밀번호 찾는 함수
void pw_find() { //비밀번호 찾기 
	box_clear();
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[15] = " ";
	int brith = 0;
	int check = 0;
	basic_UI(30,3);
	goto_xy(61, 5);
	printf("비밀번호 찾기");
	small_box(38, 46, 6,44,47,"이전",6);
	small_box(80, 46, 6,86,47,"찾기",6);
	goto_xy(53, 17);
	printf("이름 ex) 홍길동");
	goto_xy(53, 19);
	printf(":");
	goto_xy(53, 26);
	printf("전화번호 11자리 ex) 010xxxxxxxx ");
	goto_xy(53, 28);
	printf(":");
	goto_xy(53, 34);
	printf("생년월일(8자리) ex) 20030728 ");
	goto_xy(53, 36);
	printf(":");
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10,44,47,"이전",6);
				Sleep(500);
				return;
			}
		}

		if (xx > 53 && xx < 87) {
			if (yy > 18 && yy < 21) {
				strcpy(name, " ");
				textcolor(6);
				goto_xy(58, 19);
				printf("                                 ");
				goto_xy(58, 19);
				EnableConsoleCursor();
				gets(name);
				HideCursor();
			}
		}
		if (xx > 53 && xx < 87) {
			if (yy > 27 && yy < 30) {
				textcolor(6);
				goto_xy(58, 28);
				printf("                                 ");
				goto_xy(58, 28);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(58, 28);
					printf("형식이 올바르지 않습니다..");
				}

			}
		}
		if (xx > 53 && xx < 87) {
			if (yy > 34 && yy < 38) {
				textcolor(6);
				goto_xy(58, 36);
				printf("                                 ");
				goto_xy(58, 36);
				EnableConsoleCursor();
				scanf("%d", &brith);
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(58, 36);
					printf("형식이 올바르지 않습니다..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (strcmp(name, " ") != 0 &&  strcmp(phone, " ") != 0 && brith != 0) {
					check = -1;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].name,name) == 0 && strcmp(all[i].phone, phone) == 0 && all[i].brith == brith) {
							check = i;
						}
					}
					if (check >= 0) {
						small_box(80, 46, 10,86,47,"찾기",6);
						Sleep(500);
						box_clear();
						goto_xy(61, 5);
						printf("비밀번호 찾기");
						small_box(38, 46, 6,44,47,"이전",6);
						small_box(80, 46, 6,85,47,"로그인",6);
						goto_xy(54, 24);
						printf("%s님의 비밀번호는 %s입니다.", all[check].name,all[check].pw);
						xx = 0;
						yy = 0;
						click(&xx, &yy);
						while (1) {
							if (xx > 38 && xx < 53) {
								if (yy > 45 && yy < 49) {
									small_box(38, 46, 10,44,47,"이전",6);
									Sleep(500);
									break;
								}
							}
							else if (xx > 80 && xx < 95) {
								if (yy > 45 && yy < 49) {
									small_box(80, 46, 10,85,47,"로그인",6);
									Sleep(500);
									member_login();
									break;
								}
							}
						}
					}
					else {
						textcolor(4);
						goto_xy(49, 40);
						printf("일치하는 회원정보를 찾을 수 없습니다");
						continue;
					}
					break;
				}
			}
		}
		xx = 0, yy = 0;

	}
}
void membership() { // 회원가입 함수 
	HideCursor();
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[20] = " ";
	char gender[5] = " ";
	int brith = 0;
	char pw[15] = " ";
	int check = 0;
	goto_xy(62, 5);
	textcolor(6);
	printf("회원가입");
	small_box(50, 10, 6,56,11,"남자",9);
	small_box(67, 10, 6,73,11,"여자",12);
	small_box(38, 46, 6,44,47,"이전",6);
	small_box(80, 46, 6,86,47,"가입",6);
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
	//Mouse();
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10,44,47,"이전",6);
				Sleep(500);
				return;
			}
		}
		if (xx > 50 && xx < 64) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "남");
				small_box(67, 10, 6,73,11,"여자",12);
				small_box(50, 10, 10,56,11,"남자",9);
			}
		}
		if (xx > 67 && xx < 82) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "여");
				small_box(50, 10,6,56,11,"남자",9);
				small_box(67, 10, 10,73,11,"여자",12);
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
					if (check == 1) {
						textcolor(4);
						goto_xy(56, 41);
						printf("이미 가입된 회원 입니다.");
						continue;
					}
					else {
						small_box(80, 46, 10,86,47,"가입",6);
						Sleep(500);
						box_clear();
						goto_xy(62, 5);
						printf("회원가입");
						small_box(38, 46, 6,44,47,"이전",6);
						small_box(80, 46, 6,85,47,"로그인",6);
						goto_xy(54, 24);
						printf("회원가입이 완료되었습니다.");
						strcpy(all[member_count].name, name);
						strcpy(all[member_count].phone, phone);
						strcpy(all[member_count].pw, pw);
						strcpy(all[member_count].gender, gender);
						all[member_count].brith = brith;
						file_append();
						member_count += 1;
						while (1) {
							xx = 0;
							yy = 0;
							click(&xx, &yy);
							if (xx > 38 && xx < 53) {
								if (yy > 45 && yy < 49) {
									small_box(38, 46, 10,44,47,"이전",6);
									Sleep(500);
									break;
								}
							}
							else if (xx > 80 && xx < 95) {
								if (yy > 45 && yy < 49) {
									small_box(80, 46, 10,85,47,"로그인",6);
									Sleep(500);
									member_login();
									break;
								}
							}
						}
					}
					xx = 0;
					yy = 0;
					break;
				}
			}
		}		
	}
}
void initial_screen() { // 초기화면
	box_clear(30,3);
	HideCursor();
	int xx = 0, yy = 0, lr = 0;
	basic_UI(30,3);
	goto_xy(62, 5);
	printf("가든 헤어샵");
	big_box(57,20, 6, 66, 22,"예약");
	big_box(57, 27, 6, 61, 29,"예약 내역 조회");
	small_box(36, 46, 6, 41, 47, "로그인",6);
	small_box(59, 46, 6, 63, 47,"회원가입",6);
	small_box(82, 46, 6, 87, 47,"PW 찾기",6);
	Mouse();
	//ExClick();
	while (1) {
		xx = 0, yy = 0, lr = 0;
		click(&xx, &yy);
		if (xx > 59 && xx < 74) {
			if (yy > 46 && yy < 49) {
				small_box(59, 46, 10,63,47,"회원가입",6);
				Sleep(500);
				box_clear();
				membership();
				break;
			}
		}
		if (xx > 36 && xx < 50) {
			if (yy > 46 && yy < 49) {
				small_box(36, 46, 10,41,47,"로그인",6);
				Sleep(500);
				login_menu_choice();
				break;
			}
		}
		if (xx > 82 && xx < 97) {
			if (yy > 46 && yy < 49) {
				small_box(82, 46, 10,87,47,"PW 찾기",6);
				Sleep(500);
				pw_find();
				break;
			}
		}
		
	}
	xx = 0, yy = 0;
	return initial_screen();
}
int main(void) { //메인함수
	file_read();
	design_file_read();

	initial_screen();
}
