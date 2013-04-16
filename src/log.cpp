#include "log.h"
#include <cstdio>
#include <cstdarg>

FILE *log = NULL;

bool init_log()
{
    log = fopen("calc.log", "w");
    if(!log)
        return false;
    fprintf(log, "\tLogger file SmartCalc v0.6 beta.\n");
}

void log_print(const char *text, ...)
{
    va_list p;
    va_start(p, text);

    vfprintf(log, text, p);
    fflush(log);
    vprintf(text, p);
    va_end(p);
}

void destroy_log()
{
    fprintf(log, "\tEnd logger file.\n");
    fclose(log);
}
