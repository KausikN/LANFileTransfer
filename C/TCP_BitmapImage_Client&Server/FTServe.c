#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


int main()
{
	char buffer;
	int check;
	int s_socket, s_server;
	s_socket= socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server, other;
	memset(&server, 0 , sizeof(server));
	memset(&other, 0 , sizeof(other));

	server.sin_family=AF_INET;
	server.sin_port=htons(9009);
	server.sin_addr.s_addr=INADDR_ANY;

	if(bind(s_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		printf("Bind Error !!!\n");
		exit(1);
	}

	listen(s_socket, 5);

	socklen_t len;
	len=sizeof(other);	
	s_server=accept(s_socket, (struct sockaddr *)&other, &len);
	
	FILE *fp;
	fp=fopen("avengers_assemble.bmp","w");

	while(1)
	{
		read(s_server, &check, sizeof(check));
		read(s_server, &buffer, sizeof(buffer));
		if(check==0)
			break;
		else 
			fputc(buffer,fp);
	}

	printf("Picture sent Sucessfully !!! \n");
	printf("Data Copied in avengers_assemble.bmp file !!! \n");

	fclose(fp);
	close(s_server);
	close (s_socket);

	return 0;
}