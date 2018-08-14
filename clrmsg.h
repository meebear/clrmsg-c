#ifndef _CLR_MSG_H_
#define _CLR_MSG_H_

#include <stdio.h>

#define BLA   "\033[1;30m"  // Black
#define RED   "\033[1;31m"  // Red
#define GRN   "\033[1;32m"  // Green
#define YEL   "\033[1;33m"  // Yellow
#define BLU   "\033[1;34m"  // Blue
#define MAG   "\033[1;35m"  // Magenta
#define CYN   "\033[1;36m"  // Cyan
#define WHT   "\033[1;37m"  // White
#define BLA_D "\033[2;30m"  // black
#define RED_D "\033[2;31m"  // red
#define GRN_D "\033[2;32m"  // green
#define YEL_D "\033[2;33m"  // yellow
#define BLU_D "\033[2;34m"  // blue
#define MAG_D "\033[2;35m"  // magenta
#define CYN_D "\033[2;36m"  // cyan
#define WHT_D "\033[2;37m"  // white
#define RST   "\033[0m"     // Reset

#define BG_BLA "\033[1;40m"  // Black
#define BG_RED "\033[1;41m"  // Red
#define BG_GRN "\033[1;42m"  // Green
#define BG_YEL "\033[1;43m"  // Yellow
#define BG_BLU "\033[1;44m"  // Blue
#define BG_MAG "\033[1;45m"  // Magenta
#define BG_CYN "\033[1;46m"  // Cyan
#define BG_WHT "\033[1;47m"  // White

/* inline color formmating:
 *  %{color}
 *          -- start using 'color' from here on
 *  %{color:content}
 *          -- use 'color' for 'content' only, will be reset after
 *             spaces in 'content' are reserved
 *             printf format specifier can be used in 'content'
 *
 *  'color' can be in format
 *      fgclr
 *          -- font color
 *      fgclr@bgclr
 *          -- font color on top of background color
 *      @bgclr
 *          -- specify only background color
 *
 *  'color' eigher fgclr or bgclr should be 'Red', 'Green', 'blue', etc...
 *          refer to comment of the defines above
 */

typedef int (*CmPrint)(const char *, ...);

int cmMsg_(CmPrint print, const char *fmt, ...);

const char* cmFmtClear(const char *fmt, char *newFmt, size_t sz);

void cmSetPrtFunc(CmPrint print);

#define cmPrintf(fmt, args...) cmMsg_(printf, fmt, ##args)

#define cmMsg(fmt, args...) \
	cmMsg_(NULL, fmt, ##args)
#define cmInfo(fmt, args...) \
	cmMsg_(NULL, "%{Green:info} " fmt, ##args)
#define cmWarn(fmt, args...) \
	cmMsg_(NULL, "%{Yellow:warn} " fmt, ##args)
#define cmErr(fmt, args...) \
	cmMsg_(NULL, "%{Red:err} " fmt, ##args)
#define cmDbg(fmt, args...) \
	cmMsg_(NULL, "%{Cyan:%s:%d} " fmt, __func__, __LINE__, ##args)

#endif
