

char UCSCBuffer[UCSC_BUFFER_SIZE][UCSC_SIZE];
char(* pUscsBuf)[UCSC_SIZE];
char(* pCur)[UCSC_SIZE];
int safeReadCmd(char* buf, int bufSize, HANDLE hOut, PCONSOLE_SCREEN_BUFFER_INFO pCsbiInfo);
