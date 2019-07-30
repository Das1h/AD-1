#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()
#include "MyDatabase.h"

#define QUEUELIMIT 5

const int MSG_HEADER_SIZE = sizeof(int);
const int MAX_MSG_BODY_SIZE = 1024;
const int MAX_MSG_SIZE = sizeof(int) + 1024;

// Big Endian based
int bytes_to_int(char* bytes)
{
	return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3] << 0);
}

// Big Endian based
void int_to_bytes(int num, char* bytes)
{
	bytes[0] = (num >> 24) & 0xff;
	bytes[1] = (num >> 16) & 0xff;
	bytes[2] = (num >> 8) & 0xff;
	bytes[3] = (num >> 0) & 0xff;
}

int send_all(int socket, char* buffer, size_t length)
{
	int send_len;
	char *p = buffer;
	while(length > 0)
	{
		if((send_len = send(socket, p, length, 0)) < 1)
		{
			return 0;
		}
		length -= send_len;
		p += send_len;
	}

	return 1;
}

int recv_all(int socket, char* buffer, size_t length)
{
	int recv_len;
	char *p = buffer;
	while(length > 0)
	{
		if((recv_len = recv(socket, p, length, 0)) < 1)
		{
			return 0;
		}
		length -= recv_len;
		p += recv_len;
	}

	return 1;
}

int send_msg(int socket, char* msg_body)
{
	char msg[MAX_MSG_SIZE];
	int msg_len = strlen(msg_body);

	int_to_bytes(msg_len, msg);
	memcpy(msg + MSG_HEADER_SIZE, msg_body, msg_len);

	return send_all(socket, msg, MSG_HEADER_SIZE + msg_len);
}

int recv_msg(int socket, char* msg)
{
	char msg_header[MSG_HEADER_SIZE];
	int msg_len;
	int result;

	recv_all(socket, msg_header, MSG_HEADER_SIZE);
	msg_len = bytes_to_int(msg_header);

	result = recv_all(socket, msg, msg_len);
	msg[msg_len] = '\0';

	return result;
}

int main(int argc, char* argv[])
{
	int serverSocket;
	int clientSocket;
	struct sockaddr_in serverSocketAddr;
	struct sockaddr_in clientSocketAddr;
	unsigned short serverPort;
	unsigned int clientLen;

	const int MAX_MSG_SIZE = 1024;
	char msg[MAX_MSG_SIZE];

	char* db_filename;
	MyDatabase* db;
	Data* data;

	char cmd[16];
	char op1[256], op2[256];

	if(argc != 3)
	{
		fprintf(stderr, "Invalid arguments.\n");
		fprintf(stderr, "Command: this_program port_number db_filename\n");
		fprintf(stderr, "Example: c_server 12345 test.db\n");
		exit(EXIT_FAILURE);
	}

	if((serverPort = (unsigned short)atoi(argv[1])) == 0)
	{
		fprintf(stderr, "Invalid port number.\n");
		exit(EXIT_FAILURE);
	}

	db_filename = argv[2];
	db = create_database();
	load_database(db, db_filename);

	if((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
	{
		perror("socket() failed.");
		exit(EXIT_FAILURE);
	}

	memset(&serverSocketAddr, 0, sizeof(serverSocketAddr));
	serverSocketAddr.sin_family = AF_INET;
	serverSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverSocketAddr.sin_port = htons(serverPort);

	if(bind(serverSocket, (struct sockaddr *)&serverSocketAddr, sizeof(serverSocketAddr)) < 0 )
	{
		perror("bind() failed.");
		exit(EXIT_FAILURE);
	}

	if(listen(serverSocket, QUEUELIMIT) < 0)
	{
		perror("listen() failed.");
		exit(EXIT_FAILURE);
	}

	clientLen = sizeof(clientSocketAddr);
	if((clientSocket = accept(serverSocket, (struct sockaddr *)&clientSocketAddr, &clientLen)) < 0)
	{
		perror("accept() failed.");
		exit(EXIT_FAILURE);
	}

	printf("connected from %s.\n", inet_ntoa(clientSocketAddr.sin_addr));

	while(recv_msg(clientSocket, msg))
	{
		printf("{\n    Query: \"%s\"\n", msg);

		sscanf(msg, "%s %s %s", cmd, op1, op2);

		if(strcmp(cmd, "get_all") == 0)
		{
			int cnt = 0;
			Data* result[1024];
			char msg_buff[1024];

			if(strcmp(op1, "key") == 0)
			{
				get_all_node(db->key_tree, result, &cnt);
			}
			else if(strcmp(op1, "value") == 0)
			{
				get_all_node(db->value_tree, result, &cnt);
			}
			else if(strcmp(op1, "time_stamp") == 0)
			{
				get_all_node(db->time_stamp_tree, result, &cnt);
			}

			sprintf(msg, "");
			for(int i = 0; i < cnt; ++i)
			{
				if(i == cnt - 1)
				{
					sprintf(msg_buff, "%s %s %s", result[i]->key, result[i]->value, result[i]->time_stamp);
				}
				else
				{
					sprintf(msg_buff, "%s %s %s\n", result[i]->key, result[i]->value, result[i]->time_stamp);
				}
				strcat(msg, msg_buff);
			}
		}
		else if((data = db_query(db, msg)) == NULL)
		{
			sprintf(msg, "NULL");
		}
		else
		{
			sprintf(msg, "%s %s %s", data->key, data->value, data->time_stamp);
		}

		save_database(db, db_filename);

		printf("    Result: \"%s\"\n}", msg);

		send_msg(clientSocket, msg);
		puts("");
	}

	puts("disconnected");

	close(clientSocket);
	close(serverSocket);

	return EXIT_SUCCESS;
}
