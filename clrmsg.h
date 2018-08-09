#ifndef _CLR_MSG_H_
#define _CLR_MSG_H_

#include <stdio.h>

#define RED  "\033[1;31m"
#define GRN  "\033[1;32m"
#define YEL  "\033[1;33m"
#define BLU  "\033[1;34m"
#define MAG  "\033[1;35m"
#define CYN  "\033[1;36m"
#define WHT  "\033[1;37m"
#define RED_D  "\033[2;31m"
#define GRN_D  "\033[2;32m"
#define YEL_D  "\033[2;33m"
#define BLU_D  "\033[2;34m"
#define MAG_D  "\033[2;35m"
#define CYN_D  "\033[2;36m"
#define WHT_D  "\033[2;37m"
#define RST  "\033[0m"

const char* cmFmt(const char *fmt, char *newFmt, size_t sz);
const char* cmFmtClear(const char *fmt, char *newFmt, size_t sz);

int cmPrintf(const char *fmt, ...);

int cmMsg_(int (*print)(const char *fmt, ...), const char *fmt, ...);

extern int (*cm_print)(const char *fmt, ...);
#define cmSetPrtFunc(prt) do { cm_print = prt; } while (0)

#define cmMsg(fmt, args...) \
    cmMsg_(cm_print, fmt, ##args)

#define cmInfo(fmt, args...) \
    cmMsg_(cm_print, "info{Green} "fmt, ##args)

#define cmWarn(fmt, args...) \
    cmMsg_(cm_print, "warn{Yellow} "fmt, ##args)

#define cmErr(fmt, arg...) \
    cmMsg_(cm_print, "err{Red} "fmt, ##arg)

#define cmDbg(fmt, arg...) \
    cmMsg_(cm_print, "%s{Cyan}:%d{Cyan} " fmt, __FUNCTION__,__LINE__,##arg)

#define cmMsgRed(fmt, arg...) \
    cmMsg_(cm_print, "{Red}"fmt, ##arg)

#endif
