# Server Scanner and Exploit

This program is designed to scan a range of IP addresses for servers listening on port 8080 and exploit a vulnerability on those servers.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Usage](#usage)
- [Structure](#structure)


## Introduction

This C program uses socket programming to scan a specified range of IP addresses for servers listening on port 8080. It creates a linked list of available servers and attempts to exploit a vulnerability on those servers by establishing a connection and executing an exploit code.

## Features

- **Server Scanner**: Scans IP addresses within a specified range to detect servers listening on port 8080.
- **Exploit Functionality**: Attempts to exploit a vulnerability on detected servers.
- **Linked List Implementation**: Utilizes a linked list data structure to store available servers.

## Usage

To use this program:

1. Compile the code using a C compiler (e.g., gcc).
2. Execute the compiled binary, which will scan the specified IP range and attempt to exploit vulnerable servers.
3. Review the console output for a list of servers found and exploited.

**Note**: Ensure compliance with legal and ethical standards when scanning and exploiting servers. Unauthorized access to systems is illegal and unethical.

## Structure

- `struct cell_server`: Represents an individual server cell in the linked list.
- `struct list_server`: Defines the server list structure.
- `Init_List()`: Initializes the list structure.
- `Add_Beginning()`: Adds a server to the beginning of the list.
- `print_list()`: Prints the contents of the server list.
- `free_list()`: Frees memory occupied by the server list.
- `is_infected()`: Checks if a server is infected.
- `infect()`: Functionality for infecting servers (exploit code not detailed).
- `scan_server_available()`: Scans for available servers in a given IP range.
- `exploit()`: Executes an exploit on a vulnerable server.
- `entry_point()`: Main function where scanning and exploiting occur.
- `main()`: Program entry point.

