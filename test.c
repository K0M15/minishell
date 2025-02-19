#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

// void keep_me(void *ptr)
// {
// 	static 
// }

int main()
{
	while (1)
	{
		printf("Ask me");
		char *test = readline("something: ");
		add_history(test);
		printf("\n>%s\n", test);
		// free(test);
	}
	return (0);
}