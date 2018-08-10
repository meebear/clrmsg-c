#define _GNU_SOURCE
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <alloca.h>
#include "clrmsg.h"

struct clr {
    const char *name;
    const char *val;
} fg_clrs[] = {
    {"Black",  "\033[1;30m"},
    {"Red",    "\033[1;31m"},
    {"Green",  "\033[1;32m"},
    {"Yellow", "\033[1;33m"},
    {"Blue",   "\033[1;34m"},
    {"Magenta","\033[1;35m"},
    {"Cyan",   "\033[1;36m"},
    {"White",  "\033[1;37m"},
    {"black",  "\033[2;30m"},
    {"red",    "\033[2;31m"},
    {"green",  "\033[2;32m"},
    {"yellow", "\033[2;33m"},
    {"blue",   "\033[2;34m"},
    {"magenta","\033[2;35m"},
    {"cyan",   "\033[2;36m"},
    {"white",  "\033[2;37m"},
    {NULL}
};

const char *reset = "\033[0m";

char* cmfmt_generate(const char *fmt, char *cmfmt, int len)
{
    return NULL;
}

int cmMsg_(int (*print)(const char *, ...), const char *fmt, ...)
{
    char *msg = NULL;
    va_list args;

    if (!print || !fmt)
        return 0;

    int len = strlen(fmt) * 2;
    char *cmfmt = alloca(len);

    va_start(args, fmt);
    int rv = vasprintf(&msg, cmfmt_generate(fmt, cmfmt, len), args);
    va_end(args);

    if (rv != -1)
        print("%s", msg);
    return rv;
}
