/* source code courtesy of Frank Thomas Braun */
/* Generation of the graph of the syntax tree */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc3.h"
#include "calc3.tab.h"
extern FILE *yyout;

int del = 1;
int eps = 3;

void graphInit (void);
void graphFinish();
void graphBox (char *s, int *w, int *h);
void graphDrawBox (char *s, int c, int l);
void graphDrawArrow (int c1, int l1, int c2, int l2);
void exNode (nodeType *p, int c, int l, int *ce, int *cm);
/**************************************************************************
***/

int ex (nodeType *p) {
	int rte, rtm;
	graphInit ();
	exNode (p, 0, 0, &rte, &rtm);
	graphFinish();
	return 0;
}

void exNode
( nodeType *p,
int c, int l,
int *ce, int *cm
)
{
int w, h;
char *s;
int cbar;
int k;
int che, chm;
int cs;
char word[20];
if (!p) return;
strcpy (word, "???");
s = word;
switch(p->type) {
case typeCon: sprintf (word, "c(%s)", p->con.val); break;
case typeId: sprintf (word, "id(%c)", p->id.i + 'A'); break;
case typeOper:
switch(p->opr.oper){
case PRINT: s = "print"; break;
case ';': s = "[;]"; break;
case '=': s = "[=]"; break;
case '+': s = "[+]"; break;
case '-': s = "[-]"; break;
case '*': s = "[*]"; break;
case '/': s = "[/]"; break;
case '<': s = "[<]"; break;
case '>': s = "[>]"; break;
case GE: s = "[>=]"; break;
case LE: s = "[<=]"; break;
case NE: s = "[!=]"; break;
case EQ: s = "[==]"; break;
case '#': s = "[#]"; break;
case '~': s="[~]";break;
case '^': s="[^]";break;
case INC: s="[++]";break;
case DEC: s="[--]";break;
}
break;
}
graphBox (s, &w, &h);
cbar = c;
*ce = c + w;
*cm = c + w / 2;

if (p->type == typeCon || p->type == typeId || p->opr.nops == 0) {
	graphDrawBox (s, cbar, l);
	return;
}

cs = c;
for (k = 0; k < p->opr.nops; k++) {
	exNode (p->opr.op[k], cs, l+h+eps, &che, &chm);
	cs = che;
}

if (w < che - c) {
	cbar += (che - c - w) / 2;
	*ce = che;
	*cm = (c + che) / 2;
}

graphDrawBox (s, cbar, l);

cs = c;
for (k = 0; k < p->opr.nops; k++) {
	exNode (p->opr.op[k], cs, l+h+eps, &che, &chm);
	graphDrawArrow (*cm, l+h, chm, l+h+eps-1);
	cs = che;
}
}

#define lmax 200
#define cmax 200
char graph[lmax][cmax];
int graphNumber = 0;
void graphTest (int l, int c)
{ int ok;
ok = 1;
if (l < 0) ok = 0;
if (l >= lmax) ok = 0;
if (c < 0) ok = 0;
if (c >= cmax) ok = 0;
if (ok) return;
//printf ("\n+++error: l=%d, c=%d not in drawing rectangle 0, 0 ... %d, %d",l, c, lmax, cmax);
fprintf (yyout, "\n+++error: l=%d, c=%d not in drawing rectangle 0, 0 ... %d, %d",l, c, lmax, cmax);
exit(1);
}
void graphInit (void) {
int i, j;
for (i = 0; i < lmax; i++) {
	for (j = 0; j < cmax; j++) {
		graph[i][j] = ' ';
	}
}
}
void graphFinish() {
int i, j;
for (i = 0; i < lmax; i++) {
for (j = cmax-1; j > 0 && graph[i][j] == ' '; j--);
graph[i][cmax-1] = 0;
if (j < cmax-1) graph[i][j+1] = 0;
if (graph[i][j] == ' ') graph[i][j] = 0;
}
for (i = lmax-1; i > 0 && graph[i][0] == 0; i--);
//printf ("\n\nGraph %d:\n", graphNumber++); 
fprintf (yyout,"\n\nGraph %d:\n", graphNumber++);
for (j = 0; j <= i; j++) /* printf ("\n%s", graph[j]); */ fprintf (yyout, "\n%s", graph[j]);
//printf("\n");
fprintf(yyout, "\n");
}
void graphBox (char *s, int *w, int *h) {
*w = strlen (s) + del;
*h = 1;
}
void graphDrawBox (char *s, int c, int l) {
int i;
graphTest (l, c+strlen(s)-1+del);
for (i = 0; i < strlen (s); i++) {
graph[l][c+i+del] = s[i];
}
}
void graphDrawArrow (int c1, int l1, int c2, int l2) {
int m;
graphTest (l1, c1);
graphTest (l2, c2);
m = (l1 + l2) / 2;
while (l1 != m) { graph[l1][c1] = '|'; if (l1 < l2) l1++; else l1--; }
while (c1 != c2) { graph[l1][c1] = '-'; if (c1 < c2) c1++; else c1--; }
while (l1 != l2) { graph[l1][c1] = '|'; if (l1 < l2) l1++; else l1--; }
graph[l1][c1] = '|';
}