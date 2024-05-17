import subprocess
import tempfile
import os
from time import sleep

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
	"cat LICENSE|grep Copy",
	"ls>test",
	"ls > test",
	"ls> test",
	"ls >test",
	"wc -w < test",
	"wc -w<test",
	"wc -w< test",
	"wc -w <test"
	]

	for script_content in test_scenarios:
		bash = script_content
		tags = get_tags(index)
		tokens = tokenizer(index)
		index += 1
		minishell = run_minishell_script(script_content)
		minishell_output = str(minishell.stdout)
		test_outputs(tags, tokens, minishell_output, bash, index)
		sleep(0.6)
	return

def get_tags(index):
	with open('tests/temp_files/flags_model.txt', 'r') as t:
		for l, line in enumerate(t):
			if l == index:
				temp = line.replace("\n", "", 1)
				return temp

def tokenizer(index):
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
	os.unlink(script_file.name)
	return result

def test_outputs(tags, tokens, minishell_output, bash, index):
	print("\n")
	print_yellow("----------Testing Tags: ")
	print(index, end=' ')
	print_yellow("----------")
	print("\n")
	print_purple("INPUT: " + "[" + bash + "]" + "\n")
	if tags == minishell_output:
		print_green("TAGS CHECK [OK]")
		print("ACTUAL TAGS: ", end ='')
		print(tags)
	else:
		print_red("TAGS CHECK [KO]")
		print("\nEXPECTED TAGS: ", end='')
		print(tags, end='')
		print("\nACTUAL TAGS: ", end ='')
		print(minishell_output)
		print("TOKENS: ", end ='')
		print(tokens)

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
