
#include "minishell.h"

int main()
{
	t_command cmdone, cmdtwo;
	cmdone.prg_name = "/Users/afelger/Documents/minishell/tests/p1.py";
	cmdone.argv = malloc(3 * sizeof(char **));
	cmdone.argv[0] = "/Users/afelger/Documents/minishell/tests/p1.py";
	cmdone.argv[1] = "15";
	cmdone.argv[2] = NULL;
	cmdone.pid = 0;
	cmdone.ret_value = 0xFFFF;

	cmdtwo.prg_name = "/Users/afelger/Documents/minishell/tests/color.py";
	cmdtwo.argv = malloc(3 * sizeof(char **));
	cmdtwo.argv[0] = "/Users/afelger/Documents/minishell/tests/color.py";
	cmdtwo.argv[1] = "15";
	cmdtwo.argv[2] = NULL;
	cmdtwo.pid = 0;
	cmdtwo.ret_value = 0xFFFF;
	
	printf(" CMD Result: %d => Return Value is %d\n", run_single_pipe(&cmdone, &cmdtwo), cmdone.ret_value);
}