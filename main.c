#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>


char look;


void init();
void nextChar();

void error(char *fmt, ...);
void fatal(char *fmt, ...);
void expected(char *fmt, ...);
void match(char c);
char getName();
char getNum();

void emit(char *fmt, ...);

void expression();
void term();
void factor();

int isAddOp(char c);

void add();
void subtract();
void multiply();
void divide();

void init()
{
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

int isAddOp(char c)
{
	return (c == '+' || c == '-');
}

void add()
{
	match('+');
	term();
	emit("POP BX");
	emit("ADD AX,BX");
}

void subtract()
{
	match('-');
	term();
	emit("POP BX");
	emit("SUB AX, BX");
	emit("NEG AX");
}

void multiply()
{
	match('*');
	factor();
	emit("POP BX");
	emit("IMUL BX");
}

void divide()
{
	match('/');
	factor();
	emit("POP BX");
	emit("XCHG AX, BX");
	emit("CWD");
	emit("IDIV BX");
}

void factor()
{
	if(look == '(')
	{
		match('(');
		expression();
		match(')');
	}
	else
		emit("MOV AX, %c", getNum());
}

void term()
{
	factor();
	while(look == '*' || look == '/')
	{
		emit("PUSH AX");
		switch(look)
		{
		case '*':
			multiply();
			break;
		case '/':
			divide();
			break;
		default:
			expected("MulOp");
			break;
		}
	}
}

void expression()
{
	if(isAddOp(look))
		emit("XOR AX, AX");
	else
		term();
	while(isAddOp(look))
	{
		emit("PUSH AX");
		switch(look)
		{
		case '+':
			add();
			break;
		case '-':
			subtract();
			break;
		default:
			expected("AddOp");
			break;
		}
	}
}

int main(int argc, char **argv)
{
	init();
	expression();

	return 0;
}
