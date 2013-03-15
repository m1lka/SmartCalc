#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cctype>
#include <cstring>

#define MSG(text, ...) printf(text, ##__VA_ARGS__)

struct token_t {
    int type;
    char *data;
};

struct parser {
    token_t tokens[256];
    unsigned num_tokens;
    unsigned index;
    int error;
    int init;

    parser()
    {
        this->num_tokens = 0;
        this->index = 0;
    }
    ~parser()
    {
        this->num_tokens = 0;
    }
};

float call_sinf(float x)
{
    return sinf(x);
}

float call_sinh(float x)
{
    return sinh(x);
}

float call_cosf(float x)
{
    return cosf(x);
}

float call_cosh(float x)
{
    return sinh(x);
}

float call_tanf(float x)
{
    return tanf(x);
}

float call_tanh(float x)
{
    return tanh(x);
}

float call_sqr(float x)
{
    return sqrt(x);
}

float call_cbr(float x)
{
    return cbrt(x);
}

float call_acos(float x)
{
    return acos(x);
}

float call_acosh(float x)
{
    return acosh(x);
}

float call_asin(float x)
{
    return asin(x);
}

float call_asinh(float x)
{
    return asinh(x);
}

float call_atan(float x)
{
    return atan(x);
}

float call_atanh(float x)
{
    return atanh(x);
}

float call_log(float x)
{
    return log(x);
}

float call_log10(float x)
{
    return log10(x);
}

float call_exp(float x)
{
    return exp(x);
}

float call_exp2(float x)
{
    return exp2(x);
}

void eval_expr(parser &p, float &value);
void eval_expr_1(parser &p, float &value);
void eval_expr1(parser &p, float &value);
void eval_expr2(parser &p, float &value);
void eval_expr3(parser &p, float &value);
void eval_expr4(parser &p, float &value);
void eval_expr5(parser &p, float &value);
void atom(parser &p, float &value);

#define textcpy(text_p, to_p, n) for(int i = 0; i < n; i++) {to_p[i] = text_p[i];} to_p[n] = '\0';

enum {
    UNKNOWN = 1, IDENTIFIER, INT_NUMBER, FLOAT_NUMBER,
    EXL, CARET, MULT, DIV, PLUS, MINUS, FUNC,
    PARENTHESIS_RIGHT, PARENTHESIS_LEFT
};

enum {
    FUNCTION = 1, VARIABLE
};

typedef float (*func1_t)(float);

struct float_function1_t {
    int global_id;
    func1_t func;
};

struct functions_param_t {
    int global_id;
    int n_args;
};

struct identifier_t {
    int type;
    char *name;
    int global_id;
};

float_function1_t builtin_functions1[] =
{
    {101, (func1_t) &call_sinf},
    {102, (func1_t) &call_sinh},
    {103, (func1_t) &call_cosf},
    {104, (func1_t) &call_cosh},
    {105, (func1_t) &call_tanf},
    {106, (func1_t) &call_tanh},
    {107, (func1_t) &call_asin},
    {108, (func1_t) &call_asinh},
    {109, (func1_t) &call_acos},
    {110, (func1_t) &call_acosh},
    {111, (func1_t) &call_atan},
    {112, (func1_t) &call_atanh},
    {113, (func1_t) &call_sqr},
    {114, (func1_t) &call_cbr},
    {115, (func1_t) &call_log},
    {116, (func1_t) &call_log10},
    {117, (func1_t) &call_exp},
    {118, (func1_t) &call_exp2}
};

functions_param_t builtin_functions_param[] =
{
    {101, 1},
    {102, 1},
    {103, 1},
    {104, 1},
    {105, 1},
    {106, 1},
    {107, 1},
    {108, 1},
    {109, 1},
    {110, 1},
    {111, 1},
    {112, 1},
    {113, 1},
    {114, 1},
    {115, 1},
    {116, 1},
    {117, 1},
    {118, 1},
    {0, 0}
};

identifier_t builtin_identifiers[] =
{
    {FUNCTION, (char *)"sin", 101},
    {FUNCTION, (char *)"sinh", 102},
    {FUNCTION, (char *)"cos", 103},
    {FUNCTION, (char *)"cosh", 104},
    {FUNCTION, (char *)"tan", 105},
    {FUNCTION, (char *)"tanh", 106},
    {FUNCTION, (char *)"asin", 107},
    {FUNCTION, (char *)"asinh", 108},
    {FUNCTION, (char *)"acos", 109},
    {FUNCTION, (char *)"acoh", 110},
    {FUNCTION, (char *)"atan", 111},
    {FUNCTION, (char *)"atanh", 112},
    {FUNCTION, (char *)"sqr", 113},
    {FUNCTION, (char *)"cbr", 114},
    {FUNCTION, (char *)"log", 115},
    {FUNCTION, (char *)"log10", 116},
    {FUNCTION, (char *)"exp", 117},
    {FUNCTION, (char *)"exp2", 118},
    {0, (char *)"", 0}
};

inline int find_identifier(parser &p, const char *name, int type)
{
    int i = 0;
    while(builtin_identifiers[i].type != 0) {
        if(!strcmp(builtin_identifiers[i].name, name)) {
            if(builtin_identifiers[i].type == type)
                return builtin_identifiers[i].global_id;
        }
        i++;
    }

    return -1;
}

#define find_func(p, name) (find_identifier(p, name, FUNCTION))

float call_func(parser &p, int func_id)
{
    float args;
    int n_args = 0;

    func1_t func1_ptr = NULL;

    if(p.tokens[p.index].type == IDENTIFIER) {
        p.index++;
    } else {
        MSG("expected func name\n");
        return 0.0f;
    }

    if(p.tokens[p.index].type == PARENTHESIS_LEFT) {
        p.index++;

        int i = 0; bool flag_found = 0;
        while(builtin_functions_param[i].global_id != 0) {
            if(builtin_functions_param[i].global_id == func_id) {
                n_args = builtin_functions_param[i].n_args;
                break;
            }
            i++;
        }

        if(n_args == 0) {
            MSG("wrong func\n");
            return 0.0f;
        }

#define func_ptrs(n) \
    case n: {\
        while(builtin_functions##n[i].global_id != 0) {\
            if(builtin_functions##n[i].global_id == func_id) {\
                func##n##_ptr = builtin_functions##n[i].func;\
                flag_found = 1;\
                break;\
            }\
            i++;\
        }\
        break; }

        i = 0;
        switch(n_args) {
            func_ptrs(1);
        };
#undef func_ptrs

        if(!flag_found) {
            MSG("wrong number of arguments\n");
            return 0.0f;
        }

        MSG("call func id = %i n_args = %i\n", func_id, n_args);

        if(!func1_ptr) {
            MSG("null pointer to function\n");
            return 0.0f;
        }

        bool flag_found_pr = 0;
        int r = 0;
        eval_expr_1(p, args);
        MSG("args = %f\n", args);

        if(p.tokens[p.index].type == PARENTHESIS_RIGHT) {
            flag_found_pr = 1;
            p.index++;
        } else {
            MSG("expected )\n");
            return 0.0f;
        }

        if(r+1 != n_args) {
            MSG("wrong number of arguments\n");
            return 0.0f;
        }

    } else {
        MSG("expected (\n");
        return 0.0f;
    }

    float result = 0.0f;

    switch(n_args) {
        case 1:
            result = (func1_ptr)(args);
            break;

        default:
            MSG("wrong number of arguments\n");
    }
    return result;
}

void atom(parser &p, float &value)
{
    int id = 0, last = 0;
    MSG("atom = %s\n", p.tokens[p.index].data);

    switch(p.tokens[p.index].type) {
        case IDENTIFIER:
        if((id = find_func(p, p.tokens[p.index].data)) != -1) {
            last = p.index;
            value = call_func(p, id);

        } else {
            if(p.tokens[p.index+1].type == PARENTHESIS_LEFT)
                MSG("unknown func\n");
        }
        break;
        case FLOAT_NUMBER:
            value = (float)atof((const char*)p.tokens[p.index].data);
            MSG("found float num\n");
            p.index++;
            break;

        case INT_NUMBER:
            value = (float)atoi((const char*)p.tokens[p.index].data);
            MSG("found int number\n");
            MSG("num = %s\n", p.tokens[p.index].data);

            p.index++;
            break;

        case PARENTHESIS_RIGHT:
            return;
        default:
            MSG("unknow\n");
    };
}

/// ( )
void eval_expr5(parser &p, float &value)
{
    MSG("EVAL5\n");
    if(p.tokens[p.index].type == PARENTHESIS_LEFT) {
        p.index++;

        eval_expr(p, value);

        if(p.tokens[p.index].type != PARENTHESIS_RIGHT) {
            MSG("expected )\n");
            return;
        }
        p.index++;
    } else {
        atom(p, value);
    }
}

/// !
void eval_expr4(parser &p, float &value)
{
    MSG("EVAL4\n");
    eval_expr5(p, value);

    if(p.tokens[p.index].type == EXL) {
        MSG("found !\n");
        if(value == 0.0f) {
            value = 1.0f;
            p.index++;
            return;
        }

        if(value < 0.0f) {
            MSG("skip factorial\n");
            p.index++;
            return;
        }

        float s = 1.0f;
        for(unsigned i = 1; i <= (unsigned) value; i+=1) {
            s *= i;
        }
        value = s;
        p.index++;
    }
}

/// unary -
void eval_expr3(parser &p, float &value)
{
    MSG("EVAL3\n");
    int last = -1;
    if(p.tokens[p.index].type == PLUS || p.tokens[p.index].type == MINUS) {
        last = p.index++;
        MSG("unary + - %i\n", last);
    }

    eval_expr4(p, value);
    MSG("value unary -   = %f\n", value);
    if(last > -1) {
        MSG("unary   token: %s\n", p.tokens[last].data);
        if(p.tokens[last].type == MINUS) {
            MSG("found unary -\n");
            value = -value;
            MSG("value ----- : %f", value);
        }
    }
}

/// ^
void eval_expr2(parser &p, float &value)
{
    MSG("EVAL2\n");
    eval_expr3(p, value);
    if(p.tokens[p.index].type == CARET) {
        float part = 0.0f;
        MSG("found power\n");
        p.index++;
        eval_expr3(p, part);

        if(part == 0.0f) {
            value = 1.0f;
            return;
        }

        if(value == 0.0f) {
            value = 0.0f;
            return;
        }

        if(part == 1.0) {
            return;
        }

        if(part == 2.0f) {
            value = value * value;
            return;
        }

        if(part == 3.0f) {
            value = value * value * value;
            return;
        }

        value = powf(value, part);
    }
}

/// * /
void eval_expr1(parser &p, float &value)
{
    MSG("EVAL1\n");
    eval_expr2(p, value);
    while(p.tokens[p.index].type == MULT || p.tokens[p.index].type == DIV) {
        float part = 0.0f;
        int last = 0;
        MSG("found * /\n");
        last = p.index++;

        eval_expr2(p, part);

        switch(p.tokens[last].type) {
            case MULT:
                value *= part;
                break;

            case DIV:
                if(part == 0.0f) {
                    MSG("div to NULL\n");
                    value = 0.0f;
                    return;
                }
                value /= part;
                break;
        };
        MSG("value = %f\n", value);
    }
}

/// + -
void eval_expr0(parser &p, float &value)
{
    MSG("EVAL0\n");
    eval_expr1(p, value);
    while(p.tokens[p.index].type == PLUS || p.tokens[p.index].type == MINUS) {
        float part = 0.0f;
        int last = 0;
        MSG("found + -\n");
        last = p.index++;
        eval_expr1(p, part);
        switch(p.tokens[last].type) {
            case PLUS:
                value += part;
                break;

            case MINUS:
                value -= part;
                break;
        };
    }
}

/// =
void eval_expr_1(parser &p, float &value)
{
   MSG("EVAL_1\n");
    if(p.tokens[p.index].type == IDENTIFIER) {
        MSG("found ident = %s\n", p.tokens[p.index].data);

        if(p.tokens[p.index+1].type == PARENTHESIS_LEFT) {
            eval_expr0(p, value);
            return;
        }
        p.index++;
    }
    eval_expr0(p, value);
}

void eval_expr(parser &p, float &value)
{
    MSG("EVAL\n");
    if(p.index > (p.num_tokens - 1)) {
        MSG("no expression\n");
        return;
    }

    if(p.tokens[p.index].data == "]") {
        value = 0;
        return;
    }
    eval_expr_1(p, value);
}

void syntax_parser(parser &p)
{
    do {
        float value = 0.0f;
        if(p.tokens[p.index].type == IDENTIFIER) {
            MSG("token is identificator\n");
            eval_expr(p, value);
        }
        else {
            eval_expr0(p, value);
            MSG("token is not identificator\n");
        }
        p.index++;
        MSG("value = %f\n", value);
    } while(p.index < (p.num_tokens - 1));
    MSG("end syntax\n");
}

void lexer_parser(parser &p, const char *s)
{
    MSG("[DEBUG] lexer_parser()\n");
    char *ptr = (char *)s;

    while(*ptr != '\0') {
        while(*ptr == ' ')
            ptr++;
        if(*(ptr+1) == '\0') {

        }
        if(isalpha(*ptr)) {
            char *begin = ptr;
            MSG("isalpha!\n");
            
            while((isalpha(*ptr) || isdigit(*ptr)) && ((ptr-begin) < 63)) {
            	MSG("%c\n", *ptr); ptr++;
            }
            
            p.tokens[p.num_tokens].type = IDENTIFIER;
            p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * (ptr-begin));
            textcpy(begin, p.tokens[p.num_tokens].data, (ptr-begin));
            ptr--;
            p.num_tokens++;
        } else if(isdigit(*ptr)) {
            MSG("isdigit! : ");
            char *begin = ptr;
            while(isdigit(*ptr))
                ptr++;

            if(*ptr == '.') {
                if(isdigit(*(ptr+1)))
                    ptr++;
                while(isdigit(*ptr))
                    ptr++;
                MSG("float num\n");
                p.tokens[p.num_tokens].type = FLOAT_NUMBER;
            } else {
                MSG("int num\n");
                p.tokens[p.num_tokens].type = INT_NUMBER;
            }
            p.tokens[p.num_tokens].data = (char*) malloc(sizeof(char) * (ptr - begin));
            textcpy(begin, p.tokens[p.num_tokens].data, (ptr - begin));
            ptr--;
            p.num_tokens++;
        } else if(isprint(*ptr)) {
            MSG("isprint! : ");
            switch(*ptr) {

            case '(':
                MSG("(\n");
                p.tokens[p.num_tokens].type = PARENTHESIS_LEFT;
                p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            case ')':
                MSG(")\n");
                p.tokens[p.num_tokens].type = PARENTHESIS_RIGHT;
                p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            case '!':
                MSG("!\n");
                p.tokens[p.num_tokens].type = EXL;
                p.tokens[p.num_tokens].data = (char *)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            case '^':
                MSG("^\n");
                p.tokens[p.num_tokens].type = CARET;
                p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            case '*':
                MSG("*\n");
                p.tokens[p.num_tokens].type = MULT;
                p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            case '/':
                MSG("/\n");
                p.tokens[p.num_tokens].type = DIV;
                p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            case '+':
                MSG("+\n");
                p.tokens[p.num_tokens].type = PLUS;
                p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            case '-':
                MSG("-\n");
                p.tokens[p.num_tokens].type = MINUS;
                p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, p.tokens[p.num_tokens].data, 1);
                break;

            default:
                MSG("undefined token\n");
                return;
            };
            p.num_tokens++;
        }
        ptr++;
        if(*ptr == '\0') {
            MSG("null_symbol\n");
            p.tokens[p.num_tokens].data = (char*)malloc(sizeof(char) * 1);
            textcpy("]", p.tokens[p.num_tokens].data, 1);
            p.num_tokens++;
        }
    }
    MSG("close : lexer_parser()\n");
}
#undef textcpy

int main()
{
    parser p;
    char text[80];
    lexer_parser(p, "56+5+6");
    syntax_parser(p);
    for(int i = 0; i < p.num_tokens; i++) {
        MSG("( %s )\n", p.tokens[i].data);
    }
    MSG("\n");
    return 0;
}

