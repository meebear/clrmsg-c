#include <stdio.h>
#include "clrmsg.h"

int main()
{
    const char *cmf =
		"%{Red} hello %{Gre}world %{Cyan:cyan number %d}\n";
    cmPrintf(cmf, 1000);
    return 0;
}
