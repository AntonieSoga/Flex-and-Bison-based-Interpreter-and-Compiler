#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "calc3.h"
#include "calc3.tab.h"
extern FILE *yyout;
char *repetaStr (char *str, char *number) { //inmultire siruri
int count = atoi(number);

if (count == 0) return NULL;
char *ret = malloc (strlen (str) * count );
if (ret == NULL) return NULL;
strcpy (ret, str);
while (--count > 0) {
strcat (ret, str);
}
return ret;
}

char* toggleCase(char* str) {
    size_t length = strlen(str);
    char* result = malloc(length + 1);
    if (result == NULL) return NULL;

    for (size_t i = 0; i < length; i++) {
        if (islower(str[i])) {
            result[i] = toupper(str[i]);
        } else if (isupper(str[i])) {
            result[i] = tolower(str[i]);
        } else {
            result[i] = str[i];
        }
    }
    result[length] = '\0';
    return result;
}

char *incrementFirstChar(char *str) {
    size_t length = strlen(str);
    char *result = malloc(length + 1);
    if (result == NULL) return NULL;

    if (length == 0) {
        result[0] = '\0';
    } else {
        result[0] = (str[0] == 'z') ? 'a' : str[0] + 1;
        strncpy(result + 1, str + 1, length - 1);
        result[length] = '\0';
    }

    return result;
}
char *decrementFirstChar(char *str) {
    size_t length = strlen(str);
    char *result = malloc(length + 1);
    if (result == NULL) return NULL;

    if (length == 0) {
        result[0] = '\0';
    } else {
        result[0] = (str[0] == 'a') ? 'z' : str[0] - 1;
        strncpy(result + 1, str + 1, length - 1);
        result[length] = '\0';
    }

    return result;
}

char *getLastNCharacters(char *str, char *number) {
    int count = atoi(number);
    int length = strlen(str);
    if (count <= 0 || count >= length) return strdup(str);  // Return the whole string

    char *result = malloc(count + 1);
    if (result == NULL) return NULL;

    strncpy(result, str + length - count, count);
    result[count] = '\0';
    return result;
}

char *getFirstNCharacters(char *str, char *number) {
    int count = atoi(number);
    int length = strlen(str);
    if (count <= 0 || count >= length) return strdup(str);  // Return the whole string

    char *result = malloc(count + 1);
    if (result == NULL) return NULL;

    strncpy(result, str, count);
    result[count] = '\0';
    return result;
}

char *stringDifference(char *str1, char *str2) {
    char *result = malloc(strlen(str1) + 1);
    if (result == NULL) return NULL;

    char *ptr = strstr(str1, str2);
    if (ptr == NULL) {
        strcpy(result, str1);  // If str2 is not found in str1, return a copy of str1
    } else {
        strncpy(result, str1, ptr - str1);
        result[ptr - str1] = '\0';
        strcat(result, ptr + strlen(str2));
    }

    return result;
}

int stringEquals(char *str1, char *str2) {
    return strcmp(str1, str2) == 0;
}

int stringNotEquals(char *str1, char *str2) {
    return strcmp(str1, str2) != 0;
}


int stringGreaterThan(char *str1, char *str2) {
    return strcmp(str1, str2) > 0;
}

int stringGreaterThanOrEqual(char *str1, char *str2) {
    return strcmp(str1, str2) >= 0;
}

int stringLessThan(char *str1, char *str2) {
    return strcmp(str1, str2) < 0;
}

int stringLessThanOrEqual(char *str1, char *str2) {
    return strcmp(str1, str2) <= 0;
}

char* ex(nodeType *p) {
if (!p) return 0;
switch(p->type) {
case typeCon: return p->con.val;
case typeId: return sym[p->id.i];
case typeOper:
switch(p->opr.oper) {
case PRINT: fprintf(yyout,"%s\n", ex(p->opr.op[0])); return 0;
case ';': ex(p->opr.op[0]); return ex(p->opr.op[1]);
case '=': return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
case '+': return strcat(ex(p->opr.op[0]),ex(p->opr.op[1])); //strcat e functia de concatenare
case '-': return stringDifference(ex(p->opr.op[0]), ex(p->opr.op[1]));
case '/':  return getLastNCharacters(ex(p->opr.op[0]), ex(p->opr.op[1]));
case '#':  return getFirstNCharacters(ex(p->opr.op[0]), ex(p->opr.op[1]));
case '<': return stringLessThan(ex(p->opr.op[0]), ex(p->opr.op[1])) ? "1" : "0";
case '>': return stringGreaterThan(ex(p->opr.op[0]), ex(p->opr.op[1])) ? "1" : "0";
case GE: return stringGreaterThanOrEqual(ex(p->opr.op[0]), ex(p->opr.op[1])) ? "1" : "0";
case LE: return stringLessThanOrEqual(ex(p->opr.op[0]), ex(p->opr.op[1])) ? "1" : "0";
case NE: return stringNotEquals(ex(p->opr.op[0]), ex(p->opr.op[1])) ? "1" : "0";
case EQ: return stringEquals(ex(p->opr.op[0]), ex(p->opr.op[1])) ? "1" : "0";
case '~': fprintf(yyout, "Lungimea sirului este %zu", strlen(ex(p->opr.op[0]))); return 0;
 //return 0;
case '*': return repetaStr(ex(p->opr.op[0]),ex(p->opr.op[1]));
case '^': fprintf(yyout, "%s\n", toggleCase(ex(p->opr.op[0]))); return 0;
case INC: return incrementFirstChar(ex(p->opr.op[0]));
case DEC: return decrementFirstChar(ex(p->opr.op[0]));
}
}
return 0;
}
