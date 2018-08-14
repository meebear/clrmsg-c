#include <stdio.h>
#include "clrmsg.h"

int main()
{
    const char *cmf = "hello %{Red}world %{Cyan:cyan number %d} back to normal\n";
    cmMsg_(printf, cmf, 1000);

    char fmt[1024];
    cmFmtClear(cmf, fmt, 1024);
    printf(fmt, 1000);

	cmDbg("debug message\n");
    return 0;
}
