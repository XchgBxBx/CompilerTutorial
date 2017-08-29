#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>


char look;
int labelCount;

void init();
void nextChar();
void error(char *fmt, ...);
void fatal(char *fmt, ...);
void expected(char *fmt, ...);
void match(char c);
char getName();
char getNum();
void emit(char *fmt, ...);

void other();
void program();
void block();
int newLabel();
int postLabel(int lbl);
void doIf();
void condition();
void doWhile();
void doLoop();
void doRepeat();
void doFor();
void expression();

void init()
{
	labelCount = 0;
	nextChar();
}

void nextChar()
{
	look = getchar();
}

void error(char *fmt, ...)
{
	va_list args;

	fputs("Error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fputc('\n', stderr);
}

void fatal(char *fmt, ...)
{
	va_list args;

	fputs("Error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fputc('\n', stderr);

	exit(1);
}

void expected(char *fmt, ...)
{
	va_list args;

	fputs("Error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fputs(" expected!\n", stderr);

	exit(1);
}


void match(char c)
{
	if(look != c)
		expected("'%c'", c);
	nextChar();
}

char getName()
{
	char name;

	if(!isalpha(look))
		expected("Name");
	name = toupper(look);
	nextChar();

	return name;
}

char getNum()
{
	char num;

	if(!isdigit(look))
		expected("Integer");
	num = look;
	nextChar();

	return num;
}

void emit(char *fmt, ...)
{
	va_list args;

	putchar('\t');

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	putchar('\n');
}

void other()
{
	emit("# %c", getName());
}

void program()
{
	block();
	if(look != 'e')
		expected("End");
	emit("END");
}

void block()
{
	int follow;

	follow = 0;
	
	while(!follow)
	{
		switch(look)
		{
		case 'i':
			doIf();
			break;
		case 'w':
			doWhile();
			break;
		case 'p':
			doLoop();
			break;
		case 'r':
			doRepeat();
			break;
		case 'f':
			doFor();
			break;
		case 'e':
		case 'l':
		case 'u':
			follow = 1;
			break;
		default:
			other();
			break;
		}
	}
}

int newLabel()
{
	return labelCount++;
}

int postLabel(int lbl)
{
	printf("L%d:\n", lbl);
}

void doIf()
{
	int l1, l2;

	match('i');
	condition();
	l1 = newLabel();
	l2 = l1;
	emit("JZ L%d", l1);
	block();
	if(look == 'l')
	{
		match('l');
		l2 = newLabel();
		emit("JMP L%d", l2);
		postLabel(l1);
		block();
	}
	match('e');
	postLabel(l2);
}

void condition()
{
	emit("# condition");
}

void doWhile()
{
	int l1, l2;

	match('w');
	l1 = newLabel();
	l2 = newLabel();
	postLabel(l1);
	condition();
	emit("JZ L%d", l2);
	block();
	match('e');
	emit("JMP L%d", l1);
	postLabel(l2);
}

void doLoop()
{
	int l;

	match('p');
	l = newLabel();
	postLabel(l);
	block();
	match('e');
	emit("JMP L%d", l);
}

void doRepeat()
{
	int l;

	match('r');
	l = newLabel();
	postLabel(l);
	block();
	match('u');
	condition();
	emit("JZ L%d", l);
}

void doFor()
{
	int l1, l2;
	char name;

	match('f');
	l1 = newLabel();
	l2 = newLabel();
	name = getName();
	match('=');
	expression();
	emit("DEC AX");
	emit("MOV [%c], AX", name);
	expression();
	emit("PUSH AX");
	postLabel(l1);
	emit("MOV AX, [%c]", name);
	emit("INC AX");
	emit("MOV [%c], AX", name);
	emit("POP BX");
	emit("PUSH BX");
	emit("CMP AX, BX");
	emit("JG L%d", l2);
	block();
	match('e');
	emit("JMP L%d", l1);
	postLabel(l2);
	emit("POP AX");
}

void expression()
{
	emit("# EXPR");
}

int main(int argc, char **argv)
{
	init();
	program();

	return 0;
}
