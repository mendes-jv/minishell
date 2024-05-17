import subprocess
import tempfile
import os
from time import sleep

# Esse teste tem o intuito de verificar se o executável ./minishell está printando a string corretamente ao ser executado, comparando o resultado impresso em sua execução com o
# resultado obtido no teste. Além disso, vai ser verificado se houve leak de memória. Esse teste valida se o programa remove os espaços do início e do final da string, e mantém correto
# os valores que estão dentro de aspas simples ou duplas. Nesse teste ainda não foram feitas as etapas de quebrar o input em palavras e operadores, nem como expansão de alias, nem regras
# de quoting.

def main():
	index = 0
	test_scenarios = [
	"   'gre'pattern file.txt",
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
	"(grep 'error'     logfile || (cat error.log    |    sed 's/error/warning/g' >    error_warnings.log))   &&   (echo 'No errors found!'   >    status.log)   ||   echo   'Errors detected!'",
	"cat banana && cat abacaxi",
	"cat banana &&cat abacaxi",
	"cat banana&&cat abacaxi",
	"cat banana&& cat abacaxi",
	"echo test || echo test",
	"echo test ||echo test",
	"echo test||echo test",
	"echo test|| echo test",
	"cat LICENSE | grep Copy",
	"cat LICENSE |grep Copy",
	"cat LICENSE| grep Copy",
	"cat LICENSE|grep Copy"
	]

	for script_content in test_scenarios:
		bash = script_content
		tokens = tokenizer(index)
		index += 1
		minishell = run_minishell_script(script_content)
		minishell_output = str(minishell.stdout)
		val = subprocess.run('cat tests/temp_files/val.txt | wc -l', shell=True, capture_output=True, text=True)
		test_outputs(tokens, minishell_output, val, bash, index)
		sleep(0.6)
	subprocess.run('rm tests/temp_files/val.txt', shell=True)
	return

def tokenizer(index):
	i = 0
	with open('tests/temp_files/lexer_model.txt', 'r') as t:
		for l, line in enumerate(t):
			if l == index:
				temp = line.replace("\n", "", 1)
				return temp

def run_minishell_script(script_content):
	with tempfile.NamedTemporaryFile('w', delete=False) as script_file:
		script_file.write(script_content)
	minishell_command = f'./minishell {script_file.name}'
	result = subprocess.run(minishell_command, input=script_content, shell=True, capture_output=True, text=True)
	with open('tests/temp_files/val.txt', 'w') as v:
		valgrind_command = ['valgrind', '-q', '--leak-check=full', '--show-leak-kinds=all', './minishell']
		valgrind_command.extend([script_file.name])
		subprocess.run(valgrind_command, input=script_content, text=True, stderr=v, stdout=subprocess.DEVNULL)
	os.unlink(script_file.name)
	return result

def test_outputs(tokens, minishell_output, val, bash, index):
	print("\n")
	print_yellow("----------Test ")
	print(index, end=' ')
	print_yellow("----------")
	print("\n")
	print_purple("INPUT: " + "[" + bash + "]" + "\n")
	if tokens == minishell_output:
		print_green("TOKENS CHECK [OK]")
		print("ACTUAL TOKENS: ", end ='')
		print(tokens)
	else:
		print_red("TOKENS CHECK [KO]")
		print("\nEXPECTED TOKENS: ", end='')
		print(tokens, end='')
		print("\nACTUAL TOKENS: ", end ='')
		print(minishell_output)
	if val.stdout.strip() == '0':
		print_green("\nMEM CHECK [MOK]")
	else:
		print_red("\nMEM CHECK [MKO]")
		print("\nVALGRIND ERROR:")
		subprocess.run('cat tests/temp_files/val.txt', shell=True)

class Color:
	GREEN = '\033[92m'
	RED = '\033[91m'
	PURPLE = '\033[95m'
	YELLOW = '\033[93m'
	RESET = '\033[0m'

def print_red(text):
	print(Color.RED + text + Color.RESET, end="")

def print_green(text):
	print(Color.GREEN + text + Color.RESET)

def print_purple(text):
	print(Color.PURPLE + text + Color.RESET)

def print_yellow(text):
	print(Color.YELLOW + text + Color.RESET, end='')

if __name__ == "__main__":
	main()
