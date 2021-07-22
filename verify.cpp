#include "verify.h"

void uperChar(char *str, int strLength) {
	//小写转大写
	for (int n = 0; n < strLength; n++) {
		if (*(str + n) >= 0x61 && *(str + n) <= 0x7A) {
			*(str + n) = *(str + n) - 32;
		}
	}
}

int char2Num(char c) {
	char ch[21] = { 'A','B','C','D','E','F','G',
		'H','J','K','L','M','N','P',
		'Q','R','T','U','W','X','Y' };
	for (int n = 0; n<21; n++) {
		if (c == ch[n]) {
			return n + 10;
		}
	}
	return 0;
}

char getLastCodeOfIdentificationCardNumber(char *p) {
	char valCodeArr[11] = { '1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2' };
	int wi[17] = { 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 };
	int totalmulAiWi = 0;
	for (int n = 0; n<17; n++) {
		totalmulAiWi = (int(*(p + n)) - 0x30) * wi[n] + totalmulAiWi;
	}
	return valCodeArr[totalmulAiWi % 11];
}

char getLastCodeOfUCSC(char *p) {
	//社会统一信用代码校验规则,返回最后一位效验位，错误则返回'\0'
	//GB 32100-2015
	int power[17] = { 1, 3, 9, 27, 19, 26, 16, 17, 20,
		29, 25, 13, 8, 24, 10, 30, 28 };
	char c[21] = { 'A','B','C','D','E','F','G',
		'H','J','K','L','M','N','P',
		'Q','R','T','U','W','X','Y' };
	char code[18] = { 0 };
	int sum17 = 0;
	int temp = 0;
	if (*p != 0x31 && *p != 0x35 && *p != 0x39 && *p != 0x59) {
		return '\0';
	}
	if (*(p + 1) != 0x31 && *(p + 1) != 0x32 && *(p + 1) != 0x33 && *(p + 1) != 0x39) {
		return '\0';
	}
	for (int n = 2; n<8; n++) { //3-8位为纯数字
		if (*(p + n) < 0x30 || *(p + n) > 0x39) {
			return '\0';
		}
	}
	for (int n = 0; n<17; n++) {
		code[n] = *(p + n);
		if (*(p + n) >= 0x3A && *(p + n) <= 0x40 ||
			*(p + n) < 0x30 || *(p + n) > 0x5A) {
			return '\0';
		}
		if (*(p + n) == 'I' || *(p + n) == 'O' || *(p + n) == 'S' ||
			*(p + n) == 'V' || *(p + n) == 'Z') {
			return '\0';
		}
	}
	for (int n = 0; n<17; n++) {
		if (code[n] > 0x40) {
			sum17 = sum17 + char2Num(code[n]) * power[n];
		}
		else {
			sum17 = sum17 + (code[n] - 0x30) * power[n];
		}
	}
	temp = sum17 % 31;
	if (temp != 0) {
		temp = 31 - temp;
	}
	if (temp < 10) {
		return char(temp + 0x30);
	}
	else {
		return c[temp - 10];
	}
}

bool isUnifiedCodeOfSocialCredit(char *p) {
	//社会统一信用代码校验规则
	char verifyCode = '\0';
	verifyCode = getLastCodeOfUCSC(p);
	if (*(p + 17) == getLastCodeOfUCSC(p) && verifyCode != '\0') {
		return true;
	}
	else {
		return false;
	}
}

bool isIdentificationCardNumber(char* p) {
	for (int i = 0; i<17; i++) {
		if (*(p + i) < 0x30 || *(p + i)>0x39) return false;
	}
	if (*(p + 17) == getLastCodeOfIdentificationCardNumber(p)) {
		return true;
	}
	else {
		return false;
	}
}

char* searchUCSC(char *str, int strLength) {
	char subStr[18] = { 0 };
	char *pCode = NULL;
	if (strLength >= 18) {
		for (int k = 0; k<strLength - 18; k++) {
			for (int n = 0; n<18; n++) {
				subStr[n] = *(str + n + k);
			}
			if (isUnifiedCodeOfSocialCredit(subStr)) {
				pCode = str + k;
				break;
			}
		}
	}
	return pCode;
}

int findAllUCSC(char *buf, char(*des)[UCSC_SIZE], int bufSize) {
	char *p = NULL;
	char *p0 = NULL;
	int i = 0;
	int j = 0;
	int numberOfUCSC = 0;
	p0 = buf;
	while (i <= bufSize - UCSC_SIZE && numberOfUCSC<MAX_COUNT_UCSC) {
		p = searchUCSC(buf, bufSize - j);
		if (p != NULL) {
			for (int n = 0; n<UCSC_SIZE - 1; n++) {
				*(*(des + numberOfUCSC) + n) = *(p + n);
			}
			numberOfUCSC++;
			while (p0 != p) {
				p0++;
				j++;
			}
			i = j;
			buf = p + 1;
		}
		else {
			break;
		}
	}
	return numberOfUCSC;
}

