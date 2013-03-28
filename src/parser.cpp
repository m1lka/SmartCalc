#include "parser.h"
#include "log.h"
#include "math/math_func.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

float value = 0.0f;
Parser_types::float_var_value float_buildin_var[2] =
{
    {50, 0.0}, {0, 0.0}
};

#define textcpy(text_p, to_p, n) for(int i = 0; i < n; i++) {to_p[i] = text_p[i];} to_p[n] = '\0';

void Parser::error_set(unsigned error_value)
{
    if(Parser::P.error == 0)
        Parser::P.error = error_value;
}

int Parser::find_identifier(const char *name, int type)
{
    int i = 0;
    while(Builtin::identfiers[i].type != 0) {
        if(!strcmp(Builtin::identfiers[i].name, name)) {
            if(Builtin::identfiers[i].type == type) {
                return Builtin::identfiers[i].global_id;
            }
        }
        i++;
    }
    return -1;
}

#define find_var(name) (Parser::find_identifier(name, Parser_types::VARIABLE))
#define find_func(name) (Parser::find_identifier(name, Parser_types::FUNCTION))

float Parser::call_func(int func_id)
{
    float args;
    int n_args = 0;

    Parser_types::func1_t func1_ptr = NULL;

    if(Parser::P.tokens[Parser::P.index].type == Parser_types::IDENTIFIER) {
        Parser::P.index++;
    } else {
        MSG("expected func name\n");
        return 0.0f;
    }

    if(Parser::P.tokens[Parser::P.index].type == Parser_types::PARENTHESIS_LEFT) {
        Parser::P.index++;

        int i = 0; bool flag_found = 0;
        while(Builtin::param_funcs[i].global_id != 0) {
            if(Builtin::param_funcs[i].global_id == func_id) {
                n_args = Builtin::param_funcs[i].n_args;
                break;
            }
            i++;
        }
        if(n_args == 0) {
            MSG("wrong func\n");
            Parser::error_set(4);
            return 0.0f;
        }

#define func_ptrs(n) \
    case n: {\
        while(Builtin::func##n[i].global_id != 0) {\
            if(Builtin::func##n[i].global_id == func_id) {\
                func##n##_ptr = Builtin::func##n[i].func;\
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
        Parser::eval_expr0(args);
        MSG("args = %f\n", args);

        if(Parser::P.tokens[Parser::P.index].type == Parser_types::PARENTHESIS_RIGHT) {
            flag_found_pr = true;
            Parser::P.index++;
        } else {
            log_print("call_func(): expected ')'\n");
            Parser::error_set(2);
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

float Parser::get_var_value(int global_id)
{
    int i = 0;
    while(float_buildin_var[i].global_id != 0) {
        MSG("id = %i <> %i\n", global_id, float_buildin_var[i].global_id);
        if(float_buildin_var[i].global_id == global_id) {
            return float_buildin_var[i].value_var;
        }
        i++;
    }
    return -1.0;
}

char* Parser::get_var_name(int global_id)
{
    int i = 0;
    while(Builtin::identfiers[i].global_id != 0) {
        if(Builtin::identfiers[i].global_id == global_id && Builtin::identfiers[i].type == Parser_types::VARIABLE) {
            return (char*)Builtin::identfiers[i].name;
        }
        i++;
    }
    return NULL;
}

void Parser::assign_var(int global_id, float value)
{
    int i = 0;
    MSG("assign: %s(%i) = %f\n", Parser::get_var_name(global_id), global_id, value);
    while(float_buildin_var[i].global_id != 0) {
        if(float_buildin_var[i].global_id != 0) {
            float_buildin_var[i].value_var = value;
            return;
        }
        i++;
    }
}

// ololo
void Parser::atom(float &value)
{
    int id = 0, last = 0;
    log_print("atom(): enter\n");
    log_print("\ttoken: %s\n", Parser::P.tokens[Parser::P.index].data);

    switch(Parser::P.tokens[Parser::P.index].type) {
    case Parser_types::IDENTIFIER:
        if((id = find_func(Parser::P.tokens[Parser::P.index].data)) != -1) {
            last = Parser::P.index;
            value = call_func(id);
        } else if((id = find_var(Parser::P.tokens[Parser::P.index].data)) != -1) {
            last = Parser::P.index;
            value = Parser::get_var_value(id);
            log_print("\tget variable: %s = %f", Parser::P.tokens[Parser::P.index].data, value);
            Parser::P.index++;
        } else {
            if(Parser::P.tokens[Parser::P.index+1].type == Parser_types::PARENTHESIS_LEFT) {
                log_print("\tfunction undefined: %s\n", Parser::P.tokens[Parser::P.index].data);
                Parser::error_set(3);
            }
        }
        break;
    case Parser_types::FLOAT_NUMBER:
            value = (float)atof((const char*)Parser::P.tokens[Parser::P.index].data);
            log_print("\tfound float number: %s\n", Parser::P.tokens[Parser::P.index].data);
            Parser::P.index++;
            break;

        case Parser_types::INT_NUMBER:
            value = (float)atoi((const char*)Parser::P.tokens[Parser::P.index].data);
            log_print("\tfound int number: %s\n", Parser::P.tokens[Parser::P.index].data);
            Parser::P.index++;
            break;

        case Parser_types::PARENTHESIS_RIGHT:
            return;
        default:
            log_print("\tunknow token\n");
    };
    log_print("atom(): exit function\n");
}

// ( )
void Parser::eval_expr6(float &value)
{
    log_print("eval_expr6(): enter\n");
    if(Parser::P.tokens[Parser::P.index].type == Parser_types::PARENTHESIS_LEFT) {
        Parser::P.index++;

        Parser::eval_expr(value);

        if(Parser::P.tokens[Parser::P.index].type != Parser_types::PARENTHESIS_RIGHT) {
            log_print("[Error] expected ')'\n");
            Parser::error_set(2);
            return;
        }
        Parser::P.index++;
    } else {
        Parser::atom(value);
    }
    log_print("eval_expr6(): exit_function\n");
}

// !
void Parser::eval_expr5(float &value)
{
    log_print("eval_expr5(): enter\n");
    Parser::eval_expr6(value);

    if(Parser::P.tokens[Parser::P.index].type == Parser_types::EXL) {
        log_print("\tfound %s\n", Parser::P.tokens[Parser::P.index].data);
        if(value == 0.0f) {
            value = 1.0f;
            Parser::P.index++;
            return;
        }

        if(value < 0.0f) {
            log_print("\tskip factorial (-)\n");
            Parser::P.index++;
            return;
        }

        float s = 1.0f;
        for(unsigned i = 1; i <= (unsigned) value; i+=1) {
            s *= i;
        }
        value = s;
        Parser::P.index++;
    }
    log_print("eval_expr5(): exit function\n");
}

// unary -
void Parser::eval_expr4(float &value)
{
    log_print("eval_expr4(): enter\n");
    int last = -1;
    if(Parser::P.tokens[Parser::P.index].type == Parser_types::PLUS || Parser::P.tokens[Parser::P.index].type == Parser_types::MINUS) {
        last = Parser::P.index++;
    }

    Parser::eval_expr5(value);
    if(last > -1) {
        if(Parser::P.tokens[last].type == Parser_types::MINUS) {
            log_print("\tfound unary %s\n", Parser::P.tokens[last].data);
            value = -value;
        }
    }
    log_print("eval_expr4(): exit function\n");
}

// ^
void Parser::eval_expr3(float &value)
{
    log_print("eval_expr3(): enter\n");
    Parser::eval_expr4(value);
    if(Parser::P.tokens[Parser::P.index].type == Parser_types::CARET) {
        float part = 0.0f;
        log_print("\tfound %s\n", Parser::P.tokens[Parser::P.index].data);
        Parser::P.index++;
        Parser::eval_expr4(part);

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
    log_print("eval_expr3(): exit function\n");
}

// * /
void Parser::eval_expr2(float &value)
{
    log_print("eval_expr2(): enter\n");
    Parser::eval_expr3(value);
    while(Parser::P.tokens[Parser::P.index].type == Parser_types::MULT || Parser::P.tokens[Parser::P.index].type == Parser_types::DIV) {
        float part = 0.0f;
        int last = 0;
        last = Parser::P.index++;

        Parser::eval_expr3(part);

        switch(Parser::P.tokens[last].type) {
            case Parser_types::MULT:
                log_print("\tfound %s\n", Parser::P.tokens[last].data);
                value *= part;
                break;

            case Parser_types::DIV:
                if(part == 0.0f) {
                    log_print("[Error] divide by zero!\n");
                    Parser::error_set(1);
                    value = 0.0f;
                    return;
                }
                log_print("\tfound %s\n", Parser::P.tokens[last].data);
                value /= part;
                break;
        };
    }
    log_print("eval_expr2(): exit function\n");
}

// + -
void Parser::eval_expr1(float &value)
{
    log_print("eval_expr1(): enter\n");
    Parser::eval_expr2(value);
    while(Parser::P.tokens[Parser::P.index].type == Parser_types::PLUS || Parser::P.tokens[Parser::P.index].type == Parser_types::MINUS) {
        float part = 0.0f;
        int last = 0;
        last = Parser::P.index++;
        Parser::eval_expr2(part);
        switch(Parser::P.tokens[last].type) {
            case Parser_types::PLUS:
                log_print("\tfound %s\n", Parser::P.tokens[last].data);
                value += part;
                break;

            case Parser_types::MINUS:
                log_print("\tfound %s\n", Parser::P.tokens[last].data);
                value -= part;
                break;
        };
    }
    log_print("eval_expr1(): exit function\n");
}

// =
void Parser::eval_expr0(float &value)
{
    MSG("eval_expr0(): enter\n");
     if(Parser::P.tokens[Parser::P.index].type == Parser_types::IDENTIFIER) {
         MSG("\tfound identifier: %s\n", Parser::P.tokens[Parser::P.index].data);
         int var_id = -1;
         if((var_id = find_var(Parser::P.tokens[Parser::P.index].data)) < 0) {
             if(Parser::P.tokens[Parser::P.index+1].type == Parser_types::PARENTHESIS_LEFT) {
                 Parser::eval_expr1(value);
                 return;
             }
         }
         if(var_id <= 0) {
             MSG("\tvariable not found\n");
             return;
         }

         MSG("\tfound variable: %s\n", Parser::P.tokens[Parser::P.index].data);
         float var_value = Parser::get_var_value(var_id);
         MSG("\tvalue var %s = %f\n", Parser::P.tokens[Parser::P.index].data, var_value);
         Parser::assign_var(var_id, var_value);
         value = var_value;
         Parser::eval_expr1(value);
         return;
     }
     Parser::eval_expr1(value);
     log_print("eval_expr0(): exit function\n");
}

// begin
void Parser::eval_expr(float &value)
{
    log_print("eval_expr(): enter\n");
    if(Parser::P.index > (Parser::P.num_tokens - 1)) {
        MSG("no expression\n");
        return;
    }

    if(Parser::P.tokens[Parser::P.index].data == "]") {
        value = 0;
        return;
    }
    Parser::eval_expr0(value);
    log_print("eval_expr(): exit function\n");
}

float Parser::syntax_parser()
{
    log_print("syntax_parser(): enter\n");
    float value = 0.0f;
    do {
        value = 0.0f;
        if(Parser::P.tokens[Parser::P.index].type == Parser_types::IDENTIFIER) {
            log_print("\ttoken is identificator\n");
            Parser::eval_expr(value);
        }
        else {
            log_print("\ttoken is not identificator\n");
            Parser::eval_expr1(value);
        }
        Parser::P.index++;
    } while(Parser::P.index < (Parser::P.num_tokens - 1));
    log_print("\tvalue expression = %f\n", value);
    log_print("syntax_parser(): exit function\n");
    return value;
}

void Parser::lexer_parser(const char *text)
{
    log_print("lexer_parser(): enter\n");
    char *ptr = (char *)text;

    while(*ptr != '\0') {
        while(*ptr == ' ')
            ptr++;
        while(*ptr == '\n')
            ptr++;
        if(isalpha(*ptr)) {
            char *begin = ptr;
            log_print("lexer_parser(): found alpha\n");

            while((isalpha(*ptr) || isdigit(*ptr)) && ((ptr-begin) < 63)) {
                ptr++;
            }

            Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::IDENTIFIER;
            Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * (ptr-begin));
            textcpy(begin, Parser::P.tokens[Parser::P.num_tokens].data, (ptr-begin));
            ptr--;
            Parser::P.num_tokens++;
        } else if(isdigit(*ptr)) {
            log_print("lexer_parser(): found a digit\n");
            char *begin = ptr;
            while(isdigit(*ptr))
                ptr++;

            if(*ptr == '.') {
                if(isdigit(*(ptr+1)))
                    ptr++;
                while(isdigit(*ptr))
                    ptr++;
                log_print("\tfloat number\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::FLOAT_NUMBER;
            } else {
                log_print("\tint number\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::INT_NUMBER;
            }
            Parser::P.tokens[Parser::P.num_tokens].data = (char*) malloc(sizeof(char) * (ptr - begin));
            textcpy(begin, Parser::P.tokens[Parser::P.num_tokens].data, (ptr - begin));
            ptr--;
            Parser::P.num_tokens++;
        } else if(isprint(*ptr)) {
            log_print("lexer_parser(): found a special character\n");
            switch(*ptr) {

            case '(':
                log_print("\ttoken '('\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::PARENTHESIS_LEFT;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case ')':
                log_print("\ttoken ')'\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::PARENTHESIS_RIGHT;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '!':
                log_print("\ttoken '!'\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::EXL;
                Parser::P.tokens[Parser::P.num_tokens].data = (char *)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '^':
                log_print("\ttoken '^'\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::CARET;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '*':
                log_print("\ttoken '*'\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::MULT;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '/':
                log_print("\ttoken '/'\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::DIV;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '+':
                log_print("\ttoken '+'\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::PLUS;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '-':
                log_print("\ttoken '-'\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::MINUS;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            default:
                log_print("\tundefined token\n");
                return;
            };
            Parser::P.num_tokens++;
        }
        ptr++;
        if(*ptr == '\0') {
            log_print("lexer_parser(): found null character: end expression\n");
            Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
            Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::EOE;
            textcpy("]", Parser::P.tokens[Parser::P.num_tokens].data, 1);
            Parser::P.num_tokens++;
        }
    }
    log_print("lexer_parser(): exit function\n");
}

unsigned Parser::parse_text(const char *text, float &value1)
{
    setlocale(LC_NUMERIC, "C");
    log_print("parse_text(): enter\n");
    Parser::P.error = 0;
    log_print("\tvalue R = %f\n", float_buildin_var[0].value_var);
    Parser::lexer_parser(text);
    value = Parser::syntax_parser();
    value1 = value;
    Parser::assign_var(50, value);
    log_print("\ttokens:\n\t");
    for(int i = 0; i < Parser::P.num_tokens; i++) {
       log_print("(%s) ", Parser::P.tokens[i].data);
    }
    for(int i = 0; i < Parser::P.num_tokens; i++) {
        free(Parser::P.tokens[i].data);
        Parser::P.tokens[i].data = NULL;
        Parser::P.tokens[i].type = 0;
    }
    log_print("\n");
    Parser::P.index = 0;
    Parser::P.num_tokens = 0;
    if(Parser::P.error != 0)
        return Parser::P.error;
    log_print("parse_text(): exit function\n");
    log_print("\n\n\n");
    return 0;
}
