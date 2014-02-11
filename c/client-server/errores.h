#include        <errno.h>
#include        <stdarg.h>
#include        "ourhdr.h"


void    err_ret(const char *, ...);
void    err_sys(const char *, ...);
void    err_dump(const char *, ...);
void    err_msg(const char *, ...);
void    err_quit(const char *, ...);
