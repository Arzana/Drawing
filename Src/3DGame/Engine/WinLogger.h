#pragma once
#define LogMsg_Eng(format, ...)	LogMsgAs("ENGINE", format, __VA_ARGS__)

void LogMsgAs(const char *as, const char *format, ...);