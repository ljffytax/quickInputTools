// ScanCode.cpp : Defines the entry point for the console application.
// ScanCode.cpp : 定义控制台应用程序的入口点。
//这个一个用于辅助输入纳税人识别号的小工具，另有其他一些小功能
//lianjie 2019-08-30

#include "verify.h"
#include "stdafx.h"
#include "ScanCode.h"

int safeReadCmd(char* buf, int bufSize, HANDLE hOut, PCONSOLE_SCREEN_BUFFER_INFO pCsbiInfo) {
	CONSOLE_SCREEN_BUFFER_INFO crBufferInfo;
	COORD coordCurPos;
	int i = 0;
	SMALL_RECT rcScroll;
	COORD coordDest;
	CHAR_INFO chiFill;
	DWORD dwFill;
	COORD coordPos = {0, pCsbiInfo->srWindow.Bottom - 2};
	char ctr = 0xF;
	char ctr1 = 0xF;
	coordCurPos = pCsbiInfo->dwCursorPosition;
	rcScroll.Left = 0;
	rcScroll.Top = 1;
	rcScroll.Right = pCsbiInfo->srWindow.Right;
	rcScroll.Bottom = pCsbiInfo->srWindow.Bottom-2;
	coordDest.X = 0;
	coordDest.Y = 0;
	chiFill.Attributes = pCsbiInfo->wAttributes;
	chiFill.Char.UnicodeChar = _T(' ');
	chiFill.Char.AsciiChar = (char)' ';
	for (i = 0; i < bufSize; i++) {
		ctr1 = _getch();
		GetConsoleScreenBufferInfo(hOut, &crBufferInfo);
		coordCurPos = crBufferInfo.dwCursorPosition;
		switch (ctr1)
		{
		case 0x0D: //Enter
			if (coordCurPos.Y >= crBufferInfo.srWindow.Bottom - 2) {
				//光标到达回显区底部，回显区要向上滚动一行
				ScrollConsoleScreenBuffer(hOut, &rcScroll, NULL, coordDest, &chiFill);
				SetConsoleCursorPosition(hOut, coordPos);
			}
			else {
				printf("\n");
			}
			buf[i] = ctr1;
			break;
		case 0x08: //Back
			if (coordCurPos.X > 0) {
				coordCurPos.X--;
				SetConsoleCursorPosition(hOut, coordCurPos);
				FillConsoleOutputCharacter(hOut, _T(' '), 1, coordCurPos, &dwFill);
				buf[i] = '\0';
				buf[i - 1] = '\0';
				i -= 2;
			}
			else
			{
				if (i >= crBufferInfo.srWindow.Right) {
					coordCurPos.Y--;
					coordCurPos.X = crBufferInfo.srWindow.Right;
					SetConsoleCursorPosition(hOut, coordCurPos);
					FillConsoleOutputCharacter(hOut, _T(' '), 1, coordCurPos, &dwFill);
					buf[i] = '\0';
					buf[i - 1] = '\0';
					i -= 2;
				}
			}
			break;
			//K_SPACE = 0x0020,
			//K_UP = 0xE048,
			//K_LEFT = 0xE04B,
			//K_RIGHT = 0xE04D,
			//K_HOME = 0xE047,
			//K_END = 0xE04F,
			//K_PGUP = 0xE049,
			//K_PGDN = 0xE051,
			//K_INS = 0xE052,
			//K_DOWN = 0xE050,
		case -0x20: //0xE0xx
			ctr = _getch();
			switch (ctr){
				case 0x4B: //left
					if (coordCurPos.X > 0) {
						coordCurPos.X--;
						SetConsoleCursorPosition(hOut, coordCurPos);
						i -= 2;
					}
					else
					{
						if (i >= crBufferInfo.srWindow.Right) {
							coordCurPos.Y--;
							coordCurPos.X = crBufferInfo.srWindow.Right;
							SetConsoleCursorPosition(hOut, coordCurPos);
							FillConsoleOutputCharacter(hOut, _T(' '), 1, coordCurPos, &dwFill);
							i -= 2;
						}
					}
					break;
				case 0x4D: //right
				// TODO：
					if (coordCurPos.X >= 0 && coordCurPos.X < i) {
						coordCurPos.X++;
						SetConsoleCursorPosition(hOut, coordCurPos);
					}
					else
					{
						if (i >= crBufferInfo.srWindow.Right) { //字符数多于一行
							coordCurPos.Y++;
							coordCurPos.X = 0;
							SetConsoleCursorPosition(hOut, coordCurPos);
						}
					}
					break;
				case 0x48: //up
					if (pCur == UCSCBuffer && UCSCBuffer[UCSC_BUFFER_SIZE-1][0] != '\0'){ 
						//buffer is full
						pCur += UCSC_BUFFER_SIZE-1;
					}
					else{
						if (pCur != UCSCBuffer){
							pCur --;
						}
					}
					if (**pCur){
						i = 0;
						for (int n=0; n<UCSC_SIZE; n++)	{
							printf("%c",*(*pCur+n));
							buf[n] = *(*pCur+n);
							i=n;
						}
						i--;
					}
					coordCurPos.X = 0;
					SetConsoleCursorPosition(hOut, coordCurPos);
					break;
				case 0x50: //down
					if (pCur == &UCSCBuffer[UCSC_BUFFER_SIZE-1]){// last of buff
						pCur = UCSCBuffer;
					}
					else{
						if (**(pCur+1) != '\0'){ 
							//not the last one 
							pCur ++;
						}
					}
					if (**pCur){ 
						i = 0;
						for (int n=0; n<UCSC_SIZE; n++)	{
							printf("%c",*(*pCur+n));
							buf[n] = *(*pCur+n);
							i=n;
						}
						i--;
					}
					coordCurPos.X = 0;
					SetConsoleCursorPosition(hOut, coordCurPos);
					break;
				default:
					break;
			}
			break;
		default:
			if (buf[i] == '\0') {
				buf[i] = ctr1;
			}
			else { // insert modle
				int k = BUFF_SIZE - 1;
				do {
					buf[k] = buf[k - 1];
					k--;
				} while (k >= i);
				buf[i] = ctr1;
			}
			if (coordCurPos.Y >= crBufferInfo.srWindow.Bottom - 2 && coordCurPos.X == crBufferInfo.srWindow.Right) {
				printf("%s", &buf[i]);
				ScrollConsoleScreenBuffer(hOut, &rcScroll, NULL, coordDest, &chiFill);
				SetConsoleCursorPosition(hOut, coordPos);
			}
			else {
				printf("%s", &buf[i]);
				//coordPos.X -= 
				SetConsoleCursorPosition(hOut, coordPos);
			}
			break;
		}
		if (buf[i] == '\n' || buf[i] == EOF || buf[i] == 0x0D) {
			buf[i] = '\0';
			break;
		}
		if (i == bufSize - 1) {
			while ((buf[bufSize - 1] = _getch()) != '\n' && buf[bufSize - 1] != EOF);
			buf[bufSize - 1] = '\0';
			break;
		}
	}
	return i;
}

int copyData2Clipboard(char* data, int length) {
	//该长度值应该包含末尾'\0'
	int copyLength = length;
	if (length <= 0) return -1;
	if (OpenClipboard(NULL)) {
		EmptyClipboard();
		HGLOBAL mStr = GlobalAlloc(GMEM_FIXED, length * sizeof(char));
		if (mStr != NULL) {
			memcpy(mStr, data, length * sizeof(char));
			if (SetClipboardData(CF_TEXT, mStr) == NULL) {
				copyLength = -1;
			}
		}
		CloseClipboard();
	}
	else {
		copyLength = -1;
	}
	return copyLength;
}

void updateIdentificationCardNumber(void) {
	char buf[BUFF_SIZE];
	int i = 0;
	char IDCode[19];
	bool isBadIDCode = false;
	while (1) {
		memset(buf, 0x00, BUFF_SIZE);
		memset(IDCode, 0x00, 19);
		isBadIDCode = false;
		printf("请输入15位身份证号:  输入Q返回上一层\n");
		//i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
		if (i == 15) {
			for (int n = 0; n<15; n++) {
				if (buf[n]<0x30 || buf[n]>0x39) isBadIDCode = true;
				if (n<6) IDCode[n] = buf[n];
				if (n >= 6) IDCode[n + 2] = buf[n];
			}
			if (isBadIDCode) continue;
			IDCode[6] = '1';
			IDCode[7] = '9';
			IDCode[17] = getLastCodeOfIdentificationCardNumber(IDCode);
			IDCode[18] = '\0';
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			printf("%s\n", IDCode);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		}
		if (i == 1 && buf[0] == 'Q') {
			break;
		}
	}
}

void organizationCode2UnifiedCodeOfSocialCredit(void) {
	char buf[BUFF_SIZE];
	int i = 0;
	bool isBadOrgCode = false;
	char type = '0';
	char UCSCCode[UCSC_SIZE];
	while (1) {
		memset(buf, 0x00, BUFF_SIZE);
		memset(UCSCCode, 0x00, UCSC_SIZE);
		isBadOrgCode = false;
		printf("请输入9位组织机构代码:  输入Q返回上一层\n");
		//i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
		if (i == 9) {
			for (int n = 0; n<9; n++) {
				if (buf[n] >= 0x30 && buf[n] <= 0x39 ||
					buf[n] >= 0x41 && buf[n] <= 0x5A ||
					buf[n] >= 0x61 && buf[n] <= 0x7A) {
					UCSCCode[n + 8] = buf[n];
				}
				else {
					isBadOrgCode = true;
				}
			}
			if (isBadOrgCode) continue;
			printf("请输入组织机构类型: 1为公司，2为个体  输入Q返回上一层\n");
			//i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
			if (i == 1 && (buf[0] == '1' || buf[0] == '2' || buf[0] == 'Q')) {
				if (buf[0] == 'Q') {
					break;
				}
				UCSCCode[1] = buf[0];
			}
			else {
				continue;
			}
			UCSCCode[0] = '9';
			UCSCCode[2] = '4';
			UCSCCode[3] = '4';
			UCSCCode[4] = '0';
			UCSCCode[5] = '3';
			UCSCCode[6] = '0';
			UCSCCode[7] = '0';
			uperChar(UCSCCode, UCSC_SIZE);
			UCSCCode[17] = getLastCodeOfUCSC(UCSCCode);
			if (UCSCCode[17] == '\0') {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				printf("组织机构代码不正确！\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
				continue;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			printf("%s\n", UCSCCode);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			if (copyData2Clipboard(UCSCCode, UCSC_SIZE) == -1) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				printf("数据拷贝至粘贴板错误！\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else {
				printf("统一信用代码已拷贝至粘贴板！\n");
			}
		}
		if (i == 1 && buf[0] == 'Q') {
			break;
		}
	}
}

void regMode(void) {
	char buf[BUFF_SIZE];
	int i = 0;
	while (1) {
		memset(buf, 0x00, BUFF_SIZE);
		printf("寄存模式:  输入Q返回上一层\n");
		//i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
		if (i == 1 && buf[0] == 'Q') {
			break;
		}
		else {
			if (copyData2Clipboard(buf, i + 1) == -1) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				printf("数据拷贝至粘贴板错误！\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else {
				printf("数据已拷贝至粘贴板！\n");
			}
		}
	}
	return;
}

void verifyIdentificationCardNumber(void) {
	char buf[BUFF_SIZE];
	int i = 0;
	while (1) {
		memset(buf, 0x00, BUFF_SIZE);
		printf("请输入18位身份证号:  输入Q返回上一层\n");
		//i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
		if (i == 18) {
			if (isIdentificationCardNumber(buf)) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
				printf("身份证号校验通过！\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
				printf("身份证号码不正确！\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
		}
		if (i == 1 && buf[0] == 'Q') {
			break;
		}
	}
}

void printMenu() {
	printf("+-------------------------------+-----------------+\n");
	printf("| 1、校验身份证号码             | 2、身份证升位   |\n");
	printf("+-------------------------------+-----------------+\n");
	printf("| 3、组织机构代码转统一信用代码 | 4、寄存模式     |\n");
	printf("+-------------------------------+-----------------+\n");
	printf("|                输入Q返回上一层                  |\n");
	printf("+-------------------------------------------------+\n");
	return;
}

void drawStatusBar(HANDLE hOut){
	// 状态条显示  
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	DWORD NumberOfWritten;
	GetConsoleScreenBufferInfo(hOut, &bInfo);
	COORD home = { 0, bInfo.srWindow.Bottom - bInfo.srWindow.Top - 1 }; // 倒数第二行  
	WORD att0 = BACKGROUND_INTENSITY;
	WORD att1 = FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED;
	FillConsoleOutputAttribute(hOut, att0, bInfo.srWindow.Right - bInfo.srWindow.Left + 1, home, &NumberOfWritten);
	FillConsoleOutputCharacter(hOut, _T(' '), bInfo.srWindow.Right - bInfo.srWindow.Left + 1, home, &NumberOfWritten);
	SetConsoleTextAttribute(hOut, att1);

	// 状态信息显示  
	COORD staPos = {0,bInfo.srWindow.Bottom - bInfo.srWindow.Top - 1 };
	SetConsoleCursorPosition(hOut, staPos);
	TCHAR cc[6] = {'S','T','A','T',':','\0'};
	WriteConsole(hOut, cc, 5, NULL, NULL);
	staPos.X += (bInfo.srWindow.Right - bInfo.srWindow.Left)/2-10;
	SetConsoleCursorPosition(hOut, staPos);
	cc[0] = 'C';
	cc[1] = 'O';
	cc[2] = 'D';
	cc[3] = 'E';
	cc[4] = ':';
	WriteConsole(hOut, cc, 5, NULL, NULL);
	staPos.X = bInfo.srWindow.Right - bInfo.srWindow.Left - 5;
	SetConsoleCursorPosition(hOut, staPos);
	cc[0] = 'M';
	cc[1] = 'E';
	cc[2] = 'N';
	cc[3] = 'U';
	cc[4] = ':';
	WriteConsole(hOut, cc, 5, NULL, NULL);
	// 环境恢复  
	SetConsoleTextAttribute(hOut, bInfo.wAttributes);
	SetConsoleCursorPosition(hOut, bInfo.dwCursorPosition);
}

void ErrorExit(LPTSTR lpszFunction) {
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

void statusBar(HANDLE hStdout, PCONSOLE_SCREEN_BUFFER_INFO pCsbiInfo) {
	COORD coordCurPos;
	drawStatusBar(hStdout);
	if (!GetConsoleScreenBufferInfo(hStdout, pCsbiInfo))
	{
		printf("GetConsoleScreenBufferInfo failed %d\n", GetLastError());
		return ;
	}
	coordCurPos = pCsbiInfo->dwCursorPosition;
	//如果光标到达状态栏的上一行，则不能让光标继续往下走破坏状态栏
	if (coordCurPos.Y >= pCsbiInfo->srWindow.Bottom - 2) {
		DWORD NumberOfWritten;
		COORD home = { 0, pCsbiInfo->srWindow.Bottom - pCsbiInfo->srWindow.Top - 2 }; // 倒数第3行  
		WORD att0 = pCsbiInfo->wAttributes;
		coordCurPos.Y -= 2; //up 1 row
		SetConsoleCursorPosition(hStdout, coordCurPos);
		FillConsoleOutputAttribute(hStdout, att0, pCsbiInfo->srWindow.Right - pCsbiInfo->srWindow.Left + 1, home, &NumberOfWritten);
		FillConsoleOutputCharacter(hStdout, _T(' '), pCsbiInfo->srWindow.Right - pCsbiInfo->srWindow.Left + 1, home, &NumberOfWritten);
	}
}

void printCopyright() {
	printf("=========================================================\n");
	printf("     扫入营业执照后按Ctrl+v即可以粘贴统一信用代码\n");
	printf("     适用大多数版本的营业执照   输入M可用更多功能\n");
	printf("     Author: lianjie 2019-08-30\n");
	printf("=========================================================\n");
	return;
}

void print2StatusBar(char *str, int strLen, int msgType,
	HANDLE hOut, PCONSOLE_SCREEN_BUFFER_INFO pCsbiInfo){
	COORD staPos = { pCsbiInfo->srWindow.Right - pCsbiInfo->srWindow.Left,
		pCsbiInfo->srWindow.Bottom - pCsbiInfo->srWindow.Top - 1 };
	TCHAR cc[25] = {};
	WORD att1 = BACKGROUND_INTENSITY;
	SetConsoleTextAttribute(hOut, att1);
	int i;
	for (i = 0; i < 25; i++) {
		cc[i] = *(str + i);
		if ('\0' == cc[i] || i == strLen) {
			break;
		}
	}
	cc[24] = '\0';
	switch (msgType){
		case MSG_IS_INFO:
			staPos.X = 6;
			SetConsoleCursorPosition(hOut, staPos);
			WriteConsole(hOut, cc, i, NULL, NULL);
			break;
		case MSG_IS_CODE:
			att1 = FOREGROUND_BLUE | BACKGROUND_INTENSITY;
			SetConsoleTextAttribute(hOut, att1);
			staPos.X = (pCsbiInfo->srWindow.Right - pCsbiInfo->srWindow.Left) / 2 - 4;
			SetConsoleCursorPosition(hOut, staPos);
			WriteConsole(hOut, cc, i, NULL, NULL);
			break;
		default:
			break;
	}
	SetConsoleTextAttribute(hOut, pCsbiInfo->wAttributes);
	SetConsoleCursorPosition(hOut, pCsbiInfo->dwCursorPosition);
	return;
}

char(* addUCSC2Buffer(char(*pBuf)[UCSC_SIZE], char(*pTail)[UCSC_SIZE], char* ucsc))[UCSC_SIZE]{
	int i = 0;
	//already in buff
	for (int n=0; n<UCSC_BUFFER_SIZE; n++){
		if (0 == strncmp(*(pBuf+n), ucsc, UCSC_SIZE)){
			return pTail;
		}
	}
	for (i=0; i<UCSC_SIZE; i++){
		*(*pTail+i) = *(ucsc+i);
	}
	if (pTail == pBuf+UCSC_BUFFER_SIZE-1){ //数组写满
		pTail = pBuf;
	}
	else{
		pTail ++;
	}
	return pTail;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char buf[BUFF_SIZE];
	int len = BUFF_SIZE;
	int i = 0;
	int num = 0;
	char taxCode[MAX_COUNT_UCSC][UCSC_SIZE];
	
	RECT rc;
	SMALL_RECT smallRc = { 0,0, MAX_COL - 1, MAX_ROW - 1 };
	COORD size = { MAX_COL,MAX_ROW };
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	HANDLE hStdout;

	//Do something for window setting...
	HWND hWnd = GetConsoleWindow();
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!SetConsoleWindowInfo(hStdout, true, &smallRc)) {
		ErrorExit(TEXT("SetConsoleWindowInfo"));
	}
	if (!SetConsoleScreenBufferSize(hStdout, size)) {
		ErrorExit(TEXT("SetConsoleScreenBufferSize"));
	}
	GetWindowRect(hWnd, &rc); //获得cmd窗口对应矩形
	SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &
		~WS_THICKFRAME & ~WS_MAXIMIZEBOX);
	SetWindowPos(hWnd, NULL, rc.left, rc.top,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL);
	SetConsoleTitle(L"Hello Kitty");


	//===============业务逻辑==============
	printCopyright();
	statusBar(hStdout, &csbiInfo);
	pUscsBuf = UCSCBuffer;
	pCur = pUscsBuf;
	memset(UCSCBuffer, 0x00, UCSC_BUFFER_SIZE*UCSC_SIZE);
	while (1) {
		memset(taxCode, 0x00, UCSC_SIZE*MAX_COUNT_UCSC);
		memset(buf, 0x00, BUFF_SIZE);
		num = 0;
		//printf ("等待扫枪扫入营业执照:\n");
		print2StatusBar("Waiting...", 17, MSG_IS_INFO, hStdout, &csbiInfo);
		i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
		if (i == 0) {
			break;
		}
		statusBar(hStdout, &csbiInfo);
		if (i == 1 && buf[0] == 'M') { //进入菜单
			while (1) {
				do {
					printMenu();
					i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
				} while (i != 1 || buf[0] < 0x31 || buf[0] > 0x34 && buf[0] != 'Q');
				switch (buf[0]) {
				case '1': //1、校验身份证
					verifyIdentificationCardNumber();
					break;
				case '2': //2、身份证升位
					updateIdentificationCardNumber();
					break;
				case '3': //3、组织机构代码转统一信用代码
					organizationCode2UnifiedCodeOfSocialCredit();
					break;
				case '4': //4、寄存模式
					regMode();
					break;
				case 'Q': //返回
					break;
				}
				if (buf[0] == 'Q') break;
			}
		}
		else {
			uperChar(buf, BUFF_SIZE);
			if (i < UCSC_SIZE - 1) {
				print2StatusBar("No UCSC Code!", 14, MSG_IS_CODE, hStdout, &csbiInfo);
				continue;
			}
			if (i == IDCN_SIZE - 1 && isIdentificationCardNumber(buf)) { //18位身份证号
				pUscsBuf = addUCSC2Buffer(UCSCBuffer, pUscsBuf, buf);
				print2StatusBar(buf, IDCN_SIZE, MSG_IS_CODE, hStdout, &csbiInfo);
				if (copyData2Clipboard(buf, IDCN_SIZE) == -1) {
					print2StatusBar("Clipboard Error!!!", 19, MSG_IS_CODE, hStdout, &csbiInfo);
				}
				continue;
			}
			i = findAllUCSC(buf, taxCode, BUFF_SIZE);
			if (i == 0) {
				print2StatusBar("No UCSC Code!", 14, MSG_IS_CODE, hStdout, &csbiInfo);
			}
			else {
				if (i>1) {
					printf("共找到%d个识别号,请选择:\n", i);
					for (int n = 0; n<i; n++) {
						printf("%d | %s\n", n + 1, taxCode[n]);
					}
					i = safeReadCmd(buf, BUFF_SIZE, hStdout, &csbiInfo);
					if (i>0 && buf[0] <= 0x35 && buf[0] >= 0x31) { //目前最多5个识别号
						num = buf[0] - 0x31;
					}
					else {
						num = 0;
					}
				}
				taxCode[num][UCSC_SIZE] = '\0';
				pUscsBuf = addUCSC2Buffer(UCSCBuffer, pUscsBuf, taxCode[num]);
				print2StatusBar(taxCode[num], UCSC_SIZE, MSG_IS_CODE, hStdout, &csbiInfo);
				//print2StatusBar(UCSCBuffer[num], UCSC_SIZE, MSG_IS_CODE, hStdout, &csbiInfo);
				if (copyData2Clipboard(taxCode[num], UCSC_SIZE) == -1) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
					printf("数据拷贝至粘贴板错误！\n");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | 
						FOREGROUND_RED | FOREGROUND_GREEN);
				}
			}
		}
	}
	return 0;
}