#include "parser.h"
#include "log.h"
#include <iostream>
#include <cstdio>

int main(int argc, char **argv)
{
    Parser P;
    char text[80];
    init_log();
    P.parser_create();
    for(int i = 0; i < 4; i++) {
        MSG("Input expr: ");
        gets(text);
        P.parse_text((const char *)text);
    }
    destroy_log();
    return 0;
}
