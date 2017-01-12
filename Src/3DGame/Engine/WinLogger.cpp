#pragma warning(disable: 4996)

#include <ctime>
#include <cstdio>
#include <stdarg.h>
#include <Windows.h>
#include "WinLogger.h"

void LogAs(const char * as, int clr, const char * format, ...)
{
	HANDLE hndlConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hndlConsole, clr);

	const time_t t = time(0);
	const struct tm *now = localtime(&t);
	printf("[%.4u-%.2u-%.2u %.2u:%.2u:%.2u][%s] ", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, as);
	
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	printf("\n");
	SetConsoleTextAttribute(hndlConsole, 7);
}