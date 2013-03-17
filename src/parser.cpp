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

int Parser::find_identifier(const char *name, int type)
{
    int i = 0;
    while(Builtin::identfiers[i].type != 0) {
        if(!strcmp(Builtin::identfiers[i].name, name)) {
            if(Builtin::identfiers[i].type == type)
            {MSG("id var = %i", Builtin::identfiers[i].global_id);return Builtin::identfiers[i].global_id;}
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
            flag_found_pr = 1;
            Parser::P.index++;
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
    MSG("atom = %s\n", Parser::P.tokens[Parser::P.index].data);

    switch(Parser::P.tokens[Parser::P.index].type) {
    case Parser_types::IDENTIFIER:
        if((id = find_func(Parser::P.tokens[Parser::P.index].data)) != -1) {
            last = Parser::P.index;
            value = call_func(id);
        } else if((id = find_var(Parser::P.tokens[Parser::P.index].data)) != -1) {
            last = Parser::P.index;
            value = Parser::get_var_value(id);
            MSG("get var: %s = %f", Parser::P.tokens[Parser::P.index].data, value);
            Parser::P.index++;
        } else {
            if(Parser::P.tokens[Parser::P.index+1].type == Parser_types::PARENTHESIS_LEFT)
                MSG("unknown func\n");
        }
        break;
    case Parser_types::FLOAT_NUMBER:
            value = (float)atof((const char*)Parser::P.tokens[Parser::P.index].data);
            MSG("found float num\n");
            Parser::P.index++;
            break;

        case Parser_types::INT_NUMBER:
            value = (float)atoi((const char*)Parser::P.tokens[Parser::P.index].data);
            MSG("found int number\n");
            MSG("num = %s\n", Parser::P.tokens[Parser::P.index].data);

            Parser::P.index++;
            break;

        case Parser_types::PARENTHESIS_RIGHT:
            return;
        default:
            MSG("unknow\n");
    };
}

// ( )
void Parser::eval_expr6(float &value)
{
    MSG("EVAL6\n");
    if(Parser::P.tokens[Parser::P.index].type == Parser_types::PARENTHESIS_LEFT) {
        Parser::P.index++;

        Parser::eval_expr(value);

        if(Parser::P.tokens[Parser::P.index].type != Parser_types::PARENTHESIS_RIGHT) {
            MSG("expected )\n");
            return;
        }
        Parser::P.index++;
    } else {
        Parser::atom(value);
    }
}

// !
void Parser::eval_expr5(float &value)
{
    MSG("EVAL5\n");
    Parser::eval_expr6(value);

    if(Parser::P.tokens[Parser::P.index].type == Parser_types::EXL) {
        MSG("found !\n");
        if(value == 0.0f) {
            value = 1.0f;
            Parser::P.index++;
            return;
        }

        if(value < 0.0f) {
            MSG("skip factorial\n");
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
}

// unary -
void Parser::eval_expr4(float &value)
{
    MSG("EVAL4\n");
    int last = -1;
    if(Parser::P.tokens[Parser::P.index].type == Parser_types::PLUS || Parser::P.tokens[Parser::P.index].type == Parser_types::MINUS) {
        last = Parser::P.index++;
        MSG("unary + - %i\n", last);
    }

    Parser::eval_expr5(value);
    MSG("value unary -   = %f\n", value);
    if(last > -1) {
        MSG("unary   token: %s\n", Parser::P.tokens[last].data);
        if(Parser::P.tokens[last].type == Parser_types::MINUS) {
            MSG("found unary -\n");
            value = -value;
            MSG("value ----- : %f", value);
        }
    }
}

// ^
void Parser::eval_expr3(float &value)
{
    MSG("EVAL3\n");
    Parser::eval_expr4(value);
    if(Parser::P.tokens[Parser::P.index].type == Parser_types::CARET) {
        float part = 0.0f;
        MSG("found power\n");
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
}

// * /
void Parser::eval_expr2(float &value)
{
    MSG("EVAL2\n");
    Parser::eval_expr3(value);
    while(Parser::P.tokens[Parser::P.index].type == Parser_types::MULT || Parser::P.tokens[Parser::P.index].type == Parser_types::DIV) {
        float part = 0.0f;
        int last = 0;
        MSG("found * /\n");
        MSG("%s\n", Parser::P.tokens[Parser::P.index].data);

        last = Parser::P.index++;

        Parser::eval_expr3(part);

        switch(Parser::P.tokens[last].type) {
            case Parser_types::MULT:
                value *= part;
                break;

            case Parser_types::DIV:
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

// + -
void Parser::eval_expr1(float &value)
{
    MSG("EVAL1\n");
    Parser::eval_expr2(value);
    while(Parser::P.tokens[Parser::P.index].type == Parser_types::PLUS || Parser::P.tokens[Parser::P.index].type == Parser_types::MINUS) {
        float part = 0.0f;
        int last = 0;
        MSG("found + -\n");
        last = Parser::P.index++;
        MSG("%s\n", Parser::P.tokens[Parser::P.index].data);
        Parser::eval_expr2(part);
        switch(Parser::P.tokens[last].type) {
            case Parser_types::PLUS:
                value += part;
                break;

            case Parser_types::MINUS:
                value -= part;
                break;
        };
    }
}

// =
void Parser::eval_expr0(float &value)
{
    MSG("EVAL0\n");
     if(Parser::P.tokens[Parser::P.index].type == Parser_types::IDENTIFIER) {
         MSG("found ident = %s\n", Parser::P.tokens[Parser::P.index].data);
         int var_id = -1;
         if((var_id = find_var(Parser::P.tokens[Parser::P.index].data)) < 0) {
             if(Parser::P.tokens[Parser::P.index+1].type == Parser_types::PARENTHESIS_LEFT) {
                 Parser::eval_expr1(value);
                 return;
             }
         }
         MSG("\nololololololo\n");
         if(var_id <= 0) {
             MSG("EVAL0 exit var_id < 0");
             return;
         }

         MSG("found var: %s\n", Parser::P.tokens[Parser::P.index].data);
         float var_value = Parser::get_var_value(var_id);
         MSG("Value var x = %f\n", var_value);
         Parser::assign_var(var_id, var_value);
         value = var_value;
         MSG("EVAL0 value = %f\n", value);
         Parser::eval_expr1(value);
         return;
     }
     Parser::eval_expr1(value);
}

// begin
void Parser::eval_expr(float &value)
{
    MSG("EVAL\n");
    if(Parser::P.index > (Parser::P.num_tokens - 1)) {
        MSG("no expression\n");
        return;
    }

    if(Parser::P.tokens[Parser::P.index].data == "]") {
        value = 0;
        return;
    }
    Parser::eval_expr0(value);
}

float Parser::syntax_parser()
{
    float value = 0.0f;
    do {
        value = 0.0f;
        if(Parser::P.tokens[Parser::P.index].type == Parser_types::IDENTIFIER) {
            MSG("token is identificator\n");
            Parser::eval_expr(value);
        }
        else {
            Parser::eval_expr1(value);
            MSG("token is not identificator\n");
        }
        Parser::P.index++;
    } while(Parser::P.index < (Parser::P.num_tokens - 1));
    MSG("end syntax\n");
    MSG("value = %f\n", value);
    return value;
}

void Parser::lexer_parser(const char *text)
{
    MSG("[DEBUG] lexer_parser()\n");
    char *ptr = (char *)text;

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

            Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::IDENTIFIER;
            Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * (ptr-begin));
            textcpy(begin, Parser::P.tokens[Parser::P.num_tokens].data, (ptr-begin));
            ptr--;
            Parser::P.num_tokens++;
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
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::FLOAT_NUMBER;
            } else {
                MSG("int num\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::INT_NUMBER;
            }
            Parser::P.tokens[Parser::P.num_tokens].data = (char*) malloc(sizeof(char) * (ptr - begin));
            textcpy(begin, Parser::P.tokens[Parser::P.num_tokens].data, (ptr - begin));
            ptr--;
            Parser::P.num_tokens++;
        } else if(isprint(*ptr)) {
            MSG("isprint! : ");
            switch(*ptr) {

            case '(':
                MSG("(\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::PARENTHESIS_LEFT;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case ')':
                MSG(")\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::PARENTHESIS_RIGHT;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '!':
                MSG("!\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::EXL;
                Parser::P.tokens[Parser::P.num_tokens].data = (char *)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '^':
                MSG("^\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::CARET;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '*':
                MSG("*\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::MULT;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '/':
                MSG("/\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::DIV;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '+':
                MSG("+\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::PLUS;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            case '-':
                MSG("-\n");
                Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::MINUS;
                Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
                textcpy(ptr, Parser::P.tokens[Parser::P.num_tokens].data, 1);
                break;

            default:
                MSG("undefined token\n");
                return;
            };
            Parser::P.num_tokens++;
        }
        ptr++;
        if(*ptr == '\0') {
            MSG("null_symbol\n");
            Parser::P.tokens[Parser::P.num_tokens].data = (char*)malloc(sizeof(char) * 1);
            Parser::P.tokens[Parser::P.num_tokens].type = Parser_types::EOE;
            textcpy("]", Parser::P.tokens[Parser::P.num_tokens].data, 1);
            Parser::P.num_tokens++;
        }
    }
    MSG("close : lexer_parser()\n");
}

void Parser::parser_create()
{
    Parser_types::float_var_value test[2] = {{50, 0.0}, //x
                                            {0, 0.0}};
    MSG("parser_create x = %f\n", Parser::get_var_value(50));
}

void Parser::parse_text(const char *text)
{
    setlocale(LC_NUMERIC, "C");
    //Parser::assign_var(50, 8.0);
    MSG("value x = %f\n", float_buildin_var[0].value_var);
    Parser::lexer_parser(text);
    value = Parser::syntax_parser();
    Parser::assign_var(50, value);
    for(int i = 0; i < Parser::P.num_tokens; i++) {
        MSG("(%s) ", Parser::P.tokens[i].data);
    }
    MSG("\n");
    for(int i = 0; i < Parser::P.num_tokens; i++) {
        free(Parser::P.tokens[i].data);
        Parser::P.tokens[i].data = NULL;
        Parser::P.tokens[i].type = 0;
    }
    Parser::P.index = 0;
    Parser::P.num_tokens = 0;
}
