#include "parser.h"
#include <iostream>
#include "log.h"

int main(int argc, char **argv)
{
    Parser P;
    init_log();
    P.parse_text("4+4*5");
    destroy_log();
    return 0;
}
