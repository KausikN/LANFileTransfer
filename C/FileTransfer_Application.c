#include<stdio.h>

#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>


int ClientCreate(int port, int anyip, char IPADDR[])
{
	int c_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(port);
	if(anyip == 0) client.sin_addr.s_addr = inet_addr(IPADDR);
	else client.sin_addr.s_addr = INADDR_ANY;
	if(connect(c_socket, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		printf("Connection Issue.\n");
		return -777;
	}
	else return c_socket;
}


int ServerCreate(int port)
{
	int s_socket, s_server;
	s_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server, other;
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;


	if(bind(s_socket, (struct sockaddr*)&server, sizeof(server)) != -1)
	{
		printf("Waiting for Connection.....\n");
		listen(s_socket, 5);
		socklen_t add;
		add = sizeof(other);
		s_server = accept(s_socket, (struct sockaddr*)&other, &add);
		return s_server;
	}
	else
	{
		printf("\nInit issue.\n");
		close(s_server);
		close(s_socket);
		return -777;
	}
}

int main()
{
	printf("----------------------File Transfer TCP/IP Application-----------------------------\n");

	int c_socket_send, c_socket_recv;

	char charbuf[1];
	char filename[100];
	char filename_withext[100] = "";
	char ext[10];

	int exit = 0;

	int pid = fork();

	if(pid == 0)
	{
		// Child for receiving
		c_socket_recv = ClientCreate(9009, 1, "");
		if(c_socket_recv == -777)
		{
			printf("Failed init Client Receiver.\n");
			return 0;
		}

		while(exit == 0)
		{
			printf("You are now ready to receive files...\n");

			recv(c_socket_recv, filename, sizeof(filename), 0);
			recv(c_socket_recv, ext, sizeof(ext), 0);

			if(strlen(ext) == 0) return 0;

			char acknowlegement[2];

			int acceptfile = 0;
			printf("Do you want to accept %s.%s: ", filename, ext);
			scanf("%d", &acceptfile);

			if(acceptfile != 0)
			{
				printf("File %s.%s accepted for download.\n", filename, ext);

				strcpy(acknowlegement, "1");
				send(c_socket_recv, acknowlegement, sizeof(acknowlegement), 0);

				strcat(filename_withext, filename);
				strcat(filename_withext, "_recv.");
				strcat(filename_withext, ext);

				FILE *fp;
				fp = fopen(filename_withext, "w");

				char endoffile[1] = "0";

				int check = 0;
				while(check == 0)
				{
					recv(c_socket_recv, endoffile, sizeof(endoffile), 0);
					if(endoffile[0] == '1') check = 1;
					else
					{
						recv(c_socket_recv, charbuf, sizeof(charbuf), 0);
						//printf("buf: %s\n", charbuf);
						fprintf(fp, "%c", charbuf[0]);
					}
				}
				fclose(fp);

				printf("File %s Received Succesfully.\n", filename_withext);
			}
			else
			{
				printf("File %s.%s rejected for download.\n", filename, ext);

				strcpy(acknowlegement, "0");
				send(c_socket_recv, acknowlegement, sizeof(acknowlegement), 0);
			}
		}
		close(c_socket_recv);
		printf("You have stopped receiving files...\n");
	}
	else
	{
		// Parent for sending
		c_socket_send = ClientCreate(9009, 1, "");

		if(c_socket_send == -777)
		{
			printf("Failed init Client Sender.\n");
			return 0;
		}

		char filename[100];
		char filename_withext[100];
		char ext[10];

		char charbuf[1];

		while(exit == 0)
		{
			printf("Do you want to exit: ");
			scanf("%d", &exit);

			if(exit == 1) break;

			printf("Enter filename: ");
			scanf("%s", filename);

			printf("Enter extension: ");
			scanf("%s", ext);

			strcpy(filename_withext, filename);
			strcat(filename_withext, ".");
			strcat(filename_withext, ext);

			printf("Filename %s.\n", filename_withext);

			FILE *fp;
			fp = fopen(filename_withext, "r");

			printf("File %s opened.\n", filename_withext);

			char endoffile[1] = "0";

			send(c_socket_send, filename, sizeof(filename), 0);
			send(c_socket_send, ext, sizeof(ext), 0);

			char acknowlegement[2];
			recv(c_socket_send, acknowlegement, sizeof(acknowlegement), 0);

			if(acknowlegement[0] == '1')
			{
				while(!feof(fp))
				{
					fscanf(fp, "%c", &charbuf[0]);
					//printf("buf: %s\n", charbuf);
					send(c_socket_send, endoffile, sizeof(endoffile), 0);
					send(c_socket_send, charbuf, sizeof(charbuf), 0);

				}
				endoffile[0] = '1';
				send(c_socket_send, endoffile, sizeof(endoffile), 0);

				printf("File %s Sent Succesfully.\n", filename_withext);
			}
		}
		close(c_socket_send);

		printf("\n---------------------------------------------------------------------\n");
	}
	return 0;
}
