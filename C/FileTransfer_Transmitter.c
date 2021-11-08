#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int c_socket;

int ClientCreate(int port, int anyip, char IPADDR[])		// Return 1 for error
{
	c_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(port);
	if(anyip == 0) client.sin_addr.s_addr = inet_addr(IPADDR);
	else client.sin_addr.s_addr = INADDR_ANY;
	if(connect(c_socket, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		printf("Connection Issue.\n");
		return 1;
	}
	else return 0;
}

int main()
{
	printf("----------------------File Transfer TCP/IP Transmitter-----------------------------\n");

	int error = ClientCreate(9009, 1, "");
	if(error == 1)
	{
		close(c_socket);
		printf("Connection Issue.\n");
		return 0;
	}
	else 
	{
		char filename[100];
		char filename_withext[100];
		char ext[10];

		char buffer[2000];
		char charbuf[1];

		long print_interval = 0;
		printf("Enter progress printing interval: \n");
		scanf("%ld", &print_interval);


		printf("Enter filename: ");
		scanf("%s", filename);

		printf("Enter extension: ");
		scanf("%s", ext);

		strcpy(filename_withext, filename);
		strcat(filename_withext, ".");
		strcat(filename_withext, ext);

		printf("Filename %s.\n", filename_withext);

		long file_size = 0;

		FILE *fp_size;
		fp_size = fopen(filename_withext, "r");

		fseek(fp_size, 0L, SEEK_END);
		file_size = ftell(fp_size);

		fclose(fp_size);

		FILE *fp;
		fp = fopen(filename_withext, "r");

		printf("File %s opened.\n", filename_withext);

		char endoffile[1] = "0";

		send(c_socket, filename, sizeof(filename), 0);
		send(c_socket, ext, sizeof(ext), 0);

		printf("\nStarted sending data...\n");
		int index = 1;
		while(!feof(fp))
		{ 
			/*
			fscanf(fp, "%s", buffer);

			//printf("buf: %s\n", buffer);

			send(c_socket, buffer, sizeof(buffer), 0);
			*/

			fscanf(fp, "%c", &charbuf[0]);
			//printf("buf: %s\n", charbuf);
			send(c_socket, endoffile, sizeof(endoffile), 0);
			send(c_socket, charbuf, sizeof(charbuf), 0);

			if(index % print_interval == 0) printf("%d / %ld\n", index, file_size);
			index++;
		}
		/*
		strcpy(buffer, "end");
		send(c_socket, buffer, sizeof(buffer), 0);
		*/
		/*
		charbuf[0] = '\b';
		send(c_socket, charbuf, sizeof(charbuf), 0);
		*/
		printf("\n\n File Ended \n\n");
		endoffile[0] = '1';
		send(c_socket, endoffile, sizeof(endoffile), 0);
		
		printf("buffer Sent.\n");
		fclose(fp);
	}

	close(c_socket);

	printf("\n---------------------------------------------------------------------\n");	
	return 0;
}

