//Servidor en C con procesos


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){

	if(argc<2){
        	fprintf(stderr,"Error, puerto no especificado\n");
        	exit(0);
        }
	
	int sockservidor;

	//Se abre el socket
	sockservidor=socket(AF_INET,SOCK_STREAM,0);
	if (sockservidor==-1){
	 	printf("Error, no se pudo abrir el servicio\n");
	}

	struct sockaddr_in direccion;

	direccion.sin_family=AF_INET;
	direccion.sin_port=htons(atoi(argv[1]));
	direccion.sin_addr.s_addr=INADDR_ANY;

	//Se ata el socket servidor al servicio que se va a prestar
	if (bind(sockservidor,(struct sockaddr *)&direccion,sizeof(direccion))==-1){
		printf("Error\n");
		exit(0);
	} 

	//Avisamos al sistema que empiece a escuchar peticiones de clientes.
	if (listen(sockservidor,1)==-1){
		printf("Error\n");
		exit(0);
	}

	char buffer[256];	
	int n;

	while(1){

		struct sockaddr cliente;

		int sockcliente;
		int longcliente=sizeof(cliente);
	
		//Se empiezan a aceptar conexiones
		sockcliente=accept(sockservidor, &cliente, &longcliente);
		if(sockcliente==-1){
		    	printf("Error, no se pudo aceptar la petición\n");
		}

		if(fork()==0){

			while(1){

				bzero(buffer,256);
		     		n=read(sockcliente,buffer,255);
				if(n<0){
					error("Error, no se pudo leer el mensaje");
				}
				if(buffer=="salir"){
					exit(0);
				}
				printf("%s",buffer);

				n=write(sockcliente,buffer,255);
				if(n<0){
					error("Error, no se pudo enviar la respuesta");
				}
			}
		}
		else{			
		}			
	}

	close(sockservidor);
	return 0; 
}


	
		
