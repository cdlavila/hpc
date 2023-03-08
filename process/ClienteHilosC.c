//Cliente en C con hilos


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){

	if(argc<2){
	       fprintf(stderr,"Error, puerto no especificado\n");
	       exit(0);
    	}

	int sockcliente;

	//Se abre el socket
	sockcliente=socket(AF_INET,SOCK_STREAM,0);
	if (sockcliente == -1){
	    printf("Error, no se pudo abrir el servicio\n");
	}

	struct hostent *host;

	//Se obtiene la ip del servidor
	host=gethostbyname("localhost");
	if (host==NULL){
		printf("Error, no existe tal servidor\n");
	}

	struct sockaddr_in direccion;
	direccion.sin_family=AF_INET;
	direccion.sin_addr.s_addr=((struct in_addr *)(host->h_addr))->s_addr;
	direccion.sin_port = htons(atoi(argv[1]));

	//Se abre la conexión con el servidor
	if(connect(sockcliente,(struct sockaddr *)&direccion,sizeof(direccion))==-1){
	    printf("No se pudo establecer conexion con el servidor\n");
	    exit(0);
	}

	char buffer[256];
	int n;

	while(1){
		
		bzero(buffer,256);
		printf("Digite el mensaje: ");
	        fgets(buffer,255,stdin);

	        n=write(sockcliente,buffer,strlen(buffer));
	        if(n<0){ 
			error("Error, no se pudo enviar el mensaje");
		}
		if(buffer=="salir"){
			exit(0);
		}

	    	bzero(buffer,256);
	    	n=read(sockcliente,buffer,255);
	    	if(n<0){ 
			error("Error, no se pudo leer la respuesta");
		}
	    	printf("%s",buffer);	
	}

	close(sockcliente);
	return 0;
}

	
 

	
