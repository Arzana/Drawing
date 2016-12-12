#pragma once
#define LogMsg_Eng(...)	LogMsgAs("ENGINE", __VA_ARGS__)

void LogMsgAs(const char *as, const char *format, ...);