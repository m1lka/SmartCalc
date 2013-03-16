#ifndef _LOG_H
#define _LOG_H

#include <cstdio>

bool init_log();

void log_print(const char *text, ...);

void destroy_log();

#define MSG(text, ...) printf(text, ##__VA_ARGS__)

#endif
