
#include "minishell.h"

int main()
{
	printf("Return Value: %d\n", ms_heredoc("EOF", 1, NULL, 0));
}