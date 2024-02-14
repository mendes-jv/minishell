#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>

void handle_input(char *buffer);
int execute_command(char *cmd);

void draw_intro() {

}

int main() {
	char	*input;
	draw_intro();
	input = readline("ZapShell> ");
	while (input)
	{
		input = readline("ZapShell> ");
	}
	return  0;
}
