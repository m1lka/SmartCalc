#ifndef _PARSER_H_INCLUDE
#define _PARSER_H_INCLUDE

#include "math/math_func.h"

namespace Parser_types {

struct token_t {
    int type;
    char *data;
};

typedef float (*func1_t)(float);

struct float_func1_t {
    int global_id;
    func1_t func;
};

struct func_param_t {
    int global_id;
    int n_args;
};

struct identif_t {
    int type;
    const char *name;
    int global_id;
};

struct float_var_value {
    int global_id;
    float value_var;
};

struct parser_t {
    float_var_value float_buildin_var[];
    token_t tokens[256];
    unsigned num_tokens;
    unsigned index;

    parser_t()
    {
        this->num_tokens = 0;
        this->index = 0;
    }

    ~parser_t()
    {
        this->num_tokens = 0;
        this->index = 0;
    }
};

enum {
    UNKNOWN = 1, IDENTIFIER, INT_NUMBER, FLOAT_NUMBER,
    EXL, CARET, MULT, DIV, PLUS, MINUS,
    PARENTHESIS_LEFT, PARENTHESIS_RIGHT, VERT_BAR
};

enum {
    FUNCTION = 1, VARIABLE, EOE
};

}

namespace Builtin {

static Parser_types::float_func1_t func1[] =
{
    {10, (Parser_types::func1_t) &call_sinf},
    {11, (Parser_types::func1_t) &call_sinh},
    {12, (Parser_types::func1_t) &call_cosf},
    {13, (Parser_types::func1_t) &call_cosh},
    {14, (Parser_types::func1_t) &call_tanf},
    {15, (Parser_types::func1_t) &call_tanh},
    {16, (Parser_types::func1_t) &call_asin},
    {17, (Parser_types::func1_t) &call_asinh},
    {18, (Parser_types::func1_t) &call_acos},
    {19, (Parser_types::func1_t) &call_acosh},
    {20, (Parser_types::func1_t) &call_atan},
    {21, (Parser_types::func1_t) &call_atanh},
    {22, (Parser_types::func1_t) &call_sqr},
    {23, (Parser_types::func1_t) &call_cbr},
    {24, (Parser_types::func1_t) &call_log},
    {25, (Parser_types::func1_t) &call_log10},
    {26, (Parser_types::func1_t) &call_exp},
    {27, (Parser_types::func1_t) &call_exp2},
    {28, (Parser_types::func1_t) &call_abs}
};

static Parser_types::func_param_t param_funcs[] =
{
    {10, 1},
    {11, 1},
    {12, 1},
    {13, 1},
    {14, 1},
    {15, 1},
    {16, 1},
    {17, 1},
    {18, 1},
    {19, 1},
    {20, 1},
    {21, 1},
    {22, 1},
    {23, 1},
    {24, 1},
    {25, 1},
    {26, 1},
    {27, 1},
    {28, 1},
    {0, 0}
};

static Parser_types::identif_t identfiers[] =
{
    {Parser_types::FUNCTION, "sin", 10},
    {Parser_types::FUNCTION, "sinh", 11},
    {Parser_types::FUNCTION, "cos", 12},
    {Parser_types::FUNCTION, "cosh", 13},
    {Parser_types::FUNCTION, "tan", 14},
    {Parser_types::FUNCTION, "tanh", 15},
    {Parser_types::FUNCTION, "asin", 16},
    {Parser_types::FUNCTION, "asinh", 17},
    {Parser_types::FUNCTION, "acos", 18},
    {Parser_types::FUNCTION, "acoh", 19},
    {Parser_types::FUNCTION, "atan", 20},
    {Parser_types::FUNCTION, "atanh", 21},
    {Parser_types::FUNCTION, "sqr", 22},
    {Parser_types::FUNCTION, "cbr", 23},
    {Parser_types::FUNCTION, "log", 24},
    {Parser_types::FUNCTION, "log10", 25},
    {Parser_types::FUNCTION, "exp", 26},
    {Parser_types::FUNCTION, "exp2", 27},
    {Parser_types::FUNCTION, "abs", 28},
    {Parser_types::VARIABLE, "R", 50},
    {0, "", 0}
};

}

class Parser {
    Parser_types::parser_t P;



    void eval_expr(float &value);
    void eval_expr0(float &value);
    void eval_expr1(float &value);
    void eval_expr2(float &value);
    void eval_expr3(float &value);
    void eval_expr4(float &value);
    void eval_expr5(float &value);
    void eval_expr6(float &value);
    void atom(float &value);
    int find_identifier(const char *name, int type);
    float call_func(int func_id);
    char* get_var_name(int global_id);
    void assign_var(int global_id, float value);
    float get_var_value(int global_id);
    float syntax_parser();
    void lexer_parser(const char *text);

public:
    Parser() {}

    ~Parser() {}
    void parser_create();
    void parse_text(const char *text, float &value1);
};


#endif
