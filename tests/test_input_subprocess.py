import subprocess
import tempfile
import os
import re

class Color:
	GREEN = '\033[92m'
	RED = '\033[91m'
	RESET = '\033[0m'

def print_red(text):
	print(Color.RED + text + Color.RESET)

def print_green(text):
	print(Color.GREEN + text + Color.RESET)

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

def remove_extra_spaces(s):
	return re.sub(r'\s+', ' ', s)

def test_outputs(bash_args_output, minishell_output, val):
	if bash_args_output == minishell_output:
		print_green("INPUT CHECK [OK]")
	else:
		print_red("INPUT CHECK [KO]")
	if val.stdout.strip() == '0':
		print_green("MEM CHECK [OK]")
	else:
		print_red("MEM CHECK [MKO]")

def main():
	test_scenarios = [
		"ls -la",
		"echo Hello",
		"echo """"""""",
		"ls -la | echo Oi",
		"ls",
		"pwd"
	]

	for script_content in test_scenarios:
		bash = subprocess.run(script_content, text=True, shell=True, capture_output=True)
		bash_args_output = remove_extra_spaces(bash.args)
		minishell = run_minishell_script(script_content)
		minishell_output = str(minishell.stdout.strip())
		val = subprocess.run('cat val.txt | grep Memcheck | wc -l', shell=True, capture_output=True, text=True)
		test_outputs(bash_args_output, minishell_output, val)

if __name__ == "__main__":
	main()
