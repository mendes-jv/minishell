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
	subprocess.run('make', text=True, shell=True)
	minishell_command = f'./minishell < {script_file.name}'
	result = subprocess.run(minishell_command, shell=True, capture_output=True, text=True)
	with open('val.txt', 'w') as v:
		subprocess.run('valgrind -q minishell_command', shell=True, text=True, stderr=v)
	os.unlink(script_file.name)
	return result

def remove_extra_spaces(s):
	return re.sub(r'\s+', ' ', s)

def main():
	script_content = """
	wc -l < test.txt
	"""

	bash = subprocess.run('ls    -la  ', text=True, shell=True, capture_output=True)
	bash_args_normalized = remove_extra_spaces(bash.args)
	minishell = run_minishell_script(script_content)
	minishell_output = str(minishell.stdout.strip())
	val = subprocess.run('cat val.txt | grep Memcheck | wc -l', shell=True, capture_output=True, text=True)

	if bash_args_normalized == minishell_output:
		print_green("INPUT CHECK [OK]")
	else:
		print_red("INPUT CHECK [MKO]")

	if val.stdout.strip() == '0':
		print_green("MEM CHECK [OK]")
	else:
		print_red("MEM CHECK [MKO]")

if __name__ == "__main__":
	main()
