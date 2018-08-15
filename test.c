#include <stdio.h>
#include "clrmsg.h"

int main()
{
    const char *cmf =
		"%{r:hello} %{b:world} -- %{C:c}%{B:o}%{M:l}%{R:o}%{Y:r}%{C:f}%{G:u}%{W:l}\n";
    cmPrintf(cmf);
    return 0;
}
