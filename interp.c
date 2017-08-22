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
int getNum();
void emit(char *fmt, ...);

int isAddOp(char c);
int isMulOp(char c);

int expression();
int term();
int factor();

void init()
{
	nextChar();
}

void nextChar()
{
	look = getchar();
}

int isAddOp(char c)
{
	return (c == '+' || 'c' == '-');
}


int isMulOp(char c)
{
	return (c == '*' || c == '/');
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

int getNum()
{
	int i;
	i = 0;

	if(!isdigit(look))
		expected("Integer");
	
	while(isdigit(look))
	{
		i *= 10;
		i += look - '0';
		nextChar();
	}

	return i;
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

int factor()
{
	int val;
	if(look == '(')
	{
		match('(');
		val = expression();
		match(')');
	}
	else
		val = getNum();

	return val;

}

int term()
{
	int val;

	val = factor();

	while(isMulOp(look))
	{
		switch(look)
		{
		case '*':
			match('*');
			val *= factor();
			break;
		case '/':
			match('/');
			val /= factor();
			break;
		}
	}

	return val;
}

int expression()
{
	int val;

	if(isAddOp(look))
		val = 0;
	else
		val = term();

	while(isAddOp(look))
	{
		switch(look)
		{
		case '+':
			match('+');
			val += term();
			break;
		case '-':
			match('-');
			val -= term();
			break;
		}
	}
	
	return val;
}

int main(int argc, char **argv)
{
	init();
	printf("%d\n", expression());
	return 0;
}
