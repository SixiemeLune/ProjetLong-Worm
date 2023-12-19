# Controlled Worm Exploitation Project "Projet Long Worm"

This project is designed as an end-of-study exploration into controlled worm exploitation. It includes intentional vulnerabilities for educational purposes.

## Table of Contents

- [Introduction](#introduction)
- [Code Components](#code-components)
    - [Server Vulnerability](#server-vulnerability)
    - [Exploitation Code](#exploitation-code)
- [Motivation](#motivation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This project explores controlled worm exploitation using intentional vulnerabilities implemented in specific pieces of code. It includes a server vulnerability designed to exhibit a buffer overflow and an exploitative code component that leverages this vulnerability. This project inteds to study the worm propagation inside a LAN. 

## Code Components

### Server Vulnerability

The server code (`server.c`) in this project intentionally contains a vulnerable function `overflow()` that demonstrates a buffer overflow:

```c
#include "server.h"

void overflow(char *str, int n) {
    char buffer[5];
    printf("get: %d\n", n);

    if (n <= 5) {
        strcpy(buffer, str); // Intentional buffer overflow
    } else {
        printf("Input size too large\n");
    }

    printf("Received: %s\n", buffer);
}
```

This function is deliberately vulnerable and susceptible to buffer overflow for educational purposes.
### Exploitation Code

The main project (`main.c`) contains an exploit code designed to utilize the intentional vulnerability in the server:

```c
#include "server.h"

// ... (other code sections)

void chat_client(int client_socket) {
    char buff[MAX_BUFFER_SIZE];

    for (;;) {
        bzero(buff, MAX_BUFFER_SIZE);
        int n = read(client_socket, buff, sizeof(buff));

        if (n <= 0) {
            break;
        }

        printf("From client: %s\n", buff);
        overflow(buff, n); // Exploiting the buffer overflow vulnerability
        write(client_socket, buff, strlen(buff) + 1);

        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// ... (other code sections)
```
This code segment demonstrates how the buffer overflow vulnerability in the server is exploited in the context of the project.

## Motivation

The inclusion of intentionally vulnerable code components serves an educational purpose within this project. These vulnerabilities are created and utilized under controlled conditions to provide insight into the risks associated with buffer overflow vulnerabilities and their potential exploitation by malicious entities. Please note that this project has been lead within a controlled environment and is **not designed to be a threat of any kind**, especially in regard of the current process memory security standards (ASLR, stack canaries..). The initial intent was to discover and aprehend the difficulties and challenges that represented crafting a self-replicating program through a LAN. The goal was to set foot into the offensive security world for 3 curious security students, with a topic that intrigued us. Be warned, this code is far from perfect !  




