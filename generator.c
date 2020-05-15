#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int no = 0, indent = 0;
void expr();
void stmt();
void stmts(int);

static void builtin_funcs() {
    printf("int neg(int neg_i) {\n");
    printf("    return -(neg_i);\n");
    printf("}\n");
    printf("int abs(int abs_i) {\n");
    printf("    if(abs_i > 0) {\n");
    printf("        return abs_i;\n");
    printf("    }\n");
    printf("    return neg(abs_i);\n");
    printf("}\n");
}

void a(int i) {
    if(i > 6) {
        printf("a%d6[%d]", no, i - 7);
    } else if(i > 4) {
        printf("a%d5[%d]", no, i - 5);
    } else {
        printf("a%d%d", no, i);
    }
}
int output(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    return printf("%*s", indent, "") + vprintf(fmt, va);
}

void ra() {
    a(1 + (rand()&7));
}

void singleton() {
    if((rand()&31) == 0) {
        printf("read()");
    } else 
    if(rand()&3) {
        ra();
    } else {
        if(no == 0 || rand()&15) {
            printf("%d", rand()&15);
        } else {
            printf("f%d(", rand()%no);
            expr();
            printf(", ");
            expr();
            printf(", ");
            printf("a%d%d", no, 5 + (rand()&1));
            printf(")");
        }
    }
}
void parent_expr() {
    printf("(");
    expr();
    printf(")");
}
void raw_expr() {
    if((rand()&7) == 0) {
        if(rand()&7) {
            expr();
        } else {
            parent_expr();
        }
    } else {
        singleton();
    }
    printf("%c", "+-*"[rand()%3]);
    if((rand()&7) == 0) {
        if(rand()&7) {
            expr();
        } else {
            parent_expr();
        }
    } else {
        singleton();
    }
}
void expr() {
    switch(rand()&31) {
        case 0:
            printf("abs(");
            raw_expr();
            printf(")");
            break;
        case 1:
            printf("neg(");
            raw_expr();
            printf(")");
            break;
        default:
            raw_expr();
            break;
    }
}
void stmt_return() {
    output("return "); expr(); printf(";\n");
}
void stmt_if() {
    output("if(");
    parent_expr();
    if((rand()&3) == 0) {
        static const char* relops[] = {
            "<=", ">=", "==", "!=", ">", "<"
        };
        printf(" %s ", relops[rand()%6]);
        parent_expr();
    }
    printf(") {\n");
    indent += 4;
    stmts(2);
    indent -=4;
    output("}");
    if(rand()&1) {
        printf(" else {\n");
        indent += 4;
        stmt(2);
        indent -= 4;
        output("    }");
    }
    printf("\n");
}
void stmt_while() {
    static int dep = 0;
    if(dep < 4) {
        output("i%d[%d] = 0;\n", no, dep);
        output("while(i%d[%d] < %d) {\n", no, dep, 1 + (rand()&3));
        indent += 4;
        output("i%d[%d] = i%d[%d] + 1;\n", no, dep, no, dep);
        ++dep;
        stmts(2);
        --dep;
        indent -= 4;
        output("}\n");
    } else {
        stmt();
    }
}
void stmt() {
    switch(rand()&15) {
        case 0:
            stmt_if();
            return;
        case 1:
            output("write(");
            expr();
            printf(");\n");
            return;
        case 2:
                stmt_while();
                return;
        case 14:
                if((rand()&3) == 0) {
                    stmt_return();
                    return;
                }
                //fall
        default:
                output("");
                ra();
                printf(" = ");
                expr();
                printf(";\n");
    }
}
void stmts(int n) {
    for(int i = 0; i < n || ((rand() & 1) && i < 2 * n); ++i) {
        stmt();
    }
}
void gen_func() {
    output("int f%d(int a%d1, int a%d2, int a%d6[2]) {\n", no, no, no, no);
    indent += 4;
    output("int a%d3 = %d;\n", no, rand()&15);
    output("int a%d4 = %d;\n", no, rand()&15);
    output("int a%d5[2];\n", no);
    output("int i%d[4];\n", no);
    output("a%d5[0] = 17173;\n", no);
    output("a%d5[1] = 4399;\n", no);
    stmts(5);
    stmt_return();
    indent -= 4;
    output("}\n");
}
void gen_main() {
    output("int main() {\n");
    indent += 4;
    output("int a%d1 = %d;\n", no, rand()&15);
    output("int a%d2 = %d;\n", no, rand()&15);
    output("int a%d3 = %d;\n", no, rand()&15);
    output("int a%d4 = %d;\n", no, rand()&15);
    output("int a%d5[2];\n", no);
    output("int a%d6[2];\n", no);
    output("int i%d[4];\n", no);
    output("a%d5[0] = 10151;\n", no);
    output("a%d5[1] = 98808;\n", no);
    output("a%d6[0] = 114;\n", no);
    output("a%d6[1] = 514;\n", no);
    stmts(5);
    stmt_return();
    indent -= 4;
    output("}\n");
}
int main() {
    srand(time(NULL));
    builtin_funcs();
    for(no = 0; no <= 5; ++no) {
        gen_func();
    }
    no = 6;
    gen_main();
    return 0;
}
