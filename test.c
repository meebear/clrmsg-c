#include <stdio.h>
#include "clrmsg.h"

int main()
{
    const char *cmf =
		"hello %{m:world} %{g:cyan number %d}\n";
    cmPrintf(cmf, 10.00);
    return 0;
}
