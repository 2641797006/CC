#include <stdio.h>
#include <ctype.h>
#include "string"

typedef struct{
	string left;
	string right;
}rule;

#define _24k_list_t rule
#include "list.h"

int dfa[][5] = { // [6][5]
// space a-zA-Z '-' '>' '$'
	{ 0,  1, -1, -1, -1}, // 0
	{ 2,  1,  3, -1, -1}, // 1
	{ 2, -1,  3, -1, -1}, // 2
	{-1, -1, -1,  4, -1}, // 3
	{ 4,  5, -1, -1,  5}, // 4
	{ 0,  5, -1, -1,  5}  // 5
};
int final_state = 5;

char GS[] =
" S -> AB \n"
" S -> bC \n"
" A -> $  \n"
" A -> b  \n"
" B -> $  \n"
" B -> aD \n"
" C -> AD \n"
" C -> b  \n"
" D -> aS \n"
" D -> c  \n";

int sw(int);
int get_rule(const char*, rule_list*);

int
main()
{
	int i;
	rule_list L;
	rule_list_iterator it;

	rule_list_init(&L);
	i = get_rule(GS, &L);
	if ( i >= 0 ) {
		printf("ERROR: Check your input to use the correct syntax near \'%s\'\n\n", GS+i);
		return i;
	}

	it = rule_list_first(&L);
	while ( it != rule_list_tail(&L) ) {
		printf("%s -> %s\n", string_c_str(&it->left), string_c_str(&it->right));
		it = rule_list_next(it);
	}

	rule_list_destroy(&L);
}

int
init_rule(rule *r)
{
	return string_init( & r->left ) && string_init( & r->right );
}

void
destroy_rule(rule *r)
{
	string_destroy( & r->left );
	string_destroy( & r->right );
}

int
get_rule(const char *s, rule_list *list)
{
	int i, state, c, c1;
	rule r;

	i = 0;
	state = 0;
	init_rule(&r);

	for (;;) {
		c = s[i];
		if ( ! c ) {
			if ( state == final_state ) {
				rule_list_push_back(list, &r);
				return -1;
			}
			destroy_rule(&r);
			return state ? i : -1;
		}
		c1 = sw(c);
		if ( c1 < 0 )
			break;
		c1 = dfa[state][c1];
		if ( c1 < 0 )
			break;
		if (state == final_state && c1 != state ) {
			rule_list_push_back(list, &r);
			init_rule(&r);
		}
		state = c1;
		switch (state) {
		case 1:
			string_push_back(&r.left, c);
			break;
		case 5:
			string_push_back(&r.right, c);
			break;
		default:;
		}
		++i;
	}
	destroy_rule(&r);
	return i;
}

int
sw(int c)
{
	if ( isspace(c) )
		return 0;
	if ( isalpha(c) )
		return 1;
	if ( c == '-' )
		return 2;
	if ( c == '>' )
		return 3;
	if ( c == '$' )
		return 4;
	return -1;
}

