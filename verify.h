#include "stdafx.h"

#define BUFF_SIZE 1024
#define UCSC_SIZE 19
#define MAX_COUNT_UCSC  5
#define IDCN_SIZE 19 //…Ì∑›÷§
#define MAX_ROW 26
#define MAX_COL 80
#define MSG_IS_INFO 30
#define MSG_IS_CODE 31
#define UCSC_BUFFER_SIZE 64


void uperChar(char *str, int strLength);
int char2Num(char c);
char getLastCodeOfIdentificationCardNumber(char *p);
char getLastCodeOfUCSC(char *p);
bool isUnifiedCodeOfSocialCredit(char *p);
bool isIdentificationCardNumber(char* p);
char* searchUCSC(char *str, int strLength);
int findAllUCSC(char *buf, char(*des)[UCSC_SIZE], int bufSize);