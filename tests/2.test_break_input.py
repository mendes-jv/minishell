import subprocess
import tempfile
import os
from time import sleep

def main():
	test_scenarios = [
	"\"$USER\"'$USER'",
	"  i Lov3 \"$user\"",
	"\"$USER\"",
	"$USER",
	"  ls   $ls",
	"l\"S go\"",
	"echo $?",
	"\"$ USER\"",
	"\" $USER \"",
	"\"$user\"",
	"echo $ PEDRO",
	"echo $",
	"$USER",
	"'$echo'",
	"\"Pe\"$A",
	"   heelloo  ",
	"echo \"'''  hello   world!  '''\"",
	"echo \"'''  \"Hello   world!\"  '''\"",
	"$test",
	"\"$echo\"",
	"   pedro$pedro   ",
	"   pedro $pedro   ",
	"  'pedro'$pedro   ",
	"  i Lov3 \"$user\"",
	"  i Lov3 \"$USER\"",
	" ' $SHELL  ' \"   $SHELL   \"    $SHELL   ",
	"Hello, world!",
	"echo  $minihell",
	"    testing 	 trailling    spaces  and tab    inside    string		",
	"testing type long long max: +9223372036854775807    ",
	"testing type long long min: -9223372036854775807    ",
	"testing quotes: '   hello!!  spaces need to     stay    '",
	"testing quotes   again: \"  pipemasters $ '  single quotes inside     doubles    ' \"",
	"testing quotes   again: \"  pipemasters $ ' $ single quotes inside     doubles    ' \"",
	"testing quotes   again: \"  pipemasters $ ' $ $loser single quotes inside     doubles    ' \"",
	"testing quotes   again: \"  pipemasters $ $loser ' $ $loser single quotes inside     doubles    ' \"",
	"	quotes one more    tim\"E\", and what about ' \"doubles          inside          \" ' singles? ",
	"	i hate quotes \"  but '  	lets    ' '    deal    ' \" with them \" ls :\"",
	"    last but not \" least '' or 	maybe   ''\"   	",
	"   maybe the \"last\"$ time 'last'$time",
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
	"   grep 'pattern' file.txt",
	"sed 's/old/file/file.txt'",
	"awk '{print $1}'file.txt",
	"  chmod  +x outfile   ",
	"touch infile",
	"cut -d ':' -f 1 /usr/local",
	"(grep 'error'     logfile || (cat error.log    |    sed 's/error/warning/g' >    error_warnings.log))   &&   (echo 'No errors found!'   >    status.log)   ||   echo   'Errors detected!'"
	]

	for script_content in test_scenarios:
		bash = script_content
		string = remove_extra_spaces(bash)
		env = subprocess.run('printenv', shell=True, capture_output=True, text=True)
		alias_expansion = execute_alias_expansion(string, env.stdout)
		alias_output = ''.join(alias_expansion)
		minishell = run_minishell_script(script_content)
		minishell_output = str(minishell.stdout)
		val = subprocess.run('cat val.txt | wc -l', shell=True, capture_output=True, text=True)
		test_outputs(alias_output, minishell_output, val, bash)
		sleep(0.6)

def	get_lenght_without_end_spaces_or_tabs(bash):
	i = 0
	lenght = len(bash)
	while i < lenght:
		if bash[lenght - 1] == " " or bash[lenght - 1] == "\t":
			lenght -= 1
		else:
			break
	return (lenght)

def remove_extra_spaces(bash):
	space = 1
	i = 0
	newstring = ""
	lenght = get_lenght_without_end_spaces_or_tabs(bash)
	while i < lenght:
		y = 0
		z = 0
		if bash[i] == "\"":
			x = i + 1
			while x < lenght:
				if bash[x] == "\"":
					space = 0
					break
				else:
					y += 1
					x += 1
			while z <= y + 1:
				newstring += bash[i]
				z += 1
				i += 1
			space = 0
			i -= 1
		elif bash[i] == "'":
			x = i + 1
			while x < lenght:
				if bash[x] == "'":
					space = 0
					break
				else:
					y += 1
					x += 1
			while z <= y + 1:
				newstring += bash[i]
				z += 1
				i += 1
			space = 0
			i -= 1
		elif bash[i] == " " or bash[i] == "\t":
			if space == 0:
				newstring += " "
				space = 1
		else:
			space = 0
			newstring += bash[i]
		i += 1
	return(newstring)

def	search_env(string, env, p, string_len):
	env_var = ""
	while p < string_len and string[p].isalnum() :
		env_var += string[p]
		p +=1
	if len(env_var) == 0:
		return("")
	env_var += "="
	start_index = env.find(env_var)
	if start_index != -1:
		end_index = env.find("\n", start_index)
		if end_index != -1:
			new_var = env[start_index + len(env_var):end_index]
			return(new_var)
	else:
		return("")

def execute_alias_expansion(string, env):
	string_len = len(string)
	result = []
	b = 0
	i = 0
	x = 0
	while i < string_len:
		temp = []
		y = i
		if string[i] == "\"":
			y += 1
			while y < string_len and string[y] != "\"":
				if string[y] == "$":
					if (string[y + 1].isalnum()) is False:
						b = 1
						break
					env_var = search_env(string, env, y + 1, string_len)
					if (len(env_var) != 0):
						if y - 1 > 0 and string[y - 1] != " ":
							temp.append(" ")
						temp.append(env_var)
					y += 1
					while y < string_len and string[y] != "\"":
						if (y < string_len and string[y].isalnum()) is False:
							temp.append(string[y])
						y += 1
						if (y < string_len and string[y] == "\""):
							b = 1
							break
				else:
					temp.append(string[y])
				y += 1
				if (b == 1):
					b = 0
					break
			if y < string_len and string[y] != "\"" and string[y] != "'":
				temp.append(string[y])
				y += 1
		elif string[i] == "'":
			if i - 1 > 0 and string[i - 1] != "\"":
				temp.append(" ")
			y += 1
			while y < string_len and string[y] != "'":
				temp.append(string[y])
				y += 1
			y += 1
		elif string[i] == " ":
			x = i
			while string[x] == " ":
				x +=1
			if string[x] != "$" and string[x] != "\"" and string[x] != "'":
				temp.append(" ")
			y += 1
		elif string[i] == "$":
			if i + 1 == string_len or string[i + 1].isalnum() is False:
				temp.append(" $")
				y += 1
			else:
				env = search_env(string, env, y + 1, string_len)
				if len(env) == 0:
					y += 1
				else:
					temp.append(env)
					y += len(env)
				while y < string_len and string[y].isalnum():
					y += 1
		else:
			while y < string_len and string[y] != " " and string[y] != "'" and string[y] != "\"" and string[y] != "$":
				temp.append(string[y])
				y += 1
		i = y
		if temp:
			result.append("".join(temp))
	return (result)

def run_minishell_script(script_content):
	with tempfile.NamedTemporaryFile('w', delete=False) as script_file:
		script_file.write(script_content)
	minishell_command = f'./test_input {script_file.name}'
	result = subprocess.run(minishell_command, input=script_content, shell=True, capture_output=True, text=True)
	with open('val.txt', 'w') as v:
		valgrind_command = ['valgrind', '-q', '--leak-check=full', '--show-leak-kinds=all', './test_input']
		valgrind_command.extend([script_file.name])
		subprocess.run(valgrind_command, input=script_content, text=True, stderr=v, stdout=subprocess.DEVNULL)
	os.unlink(script_file.name)
	return result

def test_outputs(tokens, minishell_output, val, bash):
	print_purple("TESTING INPUT: " + "[" + bash + "]")
	if tokens == minishell_output:
		print_green("INPUT CHECK [OK]")
	else:
		print_red("INPUT CHECK [KO]")
		print_red("EXPECTED: " + "[" + tokens + "]")
		print_red("OUTPUT: " + "[" + minishell_output + "]")
	if val.stdout.strip() == '0':
		print_green("MEM CHECK [MOK]" + "\n")
	else:
		print_red("MEM CHECK [MKO]" + "\n")

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

if __name__ == "__main__":
	main()
