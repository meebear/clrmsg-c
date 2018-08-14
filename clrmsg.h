#ifndef _CLR_MSG_H_
#define _CLR_MSG_H_

#include <stdio.h>

#define BLA   "\033[1;30m"
#define RED   "\033[1;31m"
#define GRN   "\033[1;32m"
#define YEL   "\033[1;33m"
#define BLU   "\033[1;34m"
#define MAG   "\033[1;35m"
#define CYN   "\033[1;36m"
#define WHT   "\033[1;37m"
#define BLA_D "\033[2;30m"
#define RED_D "\033[2;31m"
#define GRN_D "\033[2;32m"
#define YEL_D "\033[2;33m"
#define BLU_D "\033[2;34m"
#define MAG_D "\033[2;35m"
#define CYN_D "\033[2;36m"
#define WHT_D "\033[2;37m"
#define RST   "\033[0m"

int cmMsg_(int (*print)(const char *fmt, ...), const char *fmt, ...);

#define cmPrintf(fmt, args...) cmMsg_(printf, fmt, ##args)

#ifndef __STANDALONE_TEST__
#include "debug.h"
#define cmMessage(fmt, args...) cmMsg_(message, fmt, ##args)
#endif

#endif
