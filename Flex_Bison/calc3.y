%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "calc3.h"
extern FILE *yyin;
extern FILE *yyout;
/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(char* value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);
void yyerror(char *s);
char* sym[26]; /* symbol table */
%}
%union {
    char* iValue; /* integer value */
    char sIndex; /* symbol table index */
    nodeType *nPtr; /* node pointer */
};
%token PRINT EQ NE '>' '<' GE LE
%token <iValue> INTEGER
%token <sIndex> VARIABLE 
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '#'
%right '^' '~' INC DEC
%nonassoc '(' ')'
%type <nPtr> stmt expr
%%
function:
    function stmt { ex($2); freeNode($2); }
    |
    ;
stmt:
    ';' { $$ = opr(';', 2, NULL, NULL); }
    | ',' { $$ = opr(';', 2, NULL, NULL); } //declaratia multipla unde e ,
    | expr ';' { $$ = $1; }
    | expr ',' { $$ = $1; }
    | PRINT expr ';' { $$ = opr(PRINT, 1, $2); }
    | VARIABLE '=' expr ';' { $$ = opr('=', 2, id($1), $3); }
    | VARIABLE '=' expr ',' { $$ = opr('=', 2, id($1), $3); }
    ;
expr:
    INTEGER { $$ = con($1); }
    | VARIABLE { $$ = id($1); }
    | '~' expr { $$ = opr('~', 1, $2); }
    | '^' expr { $$ = opr('^', 1, $2); }
    
    | expr INC %prec INC  { $$ =opr(INC,1,$1);}
    | INC expr %prec INC {$$=opr(INC,1,$2);}
    | expr DEC %prec DEC{$$ =opr(DEC,1,$1);}
    | DEC expr %prec DEC { $$=opr(DEC,1,$2);}
    | expr '+' expr { $$ = opr('+', 2, $1, $3); }
    | expr '-' expr { $$ = opr('-', 2, $1, $3); }
    | expr '*' expr { $$ = opr('*', 2, $1, $3); }
    | expr '/' expr { $$ = opr('/', 2, $1, $3); }
    | expr '#' expr { $$ = opr('#', 2, $1, $3); }
    | expr '<' expr { $$ = opr('<', 2, $1, $3); }
    | expr '>' expr { $$ = opr('>', 2, $1, $3); }
    | expr GE expr { $$ = opr(GE, 2, $1, $3); }
    | expr LE expr { $$ = opr(LE, 2, $1, $3); }
    | expr NE expr { $$ = opr(NE, 2, $1, $3); }
    | expr EQ expr { $$ = opr(EQ, 2, $1, $3); }
    | '(' expr ')' { $$ = $2; }
    ;
%%
nodeType *con(char* value) {
    nodeType *p;
    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->con.val = strdup(value);
    p->type = typeCon;
    return p;
}
nodeType *id(int i) {
    nodeType *p;
    /* allocate node */
    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeId;
    p->id.i = i;
    return p;
}
nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;
    /* allocate node, extending op array */
    if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");
    /* copy information */
    p->type = typeOper;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}
void freeNode(nodeType *p) {
    int i;
    if (!p) return;
    if (p->type == typeOper) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free(p);
}
void yyerror(char *s) {
    fprintf(stdout, "%s\n", s);
}
int main(int argc, char *argv[]) {
    if (argc == 3) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            printf("Eroare deschidere fisier de intrare!");
            return -1;
        }
        yyout = fopen(argv[2], "w");
        if (yyout == NULL) {
            printf("Eroare deschidere fisier de iesire!\n");
            return -2;
        }
    } else if (argc == 2) {
        printf("Trebuie sa ai si fisier de iesire si fisier de intrare!\n In cazul asta se preiau date de la tastatura.\n");
        yyin = stdin;
    } else {
        printf(" Se preiau date din consola:\n");
        yyin = stdin;
    }

    yyparse();
    fclose(yyin);
    fclose(yyout);
    return 1;
}
