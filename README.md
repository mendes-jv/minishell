<div align="center">

# Minishell

**As beautiful as a shell**

<img src="https://github.com/pedromelocf/utilities/blob/master/minishellm.png" />

Minishell is a project aimed at creating a simple shell, similar to Bash. This project will enhance your understanding of processes and file descriptors in Unix-like operating systems. 

</div>

## How to use it
```bash
git clone https://github.com/mendes-jv/minishell zapshell && cd zapshell && make && ./minishell
```

## Contents

- [I. Introduction](#i-introduction)
- [II. Mandatory Part](#ii-mandatory-part)
- [III. Bonus Part](#iii-bonus-part)

## I. Introduction
The existence of shells is fundamental to the field of computing. They provide a way for users to interact with computers through command lines rather than low-level hardware manipulations. Minishell allows you to step back in time and tackle the challenges faced before modern GUI environments, providing a hands-on experience with shell programming.

## II. Mandatory Part

- **Program Name**: `minishell`
- **Turn In Files**: `Makefile`, `*.h`, `*.c`
- **Arguments**: 
  - Use external functions like `readline`, `printf`, `malloc`, etc.
- **Libft Authorized**: Yes
- **Description**: Implement a shell that:
  - Displays a prompt when waiting for a command.
  - Maintains a command history.
  - Searches and executes commands based on the `PATH` variable or relative/absolute paths.
  - Avoids using more than one global variable for signal handling.
  - Implements quote handling for both single (`'`) and double (`"`) quotes.
  - Implements input/output redirection and piping.
  - Expands environment variables and the exit status of the last command.
  - Handles control signals (`ctrl-C`, `ctrl-D`, `ctrl-\`) appropriately.
    
- **Builtins to Implement**:
  - `echo` (with `-n`)
  - `cd` (with paths)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

## III. Bonus Part

To receive bonus credit, your implementation must include:
- Logical operators (`&&`, `||`) with proper priority handling.
- Support for wildcard (`*`) expansion in the current directory.

*Bonus evaluation is contingent on the mandatory part being perfect.*

<div align="center">

<h2> Grade </h2>

<img src="https://github.com/pedromelocf/utilities/blob/master/102-grade.png" />

</div>
