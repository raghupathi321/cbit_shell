# CBIT SHELL 

CBIT Shell is a custom-built command-line interpreter designed to execute shell commands efficiently. This project provides a basic implementation of a shell, supporting common operations, user-defined commands, and basic scripting. The shell is built to be lightweight and adaptable, providing a simple interface for executing commands and scripts on Linux/Unix-based systems.

## Features

- **Command Execution**: Supports basic shell commands like `ls`, `cd`, `pwd`, etc.
- **I/O Redirection**: Implements input/output redirection using symbols like `>` and `<`.
- **Piping**: Supports piping with `|` to connect multiple commands.
- **Signal Handling**: Graceful handling of signals like `SIGINT` (Ctrl+C) to prevent shell termination.
- **Error Handling**: Provides detailed error messages for invalid commands or failed execution.
- **Built-in Commands**: Includes commands like `exit`, `help`, and `clear` for user interaction.
- **History Support**: Tracks the history of commands executed during the session.

## Prerequisites

To run CBIT Shell, you'll need:

- **Operating System**: Linux/Unix
- **Compiler**: GCC (GNU Compiler Collection)
- **Make**: To compile using the provided Makefile (optional)

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/username/cbit-shell.git
    ```

2. Navigate to the project directory:
    ```bash
    cd cbit-shell
    ```

3. Compile the source code:
    ```bash
    make
    ```

4. Run the shell:
    ```bash
    ./cbit_shell
    ```

## Usage

### Basic Commands

You can execute any shell command as follows:

```bash
$cbit ls
$cbit pwd
$cbit cd /path/to/directory
