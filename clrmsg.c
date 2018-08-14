#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <alloca.h>
#include "clrmsg.h"

enum {
    CM_Black,
    CM_Red,
    CM_Green,
    CM_Yellow,
    CM_Blue,
    CM_Magenta,
    CM_Cyan,
    CM_White,
    CM_black,
    CM_red,
    CM_green,
    CM_yellow,
    CM_blue,
    CM_magenta,
    CM_cyan,
    CM_white,
    CM_Reset,
};

struct clr {
    const char *name;
    const char *val;
} fg_clrs[] = {
    [CM_Black]  = {"Black",  BLA},
    [CM_Red]    = {"Red",    RED},
    [CM_Green]  = {"Green",  GRN},
    [CM_Yellow] = {"Yellow", YEL},
    [CM_Blue]   = {"Blue",   BLU},
    [CM_Magenta]= {"Magenta",MAG},
    [CM_Cyan]   = {"Cyan",   CYN},
    [CM_White]  = {"White",  WHT},
    [CM_black]  = {"black",  BLA_D},
    [CM_red]    = {"red",    RED_D},
    [CM_green]  = {"green",  GRN_D},
    [CM_yellow] = {"yellow", YEL_D},
    [CM_blue]   = {"blue",   BLU_D},
    [CM_magenta]= {"magenta",MAG_D},
    [CM_cyan]   = {"cyan",   CYN_D},
    [CM_white]  = {"white",  WHT_D},
    [CM_Reset]  = {"",  RST},
};

static int lookup_color(const char *p, int len)
{
    char c = p[0];
    p++;
    len--;
    switch (c) {
    case 'B':
    case 'b':
        if (len <= 4 && !strncmp(p, "lack", len))
            return c == 'B' ? CM_Black : CM_black;
        if (len <= 3 && !strncmp(p, "lue", len))
            return c == 'B' ? CM_Blue : CM_blue;
        break;
    case 'R':
    case 'r':
        if (len <= 2 && !strncmp(p, "ed", len))
            return c == 'R' ? CM_Red : CM_red;
        break;
    case 'G':
    case 'g':
        if (len <= 4 && !strncmp(p, "reen", len))
            return c == 'G' ? CM_Green : CM_green;
        break;
    case 'Y':
    case 'y':
        if (len <= 5 && !strncmp(p, "ellow", len))
            return c == 'Y' ? CM_Yellow : CM_yellow;
        break;
    case 'M':
    case 'm':
        if (len <= 6 && !strncmp(p, "agenta", len))
            return c == 'M' ? CM_Magenta : CM_magenta;
        break;
    case 'C':
    case 'c':
        if (len <= 3 && !strncmp(p, "yan", len))
            return c == 'C' ? CM_Cyan : CM_cyan;
        break;
    case 'W':
    case 'w':
        if (len <= 4 && !strncmp(p, "hite", len))
            return c == 'W' ? CM_White : CM_white;
        break;
    }
    return -1;
}

static int parse(const char *str, int *start, int *len)
{
    const char *p = str + 1;
    if (*p != '{')
        return -1;
    char *e = strchr(++p, '}');
    if (!e)
        return -1;
    char *c = strchr(p, ':');
    int clr;
    if (c && c < e) {
        clr = lookup_color(p, c-p);
        if (clr >= 0) {
            c++;
            *start = c - str;
            *len = e - c;
        }
    } else {
        clr = lookup_color(p, e-p);
        if (clr >= 0) {
            *start = e - str + 1;
            *len = 0;
        }
    }
    return clr;
}

char* cmfmt_parse(const char *fmt, char *cmfmt, int with_color)
{
    const char *p;
    int start, len, off = 0, tmp;
    int need_reset = 0;
    for (;;) {
        //printf("-->%s<--\n", fmt);
        p = strchr(fmt, '%');
        if (p) {
            if ((len = p - fmt) > 0) {
                //printf("copy: %d: %s\n", len, fmt);
                memcpy(cmfmt + off, fmt, len);
                off += len;
            }
            fmt = p;
            int clr = parse(fmt, &start, &len);
            if (clr != -1) {
                //printf("color:%s start=%d len=%d\n", fg_clrs[clr].name, start, len);
                if (with_color) {
                    p = fg_clrs[clr].val;
                    tmp = strlen(p);
                    memcpy(cmfmt + off, p, tmp);
                    off += tmp;
                }
                fmt += start;
                if (len > 0) {
                    memcpy(cmfmt + off, fmt, len);
                    off += len;
                    fmt += len + 1;
                    if (with_color) {
                        p = fg_clrs[CM_Reset].val;
                        tmp = strlen(p);
                        memcpy(cmfmt + off, p, tmp);
                        off += tmp;
                    }
                    need_reset = 0;
                } else {
                    need_reset = 1;
                }
            } else {
                //printf("color:-1 copy %%\n");
                cmfmt[off++] = *fmt;
                fmt++;
            }
        } else {
            //printf("copy left: %s\n", fmt);
            if ((tmp = strlen(fmt)) > 0) {
                memcpy(cmfmt + off, fmt, tmp);
                off += tmp;
            }
            if (with_color && need_reset) {
                p = fg_clrs[CM_Reset].val;
                tmp = strlen(p);
                memcpy(cmfmt + off, p, tmp);
                off += tmp;
            }
            cmfmt[off] = '\0';
            break;
        }
    }
    return cmfmt;
}

int cmMsg_(int (*print)(const char *, ...), const char *fmt, ...)
{
    char *msg = NULL;
    va_list args;

    if (!print || !fmt)
        return 0;

    int len = strlen(fmt) * 2;
    char *cmfmt = alloca(len < 1024 ? 1024 : len);

    va_start(args, fmt);
    int rv = vasprintf(&msg, cmfmt_parse(fmt, cmfmt, 1), args);
    va_end(args);

    if (rv != -1) {
        print("%s", msg);
        free(msg);
    }

    return rv;
}

const char* cmFmtClear(const char *fmt, char *newFmt, size_t sz)
{
    int len = strlen(fmt) * 2;
    char *cmfmt = alloca(len < 1024 ? 1024 : len);
    cmfmt_parse(fmt, cmfmt, 0);
    len = strlen(cmfmt);
    if (len >= sz)
        len = sz - 1;
    memcpy(newFmt, cmfmt, len);
    newFmt[len] = '\0';
    return newFmt;
}
