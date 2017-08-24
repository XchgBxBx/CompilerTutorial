#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#define MAXVAR 26
int var[MAXVAR];


char look;


void init();
void initVar();

void nextChar();
void newLine();

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
void assignment();

void input();
void output();

void init()
{
	initVar();
	nextChar();
}

void initVar()
{
	int i;
	for(i = 0; i < MAXVAR; i++)
		var[i] = 0;
}

void nextChar()
{
	look = getchar();
}

void newLine()
{
	if(look == '\n')
		nextChar();
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
	else if(isalpha(look))
		val = var[getName() - 'A'];
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

void assignment()
{
	char name;
	
	name = getName();
	match('=');
	var[name -'A'] = expression();
}

void input()
{
	char name;
	char buffer[20];

	match('?');
	name = getName();
	printf("%c? ", name);
	fgets(buffer, 20, stdin);
	var[name - 'A'] = atoi(buffer);
}

void output()
{
	char name;

	match('!');
	name = getName();
	printf("%c -> %d\n", name, var[name - 'A']);
}

int main(int argc, char **argv)
{
	init();
	do
	{
		switch(look)
		{
		case '?':
			input();
			break;
		case '!':
			output();
			break;
		default:
			assignment();
			break;
		}
		newLine();
	}
	while(look != '.');

	return 0;
}
