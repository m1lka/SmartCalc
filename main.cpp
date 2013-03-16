#include "parser.h"
#include "log.h"

int main(int argc, char **argv)
{
    Parser P;
    init_log();
    P.parse_text("cbr(8)");
    destroy_log();
    return 0;
}
