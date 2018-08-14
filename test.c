#include <stdio.h>
#include "clrmsg.h"

int main()
{
    cmMsg_(printf, "hello %{Red}world %{Cyan:cyan number %d} back to normal\n", 1000);
    return 0;
}
