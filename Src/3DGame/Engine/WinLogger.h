#pragma once

#define CMD_CLR_DARKBLUE			1
#define CMD_CLR_DARKGREEN			2
#define CMD_CLR_DARKCYAN			3
#define CMD_CLR_DARKRED				4
#define CMD_CLR_DARKMAGENTA			5
#define CMD_CLR_DARKYELLOW			6
#define CMD_CLR_DARKWHITE			7
#define CMD_CLR_DARKGRAY			8
#define CMD_CLR_BLUE				9
#define CMD_CLR_GREEN				10
#define CMD_CLR_CYAN				11
#define CMD_CLR_RED					12
#define CMD_CLR_MAGENTA				13
#define CMD_CLR_YELLOW				14
#define CMD_CLR_WHITE				15

#define DISPLNAME_ENGINE			"ENGINE"
#define DISPLNAME_GF				"GRAPHICS"
#define DISPLNAME_GAME				"GAME"

#define LogVerAs(as, format, ...)	LogAs(as, CMD_CLR_DARKGREEN, format, __VA_ARGS__)
#define LogMsgAs(as, format, ...)	LogAs(as, CMD_CLR_DARKWHITE, format, __VA_ARGS__)
#define LogWarAs(as, format, ...)	LogAs(as, CMD_CLR_DARKYELLOW, format, __VA_ARGS__)
#define LogErrAs(as, format, ...)	LogAs(as, CMD_CLR_DARKRED, format, __VA_ARGS__)

#define LogVer_Eng(format, ...)		LogVerAs(DISPLNAME_ENGINE, format, __VA_ARGS__)
#define LogMsg_Eng(format, ...)		LogMsgAs(DISPLNAME_ENGINE, format, __VA_ARGS__)
#define LogWar_Eng(format, ...)		LogWarAs(DISPLNAME_ENGINE, format, __VA_ARGS__)
#define LogErr_Eng(format, ...)		LogErrAs(DISPLNAME_ENGINE, format, __VA_ARGS__)

#define LogVer_GF(format, ...)		LogVerAs(DISPLNAME_GF, format, __VA_ARGS__)
#define LogMsg_GF(format, ...)		LogMsgAs(DISPLNAME_GF, format, __VA_ARGS__)
#define LogWar_GF(format, ...)		LogWarAs(DISPLNAME_GF, format, __VA_ARGS__)
#define LogErr_GF(format, ...)		LogErrAs(DISPLNAME_GF, format, __VA_ARGS__)

#define LogVer_Game(format, ...)	LogVerAs(DISPLNAME_GAME, format, __VA_ARGS__)
#define LogMsg_Game(format, ...)	LogMsgAs(DISPLNAME_GAME, format, __VA_ARGS__)
#define LogWar_Game(format, ...)	LogWarAs(DISPLNAME_GAME, format, __VA_ARGS__)
#define LogErr_Game(format, ...)	LogErrAs(DISPLNAME_GAME, format, __VA_ARGS__)

void LogAs(const char *as, int clr, const char *format, ...);