#include <stdio.h>
#include "clrmsg.h"

int main()
{
    const char *cmf =
		"%{B@G:complex} hello %{Gre}world %{Cyan:cyan number %d}\n";
    cmPrintf(cmf, 1000);
    return 0;
}
