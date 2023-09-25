%{
/* calc3.l */
#include <stdlib.h>
#include "calc3.h"
#include "calc3.tab.h"
void yyerror(char *);
char string_buf[1000];
char *string_buf_ptr;
%}
%x str
%%
[a-z] {
yylval.sIndex = *yytext - 'a';
return VARIABLE;
}

[A-Z] {
yylval.sIndex = *yytext - 'A';
return VARIABLE;
}

0 {
//yylval.iValue = atoi(yytext);
//return INTEGER;
/*
["]*[a-zA-Z]*[0-9]*["] { //modificare pentru expresii
   
yylval.iValue = yytext;
return INTEGER;
}

*/
}
\"      {string_buf_ptr = string_buf; BEGIN(str); }

<str>\"      { /* saw closing quote - all done */
BEGIN(INITIAL);
*string_buf_ptr = '\0';
/* return string constant token type and
* value to parser
*/
yylval.iValue=strdup(string_buf);
//printf("%s",yylval.iValue);
return INTEGER;
}

<str>\n      {
//add error but for now I am fine
}

<str>\\[0-7]{1,3} {
/* octal escape sequence */
int result;

(void) sscanf( yytext + 1, "%o", &result );

if ( result > 0xff )
/* error, constant is out-of-bounds */
*string_buf_ptr++ = result;
}

<str>\\[0-9]+ {
/* generate error - bad escape sequence; something
* like '\48' or '\0777777'
*
*/
}
<str>\\n  *string_buf_ptr++ = '\n';
<str>\\t  *string_buf_ptr++ = '\t';
<str>\\r  *string_buf_ptr++ = '\r';
<str>\\b  *string_buf_ptr++ = '\b';
<str>\\f  *string_buf_ptr++ = '\f';
<str>\\(.|\n)  *string_buf_ptr++ = yytext[1];


<str>[^\\\n\"]+      {
char *yptr = yytext;
while ( *yptr )
*string_buf_ptr++ = *yptr++;
}

[-()<>=+*/;,~{}.^#] {
return *yytext;
}
">=" return GE;
"<=" return LE;
"==" return EQ;
"!=" return NE;
"++" return INC;
"--" return DEC;
"print" return PRINT;/* am scos while, if, else, print*/
[ \t\n]+ ; /* ignore whitespace */
. yyerror("Unknown character");
%%
int yywrap(void) {
return 1;
}