#include <stdio.h>
#define ResetCin()	while(fgetc(stdin) != '\n'){} // 清空输入缓冲区

int dfa[4][4] = {
	{  1,  0, -1, -1 },
	{ -1,  3,  1,  2 },
	{  1, -1, -1, -1 },
	{ -1, -1, -1, -1 }
};

int sw(int); // 将 a,b,c,d 转 0,1,2,3
int deal(const char*); // DFA

int
main()
{
	int i;
	char s[1024];

	ungetc('\n', stdin);
	for (;;) {
		printf("Please enter a string: ");
		ResetCin();
		s[0] = 0;
		if ( scanf("%1000[^\n]", s) != 1 )
			continue;
		i = deal(s);
		putchar('\n');
		if ( i >= 0 ) {
			printf("ERROR: Check your input to use the correct syntax near \'%s\'\n\n", s+i);
			continue;
		}
		putchar('\n');
	}
}

int deal(const char *s)
{
	int i, state, c, c1;

	i = 0;
	state = 0;

	for (;;) {
		c = s[i];
		if ( ! c )
			return -1;
		c1 = sw(c);
		if ( c1 < 0 )
			return i;
		c1 = dfa[state][c1];
		if ( c1 < 0 )
			return i;
		state = c1;
		putchar(c);
		if ( state == 3 )
			putchar(' '), state = 0;
		++i;
	}
}

int
sw(int c)
{
	switch (c) {
	case 'a': return 0;
	case 'b': return 1;
	case 'c': return 2;
	case 'd': return 3;
	default:  return -1;
	}
}

