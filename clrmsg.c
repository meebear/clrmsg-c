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

static inline const char* _clr(int clr, int is_bg) {
    return is_bg ? fg_clrs[clr].bg_val : fg_clrs[clr].val;
}

static const char* lookup_color(const char *p, int len, const char **bgclr)
{
    char c, *n=NULL, *at = memchr(p, '@', len);
    int nlen=0;
    const char *fgclr = NULL;
    const char **clr;

    //printf("at=%s\n", at);
    for (int i=0; i<2; i++) {
        if (i == 0) {
            if (at) {
                nlen = p + len - at - 1;
                n = at + 1;
                len = at - p;
            }
            clr = &fgclr;
        } else {
            if (n && nlen > 0) {
                p = n;
                len = nlen;
            } else {
                break;
            }
            clr = bgclr;
        }

        c = p[0];
        p++;
        len--;
        switch (c) {
        case 'B':
        case 'b':
            if (len <= 3 && !strncmp(p, "lue", len))
                *clr = c == 'B' ? _clr(CM_Blue, i) : _clr(CM_blue, i);
            if (len <= 4 && !strncmp(p, "lack", len))
                *clr = c == 'B' ? _clr(CM_Black, i) : _clr(CM_black, i);
            break;
        case 'R':
        case 'r':
            if (len <= 2 && !strncmp(p, "ed", len))
                *clr = c == 'R' ? _clr(CM_Red, i) : _clr(CM_red, i);
            break;
        case 'G':
        case 'g':
            if (len <= 4 && !strncmp(p, "reen", len))
                *clr = c == 'G' ? _clr(CM_Green, i) : _clr(CM_green, i);
            break;
        case 'Y':
        case 'y':
            if (len <= 5 && !strncmp(p, "ellow", len))
                *clr = c == 'Y' ? _clr(CM_Yellow, i) : _clr(CM_yellow, i);
            break;
        case 'M':
        case 'm':
            if (len <= 6 && !strncmp(p, "agenta", len))
                *clr = c == 'M' ? _clr(CM_Magenta, i) : _clr(CM_magenta, i);
            break;
        case 'C':
        case 'c':
            if (len <= 3 && !strncmp(p, "yan", len))
                *clr = c == 'C' ? _clr(CM_Cyan, i) : _clr(CM_cyan, i);
            break;
        case 'W':
        case 'w':
            if (len <= 4 && !strncmp(p, "hite", len))
                *clr = c == 'W' ? _clr(CM_White, i) : _clr(CM_white, i);
            break;
        }
    }
    return fgclr;
}

static const char* parse(const char *str, int *start, int *len, const char **bgclr)
{
    const char *fgclr = NULL;
    const char *p = str + 1;
    if (*p != '{')
        return NULL;
    char *e = strchr(++p, '}');
    if (!e)
        return NULL;
    char *c = memchr(p, ':', e-p);
    if (c) {
        fgclr = lookup_color(p, c-p, bgclr);
        if (fgclr || *bgclr) {
            c++;
            if ((*len = e - c) > 0)
                *start = c - str;
            else
                *start = c - str + 1;
        }
    } else {
        fgclr = lookup_color(p, e-p, bgclr);
        if (fgclr || *bgclr) {
            *start = e - str + 1;
            *len = 0;
        }
    }
    return fgclr;
}

char* cmfmt_parse(const char *fmt, char *cmfmt, int with_color)
{
    const char *p;
    int start=0, len, off=0, tmp;
    int need_reset = 0;
    for (;;) {
        printf("-->%s<--\n", fmt);
        p = strchr(fmt, '%');
        if (p) {
            if ((len = p - fmt) > 0) {
                //printf("copy: %d: %s\n", len, fmt);
                memcpy(cmfmt + off, fmt, len);
                off += len;
            }
            fmt = p;
            const char *bgclr = NULL;
            const char *fgclr = parse(fmt, &start, &len, &bgclr);
            printf("fgclr=%p, bgclr=%p\n", fgclr, bgclr);
            if (fgclr || bgclr) {
                if (with_color) {
                    if (fgclr) {
                        tmp = strlen(fgclr);
                        memcpy(cmfmt + off, fgclr, tmp);
                        off += tmp;
                    }
                    if (bgclr) {
                        tmp = strlen(bgclr);
                        memcpy(cmfmt + off, bgclr, tmp);
                        off += tmp;
                    }
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
