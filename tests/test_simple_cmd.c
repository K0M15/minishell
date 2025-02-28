
#include "minishell.h"

int main()
{

	t_command *cmd;

	cmd = create_simple_command();
	cmd->args[0] = "/bin/ls";
	ms_env_init();
	execute_simple_command(cmd, get_appstate()->enviroment);
}