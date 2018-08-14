#include <stdio.h>
#include "clrmsg.h"

int main()
{
    const char *cmf =
		"%{@b} hello %{M:world} %{Cyan:cyan number %d}\n";
    cmPrintf(cmf, 10.00);
    return 0;
}
