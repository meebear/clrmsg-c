#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "clrmsg.h"

#ifndef __STANDALONE_TEST__
#include "debug.h"
int (*cm_print)(const char *fmt, ...) = message;
#else
int (*cm_print)(const char *fmt, ...) = printf;
#endif

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

struct parse_info { const char *s, *e, *token, *clr; };

static void parse_next(struct parse_info *pi)
{
    const char *p;
    struct clr *c;
    int l;

    pi->clr = pi->token = 0;

    for (p=pi->s; p<pi->e; ++p) {
        if (*p == '%') {
            if (p == pi->s)
                continue;
            pi->token = p;
            pi->s = p;
            pi->clr = 0;
            return;
        } else if (*p == '{') {
            for (c=fg_clrs; c->name; ++c) {
                l = strlen(c->name);
                if (pi->e-p >= l+2 && *(p+l+1) == '}') {
                    if (!strncmp(p+1, c->name, l)) {
                        pi->token = p;
                        pi->s = p + l +2;
                        pi->clr = c->val;
                        return;
                    }
                }
            }
        }
    }

    pi->s = pi->e;
}

const char* _cmFmt(const char *fmt, char *newFmt, size_t sz, int clear)
{
    char *p=newFmt, *pe=p+sz-1;
    struct parse_info pi;
    const char *old_s;
    int l, all = 0;
#define COPY_P(s, len) do {\
    l = (len); \
    if (p + l >= pe) {\
        *p = '\0';\
        break;\
    }\
    memcpy(p, (s), l); \
    p += l; \
    *p = '\0'; \
} while (0)

#define COPY_P_COND(s, len) do {\
    if (!clear) \
        COPY_P(s, (len)); \
} while (0)

    if (!newFmt)
        return fmt;

    pi.s = fmt;
    pi.e = fmt + strlen(fmt);
    while (pi.s < pi.e) {
        old_s = pi.s;
        parse_next(&pi);

        if (pi.clr) {
            if (pi.token == fmt) {
                all = 1;
                COPY_P_COND(pi.clr, strlen(pi.clr));
            } else {
                if (!all)
                    COPY_P_COND(pi.clr, strlen(pi.clr));
                COPY_P(old_s, pi.token-old_s);
                if (!all)
                    COPY_P_COND(reset, strlen(reset));
            }
        } else {
            if (pi.token) {
                COPY_P(old_s, pi.token-old_s);
            } else {
                COPY_P(old_s, strlen(old_s));
                if (all)
                    COPY_P_COND(reset, strlen(reset));
            }
        }
    }
    return newFmt;
}

const char* cmFmt(const char *fmt, char *newFmt, size_t sz)
{
    return _cmFmt(fmt, newFmt, sz, 0);
}

const char* cmFmtClear(const char *fmt, char *newFmt, size_t sz)
{
    return _cmFmt(fmt, newFmt, sz, 1);
}

int cmPrintf(const char *fmt, ...)
{
    char msg[8192];
    char cmfmt[1024];
    va_list args;
    int rv;

    va_start(args, fmt);
    rv = vsnprintf(msg, sizeof(msg),
            cmFmt(fmt, cmfmt, sizeof(cmfmt)), args);
    va_end(args);

    printf("%s", msg);
    return rv;
}

int cmMsg_(int (*print)(const char *fmt, ...), const char *fmt, ...)
{
    char msg[8192];
    char cmfmt[1024];
    va_list args;
    int rv;

    if (!print)
        return 0;

    va_start(args, fmt);
    rv = vsnprintf(msg, sizeof(msg),
            cmFmt(fmt, cmfmt, sizeof(cmfmt)), args);
    va_end(args);

    print("%s", msg);
    return rv;
}
