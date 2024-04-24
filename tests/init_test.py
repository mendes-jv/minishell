import subprocess

class Color:
	GREEN = '\033[92m'
	RED = '\033[91m'
	RESET = '\033[0m'

def print_red(text):
	print(Color.RED + text + Color.RESET)

def print_green(text):
	print(Color.GREEN + text + Color.RESET)

subprocess.run('make', shell=True, text=True)

import subprocess
import tempfile
import os

def run_minishell_script(script_content):
	with tempfile.NamedTemporaryFile('w', delete=False) as script_file:
		script_file.write(script_content)
	minishell_command = f'./minishell < {script_file.name}'
	result = subprocess.run(minishell_command, shell=True, capture_output=True, text=True)
	os.unlink(script_file.name)
	return result

script_content = """
wc -l < test.txt
"""

subprocess.run('make', shell=True, text=True)

bash = subprocess.run('wc -l < test.txt', text=True, shell=True, capture_output=True)
minishell = run_minishell_script(script_content)

output = str(minishell.stdout.strip())

# outfile1 = subprocess.run('cat outfile1', shell=True, capture_output=True, text=True)
# outfile2 = subprocess.run('cat outfile2', shell=True, capture_output=True, text=True)

with open('val.txt', 'w') as v:
	subprocess.run('valgrind -q ./minishell -c "ls"', shell=True, text=True, stderr=v)

val = subprocess.run('cat val.txt | grep Memcheck | wc -l', shell=True, capture_output=True, text=True)

if bash.returncode == minishell.returncode:
	print_green("RETURN STATUS [OK]")
else:
	print_red("RETURN STATUS [KO]")

if bash.stdout == minishell.stdout:
	print_green("STDOUT [OK]")
else:
	print_red("STDOUT [KO]")

if bash.stderr == minishell.stderr:
	print_green("STDERROR [OK]")
else:
	print_red("STDERROR [KO]")

# if outfile1.stdout == outfile2.stdout:
# 	print_green("FILE CONTENT [OK]")
# else:
# 	print_red("FILE CONTENT [KO]")

if val.stdout.strip() == '0':
	print_green("MEM CHECK [OK].")
else:
	print_red("MEM CHECK [MKO].")
