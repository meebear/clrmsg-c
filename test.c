#include <stdio.h>
#include "clrmsg.h"

int main()
{
    cmMsg_(printf, "hello %{Red:red} %{Cyan:-+ ll  -+-} world\n");
    return 0;
}
