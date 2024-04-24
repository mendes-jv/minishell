import subprocess
import tempfile
import os
from time import sleep

def main():
	test_scenarios = [
		"ls      -la",
		"echo    Hello",
		"echo """"""""",
		"ls -la | echo Oi",
		"  ls  ",
		"pw0-d",
		"wc *ls    -la | ls -la >    infile"
	]

	for script_content in test_scenarios:
		bash = script_content
		bash_args_output = remove_extra_spaces(bash)
		minishell = run_minishell_script(script_content)
		minishell_output = str(minishell.stdout.strip())
		val = subprocess.run('cat val.txt | grep Memcheck | wc -l', shell=True, capture_output=True, text=True)
		test_outputs(bash_args_output, minishell_output, val, bash)
		sleep(0.8)


class Color:
	GREEN = '\033[92m'
	RED = '\033[91m'
	PURPLE = '\033[95m'
	RESET = '\033[0m'

def print_red(text):
	print(Color.RED + text + Color.RESET)

def print_green(text):
	print(Color.GREEN + text + Color.RESET)

def print_purple(text):
	print(Color.PURPLE + text + Color.RESET)

def run_minishell_script(script_content):
	with tempfile.NamedTemporaryFile('w', delete=False) as script_file:
		script_file.write(script_content)
	minishell_command = f'./minishell < {script_file.name}'
	result = subprocess.run(minishell_command, input=script_content, shell=True, capture_output=True, text=True)
	with open('val.txt', 'w') as v:
		valgrind_command = ['valgrind', '-q', './minishell']
		valgrind_command.extend(['<', script_file.name])
		subprocess.run(valgrind_command, input=script_content, text=True, stderr=v)
	os.unlink(script_file.name)
	return result

def remove_extra_spaces(bash):
	words = bash.split()
	bash_args_output = ' '.join(words)
	return bash_args_output

def test_outputs(bash_args_output, minishell_output, val, bash):
	print_purple("TESTING INPUT: " + "[" + bash + "]")
	if bash_args_output == minishell_output:
		print_green("INPUT CHECK [OK]" + "\n")
	else:
		print_red("INPUT CHECK [KO]")
		print_red("EXPECTED: " + "[" + bash_args_output + "]")
		print_red("OUTPUT: " + "[" + minishell_output + "]")
	if val.stdout.strip() == '0':
		print_green("MEM CHECK [MOK]" + "\n")
	else:
		print_red("MEM CHECK [MKO]" + "\n")


if __name__ == "__main__":
	main()
