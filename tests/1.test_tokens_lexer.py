import subprocess
import tempfile
import os
from time import sleep

# Esse teste tem o intuito de verificar se o executável ./minishell está printando a string corretamente ao ser executado, comparando o resultado impresso em sua execução com o
# resultado obtido no teste. Além disso, vai ser verificado se houve leak de memória. Esse teste valida se o programa remove os espaços do início e do final da string, e mantém correto
# os valores que estão dentro de aspas simples ou duplas. Nesse teste ainda não foram feitas as etapas de quebrar o input em palavras e operadores, nem como expansão de alias, nem regras
# de quoting.

def main():
	test_scenarios = [
	"   grep 'pattern' file.txt",
	"awk '{print $1}'file.txt",
	"   maybe the \"last\"$ time 'last'$time",
	"testing quotes: '   hello!!  spaces need to     stay    '",
	"\"\" oi",
	"'' oi",
	"oi mundo",
	"echo \"hello\"   world",
	"echo $?",
	"'echo    $minishell   ' ",
	"Hello, world!",
	"Hey '$world' \"   $env\"  ",
	"echo  $minihell",
	"    t 	 trailling    spaces  and tab    inside    string		"
	"testing type long long max: +9223372036854775807    ",
	"testing type long long min: -9223372036854775807    ",
	"testing quotes: '   hello!!  spaces need to     stay    '",
	"testing quotes   again: \"  pipemasters $$ '  single quotes inside     doubles    ' \"",
	"	quotes one more    tim\"E\", and what about ' \"doubles          inside          \" ' singles? ",
	"	i hate quotes \"  but '  	lets    ' '    deal    ' \" with them \" ls :\"",
	"    last but not \" least '' or 	maybe   ''\"   	",
	"  ls -la |    echo Oi  ",
	"  ls  ",
	"   pw0-d-- 81 ",
	"  wc *ls    -la | ls -la >    infile  ",
	"   988948 9483492 834432   ",
	"!\-- -2-3 --30-0=++33 *",
	".lscf ffd   ff  f",
	"    echo     Hello    ; ls  ",
	"\\",
	"ls ;",
	" $val ",
	"$valgrind",
	"echo '|' ois ",
	" echo ' $ois '  ",
	"	echo '$ola' ",
	'echo "$ola"',
	"echo $ola",
	"  [] <;  &&    ",
	"    65  worl d || >|   * test \\",
	"   	$ $ $ $ $   > < >> || <<     ''   ?   ? ** 4434   -00- _34331d 2342234  "" -ls-- || red | >> !  ",
	"sed 's/old/file/file.txt'",
	"  chmod  +x outfile   ",
	"touch infile",
	"cut -d ':' -f 1 /usr/local",
	"(grep 'error'     logfile || (cat error.log    |    sed 's/error/warning/g' >    error_warnings.log))   &&   (echo 'No errors found!'   >    status.log)   ||   echo   'Errors detected!'"
		]

	for script_content in test_scenarios:
		bash = script_content
		bash_args_output = remove_extra_spaces(bash)
		minishell = run_minishell_script(script_content)
		minishell_output = list(minishell.stdout)
		val = subprocess.run('cat val.txt | wc -l', shell=True, capture_output=True, text=True)
		test_outputs(bash_args_output, minishell_output, val, bash)
		sleep(0.6)

class Color:
	GREEN = '\033[92m'
	RED = '\033[91m'
	PURPLE = '\033[95m'
	RESET = '\033[0m'

def print_red(text):
	print(Color.RED + text + Color.RESET, end="")

def print_green(text):
	print(Color.GREEN + text + Color.RESET)

def print_purple(text):
	print(Color.PURPLE + text + Color.RESET)

def	get_lenght_without_end_spaces_or_tabs(bash):
	i = 0
	lenght = len(bash)
	while (i < lenght):
		if bash[lenght - 1] == " " or bash[lenght - 1] == "\t":
			lenght -= 1
		else:
			break
	return lenght

def remove_extra_spaces(bash):
	i = 0
	newlist = []
	lenght = get_lenght_without_end_spaces_or_tabs(bash)
	while i < lenght:
		newstr = ""
		z = 0
		x = 0
		b = 0
		if bash[i] == " ":
			i += 1
		else:
			z = i
			while z < lenght and bash[z] != " " and bash[z] != "\t":
				if bash[z] == "\"":
					newstr += bash[z]
					z += 1
					x += 1
					if z < lenght and bash[z] == "\"":
						newstr += bash[z]
						x += 1
						break
					if b == 1:
						b = 0
						if z < lenght:
							newstr += bash[z]
						break
					while z < lenght and bash[z] != "\"":
						newstr += bash[z]
						z += 1
						x += 1
						b = 1
				elif bash[z] == "'":
					newstr += bash[z]
					z += 1
					x += 1
					if z < lenght and bash[z] == "'":
						newstr += bash[z]
						x += 1
						break
					if b == 1:
						b = 0
						if z < lenght:
							newstr += bash[z]
							break
					while z < lenght and bash[z] != "'":
						newstr += bash[z]
						z += 1
						x += 1
						b = 1
				else:
					newstr += bash[z]
					z += 1
					x += 1
			z += 1
			x += 1
			if newstr:
				newlist += [newstr]
		i += x
	return newlist

def run_minishell_script(script_content):
	with tempfile.NamedTemporaryFile('w', delete=False) as script_file:
		script_file.write(script_content)
	minishell_command = f'../../test_input {script_file.name}'
	result = subprocess.run(minishell_command, input=script_content, shell=True, capture_output=True, text=True)
	with open('val.txt', 'w') as v:
		valgrind_command = ['valgrind', '-q', '--leak-check=full', '--show-leak-kinds=all', './test_input']
		valgrind_command.extend([script_file.name])
		subprocess.run(valgrind_command, input=script_content, text=True, stderr=v, stdout=subprocess.DEVNULL)
	os.unlink(script_file.name)
	return result

def test_outputs(bash_args_output, minishell_output, val, bash):
	print_purple("TESTING INPUT: " + "[" + bash + "]" + "\n")
	if bash_args_output == minishell_output:
		print_green("TOKENS CHECK [OK]")
	else:
		print_red("TOKENS CHECK [KO]")
		print_red("\nEXPECTED TOKENS: ")
		print(', '.join(bash_args_output))
		print_red("ACTUAL TOKENS: ")
		print(minishell_output)
	if val.stdout.strip() == '0':
		print_green("\nMEM CHECK [MOK]" + "\n" + "\n" + "\n")
	else:
		print_red("\nMEM CHECK [MKO]" + "\n" + "\n" + "\n")

if __name__ == "__main__":
	main()
