#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


int main()
{
	char buffer[100];
	int s_socket, s_server;
	char buf[100];
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
	fp=fopen("gotText.txt","w");

	while(1)
	{
		read(s_server, buffer, sizeof(buffer));
		if(strcmp(buffer,"close")==0)
			break;
		else
			fprintf(fp, "%s\n", buffer);
	}

	printf("File sent Sucessfully !!! \n");
	printf("Data Copied in gotText.txt file !!! \n");

	fclose(fp);
	close(s_server);
	close (s_socket);

	return 0;
}