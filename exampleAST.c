#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int	expr(void);
int	term(void);
int	factor(void);



const char	*input;
int			lookahead;

void	nextToken(void)
{
	while (*input == ' ')
		input++; // Skip spaces
	lookahead = *input++;
}

// Function to parse a number
int	factor(void)
{
	int	value;

	if (isdigit(lookahead))
	{
		value = lookahead - '0';
		nextToken();
		return (value);
	}
	else if (lookahead == '(')
	{
		nextToken();
		int value = expr(); // Parse inner expression
		if (lookahead == ')')
			nextToken();
		return (value);
	}
	printf("Syntax error!\n");
	exit(1);
}

// Function to parse multiplication and division
int	term(void)
{
	int		value;
	char	op;
	int		right;

	value = factor();
	while (lookahead == '*' || lookahead == '/')
	{
		op = lookahead;
		nextToken();
		right = factor();
		if (op == '*')
			value *= right;
		else
			value /= right;
	}
	return (value);
}

// Function to parse addition and subtraction
int	expr(void)
{
	int		value;
	char	op;
	int		right;

	value = term();
	while (lookahead == '+' || lookahead == '-')
	{
		op = lookahead;
		nextToken();
		right = term();
		if (op == '+')
			value += right;
		else
			value -= right;
	}
	return (value);
}

int	main(void)
{
	int	result;

	input = "3 + 5 * (2 - 1)";
	nextToken();
	result = expr();
	printf("Result: %d\n", result);
	return (0);
}
