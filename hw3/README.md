# Unix Programming HW3

## Compile and Build

Using the following command to build the library.

```
make
```

Clean the object files and library.

```
make clean
```

## Requirement

* [basic] Execute a single command.
* [basic] Properly block or unblock signals.
* [basic] Replace standard input/output of a process using the redirection operators (< and >).
* [basic] Setup foreground process group and background process groups.
* [basic] Create pipeline for commands separated by the pipe operator (|), and put the commands into the same process group.
* [optional] Manipulate environment variables: provide commands like export and unset to add and remove environment variables, respectively.
* [optional] Expand of the * and ? operators.
* [optional] Job control: support process suspension using Ctrl-Z, and jobs, fg and bg command.

## Done

* [basic] Execute a single command.
* [basic] Properly block or unblock signals.
* [basic] Replace standard input/output of a process using the redirection operators (< and >).
* [basic] Create pipeline for commands separated by the pipe operator (|), and put the commands into the same process group.
* [optional] Manipulate environment variables: provide commands like export and unset to add and remove environment variables, respectively.
* [optional] Expand of the * and ? operators.

## Execute single command

```
$ ls
```

## Properly block or unblock signals.

```
test by Ctrl-C will newline rather than termiate shell
```

## Redirection operators (< and >)

```
$ cat < /etc/passwd
$ cat > test
```

## Pipe

```
$ ls | cat | less
```

## Environment Variables

```
$ export ENV_VAR aaa bbb ccc # ENV_VAR="aaa bbb ccc"
```

```
$ unset ENV_VAR1 ENV_VAR2 # unset two varibales
```

## Expand * ?

```
$ ls *
```