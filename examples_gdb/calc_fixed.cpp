#include <cstdio>
#include <cfloat>

char peek_not_eof(std::FILE* in)
{
    char c = std::getc(in);
    if (std::feof(in) || c == '\n')
        return '\0';
    std::ungetc(c, in);
    return c;
}

void drop_char(std::FILE* in)
{
    std::getc(in);
}

enum retcode_t {
    E_OK,
    E_EOF,
    E_INVAL
};

retcode_t eval_primary(std::FILE* in, double* value)
{
    bool negate = false;
    while (char sgn = peek_not_eof(in)) {
        if (sgn != '-')
            break;
        negate = !negate;
        drop_char(in);
    }

    bool seen_digit = false;
    double cur_val = 0;
    while (char c = peek_not_eof(in)) {
        if (!(c >= '0' && c <= '9')) {
            if (!seen_digit)
                return E_INVAL;
            break;
        }
        seen_digit = true;
        cur_val = cur_val * 10 + (c - '0');
        drop_char(in);
    }
    if (!seen_digit)
        return E_EOF;
    *value = negate ? -cur_val : cur_val;
    return E_OK;
}

retcode_t eval_term(std::FILE* in, double* value)
{
    double prim;
    retcode_t rc = eval_primary(in, &prim);
    if (rc != E_OK)
        return rc;

    double cur_value = prim;
    while (char op = peek_not_eof(in)) {
        if (op != '*')
            break;
        drop_char(in);

        rc = eval_primary(in, &prim);
        if (rc != E_OK)
            return rc;
        cur_value *= prim;
    }

    *value = cur_value;
    return E_OK;
}

retcode_t eval_expr(std::FILE* in, double* value)
{
    double term;
    retcode_t rc = eval_term(in, &term);
    if (rc != E_OK)
        return rc;

    double cur_value = term;
    while (char op = peek_not_eof(in)) {
        if (!(op == '+' || op == '-'))
            return E_INVAL;
        drop_char(in);

        rc = eval_term(in, &term);
        if (rc != E_OK)
            return rc;

        if (op == '+')
            cur_value += term;
        else
            cur_value -= term;
    }

    *value = cur_value;
    return E_OK;
}

int main()
{
    double result;
    retcode_t rc = eval_expr(stdin, &result);
    switch (rc) {
    case E_OK:
        std::printf("%10lf\n", result);
        break;
    case E_EOF:
        std::printf("ERROR. Unexpected end of input\n");
        break;
    case E_INVAL:
        std::printf("ERROR. Invalid expression\n");
        break;
    }
    return 0;
}
