#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


int main()
{
	int c_socket;
	char buffer;
	int check;
	int EndIndicator=0;
	c_socket= socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in client;
	memset(&client, 0 , sizeof(client));
	client.sin_family=AF_INET;
	client.sin_port=htons(9009);
	client.sin_addr.s_addr=INADDR_ANY;
	//client.sin_addr.s_addr = inet_addr("172.16.17.77");

	if(connect(c_socket, (struct sockaddr *)&client, sizeof(client))==-1)
	{
		printf("Connection Issue\n");
		exit(1);
	}

	FILE *fp;
	fp=fopen("avengers_dissamble.bmp","r");

	while(!feof(fp))
	{
		buffer=fgetc(fp);
		check=1;
		write(c_socket,&check,sizeof(check));
		write(c_socket,&buffer,sizeof(buffer));	
	}

	check=0;
	write(c_socket,&check,sizeof(check));


	printf("Picture sent sucessfully !!! \n");
	fclose(fp);
	close(c_socket);
	return 0;
}
