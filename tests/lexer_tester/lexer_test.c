#include "../../includes/minishell.h"

//Compile with:  cc -g3 lexer_test.c ../../sources/lexer.c ../../sources/parser.c ../../libraries/libft/libft.a -o lexer
int main()
{
	t_dlist	*words = NULL;
	char	**test_scenarios = (char *[79]) {
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
			"    t 	 trailling    spaces  and tab    inside    string		",
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
			"!-2-3 --30-0=++33 *",
			".lscf ffd   ff  f",
			"    echo     Hello    ; ls  ",
			"ls ;",
			" $val ",
			"$valgrind",
			"echo '|' ois ",
			" echo ' $ois '  ",
			"	echo '$ola' ",
			"echo \"$ola\"",
			"echo $ola",
			"  [] <;  &&    ",
			"    65  worl d || >|   * test \\",
			"   	$ $ $ $ $   > < >> || <<     ''   ?   ? ** 4434   -00- _34331d 2342234  "" -ls-- || red | >> !  ",
			"sed 's/old/file/file.txt'",
			"  chmod  +x outfile   ",
			"touch infile",
			"cut -d ':' -f 1 /usr/local",
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
			"wc -w <test",
			"(grep 'error'     logfile || (cat error.log    |    sed 's/error/warning/g' >    error_warnings.log))   &&   (echo 'No errors found!'   >    status.log)   ||   echo   'Errors detected!'",
			"  (   echo \"testing  (  \"  )   ",
			"  (   echo 'testing  (  '  )   ",
			"  ( ls -la ) ",
			"  ) ls -la ( ",
			"((test))",
			"ls&",
			"    ls&  	  ",
			"ls &",
			"    ls &  	",
			"ls&uyffjfy",
			"ls&wc&cat",
			"ls &uyffjfy",
			"ls& uyffjfy",
			NULL
	};
	struct s_type_pattern {
		t_flag	flag;
		char	*pattern;
	};
	struct s_type_pattern	type_patterns[12] = {
			{WORD, "WORD"},
			{PIPE, "PIPE"},
			{GREATER, "GREATER"},
			{LESSER, "LESSER"},
			{D_GREATER, "D_GREATER"},
			{D_LESSER, "D_LESSER"},
			{D_PIPE, "D_PIPE"},
			{D_AND, "D_AND"},
			{L_PAR, "L_PAR"},
			{R_PAR, "R_PAR"},
			{END, "END"},
			{11, NULL}
	};

	for (int i = 1; *test_scenarios;)
	{
		ft_printf("Test %d: [%s]\n", i++, *test_scenarios);
		lexer(*test_scenarios, &words);
		while (words->next)
		{
			ft_printf("[%s] ", ((t_token *)words->content)->value);
			words = words->next;
		}
		ft_printf("[%s]\n", ((t_token *)words->content)->value);
		while (words->prev)
			words = words->prev;
		while (words->next)
		{
			ft_printf("%s, ", type_patterns[((t_token *)words->content)->flag].pattern);
			words = words->next;
		}
		ft_printf("%s\n\n", type_patterns[((t_token *)words->content)->flag].pattern);
		while (words->prev)
			words = words->prev;
		ft_dlstclear(&words, free, NULL);
		test_scenarios++;
	}
}
