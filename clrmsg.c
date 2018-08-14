#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <alloca.h>
#include "clrmsg.h"

#ifndef __STANDALONE_TEST__
#include "debug.h"
static CmPrint _cm_print = message;
#else
static CmPrint _cm_print = printf;
#endif

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
    const char *val;
    const char *bg_val;
} fg_clrs[] = {
    [CM_Black]  = {BLA,   BG_BLA},
    [CM_Red]    = {RED,   BG_RED},
    [CM_Green]  = {GRN,   BG_GRN},
    [CM_Yellow] = {YEL,   BG_YEL},
    [CM_Blue]   = {BLU,   BG_BLU},
    [CM_Magenta]= {MAG,   BG_MAG},
    [CM_Cyan]   = {CYN,   BG_CYN},
    [CM_White]  = {WHT,   BG_WHT},
    [CM_black]  = {BLA_D, BG_BLA},
    [CM_red]    = {RED_D, BG_RED},
    [CM_green]  = {GRN_D, BG_GRN},
    [CM_yellow] = {YEL_D, BG_YEL},
    [CM_blue]   = {BLU_D, BG_BLU},
    [CM_magenta]= {MAG_D, BG_MAG},
    [CM_cyan]   = {CYN_D, BG_CYN},
    [CM_white]  = {WHT_D, BG_WHT},
    [CM_Reset]  = {RST, RST},
};

static inline const char* _clr(int clr, int is_fg) {
    return is_fg ? fg_clrs[clr].val : fg_clrs[clr].bg_val;
}

static const char* lookup_color(const char *p, int len)
{
    char c = p[0];
    p++;
    len--;
    switch (c) {
    case 'B':
    case 'b':
        if (len <= 3 && !strncmp(p, "lue", len))
            return c == 'B' ? _clr(CM_Blue, 1) : _clr(CM_blue, 1);
        if (len <= 4 && !strncmp(p, "lack", len))
            return c == 'B' ? _clr(CM_Black, 1) : _clr(CM_black, 1);
        break;
    case 'R':
    case 'r':
        if (len <= 2 && !strncmp(p, "ed", len))
            return c == 'R' ? _clr(CM_Red, 1) : _clr(CM_red, 1);
        break;
    case 'G':
    case 'g':
        if (len <= 4 && !strncmp(p, "reen", len))
            return c == 'G' ? _clr(CM_Green, 1) : _clr(CM_green, 1);
        break;
    case 'Y':
    case 'y':
        if (len <= 5 && !strncmp(p, "ellow", len))
            return c == 'Y' ? _clr(CM_Yellow, 1) : _clr(CM_yellow, 1);
        break;
    case 'M':
    case 'm':
        if (len <= 6 && !strncmp(p, "agenta", len))
            return c == 'M' ? _clr(CM_Magenta, 1) : _clr(CM_magenta, 1);
        break;
    case 'C':
    case 'c':
        if (len <= 3 && !strncmp(p, "yan", len))
            return c == 'C' ? _clr(CM_Cyan, 1) : _clr(CM_cyan, 1);
        break;
    case 'W':
    case 'w':
        if (len <= 4 && !strncmp(p, "hite", len))
            return c == 'W' ? _clr(CM_White, 1) : _clr(CM_white, 1);
        break;
    }
    return NULL;
}

static const char* parse(const char *str, int *start, int *len)
{
    const char *p = str + 1;
    if (*p != '{')
        return NULL;
    char *e = strchr(++p, '}');
    if (!e)
        return NULL;
    char *c = strchr(p, ':');
    const char *clr = NULL;
    if (c && c < e) {
        clr = lookup_color(p, c-p);
        if (clr) {
            c++;
            if ((*len = e - c) > 0)
                *start = c - str;
            else
                *start = c - str + 1;
        }
    } else {
        clr = lookup_color(p, e-p);
        if (clr) {
            *start = e - str + 1;
            *len = 0;
        }
    }
    return clr;
}

char* cmfmt_parse(const char *fmt, char *cmfmt, int with_color)
{
    const char *p;
    int start=0, len, off=0, tmp;
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
            const char *clr = parse(fmt, &start, &len);
            if (clr) {
                if (with_color) {
                    tmp = strlen(clr);
                    memcpy(cmfmt + off, clr, tmp);
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

int cmMsg_(CmPrint print, const char *fmt, ...)
{
    char *msg = NULL;
    va_list args;
    CmPrint prt = print ?: _cm_print;

    if (!print || !fmt)
        return 0;

    int len = strlen(fmt) * 2;
    char *cmfmt = alloca(len < 1024 ? 1024 : len);

    va_start(args, fmt);
    int rv = vasprintf(&msg, cmfmt_parse(fmt, cmfmt, 1), args);
    va_end(args);

    if (rv != -1) {
        prt("%s", msg);
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

void cmSetPrtFunc(CmPrint print)
{
    _cm_print = print;
}
