#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include <termios.h>

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "lpcli.h"

int lpcli_clipboardcopy(const char *text)
{
	FILE *pout = popen("xclip -selection clipboard -quiet -loop 1", "w");
	if(!pout)
	{
		return LPCLI_FAIL;
	}
	fprintf(pout, text);
	fflush(pout);
	pclose(pout);

	return LPCLI_OK;
}

int lpcli_readpassword(const char *prompt, char *out, size_t outl)
{
	printf(prompt);
	static struct termios told, tnew;
	tcgetattr(0, &told);
	tnew = told;
	tnew.c_lflag &= ~ICANON;
	tnew.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &tnew);
	
	out = fgets(out, outl, stdin);
	tcsetattr(0, TCSANOW, &told);
	
	if(!out)
		return LPCLI_FAIL;

	out[strcspn(out, "\r\n")] = 0;
	printf("\n");
	return LPCLI_OK;
}

typedef void* (*lpcli_memset_f) (void*, int, size_t);
static volatile lpcli_memset_f lpcli_memset = memset;
void* lpcli_zeromemory(void *dst, size_t dstlen)
{
	return lpcli_memset(dst, 0, dstlen);
}

int main(int argc, const char **argv)
{
	setlocale(LC_ALL, "");
	int ret = lpcli_main(argc, argv);
	return ret;
}
