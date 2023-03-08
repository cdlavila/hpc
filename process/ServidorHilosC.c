//Servidor en C con hilos


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
#include <pthread.h>

int sockservidor;
char buffer[256];	
int n;
int sockcliente;


void *servidorchat(void *);


int main(int argc, char *argv[]){

	if(argc<2){
        	fprintf(stderr,"Error, puerto no especificado\n");
        	exit(0);
        }
	
	//Se abre el socket
	sockservidor=socket(AF_INET,SOCK_STREAM,0);
	if (sockservidor==-1){
	 	printf("Error, no se pudo abrir una conexion\n");
	}

	struct sockaddr_in direccion;

	direccion.sin_family=AF_INET;
	direccion.sin_port=htons(atoi(argv[1]));
	direccion.sin_addr.s_addr=INADDR_ANY;

	//Se ata el socket servidor al servicio que se va a prestar
	if (bind(sockservidor,(struct sockaddr *)&direccion,sizeof(direccion))==-1){
		printf("Error, no se pudo abrir el servicio\n");
		exit(0);
	} 

	//Avisamos al sistema que empiece a escuchar peticiones de clientes.
	if (listen(sockservidor,1)==-1){
		printf("Error\n");
		exit(0);
	}
	
	while(1){

		struct sockaddr cliente;
		
		int longcliente=sizeof(cliente);
	
		//Se empiezan a aceptar conexiones
		sockcliente=accept(sockservidor, &cliente, &longcliente);
		if(sockcliente==-1){
		    	printf("Error, no se pudo aceptar la petición\n");
		}

		pthread_t nuevothread;
		int hilo;
		hilo=pthread_create(&nuevothread,NULL,servidorchat,NULL);
		if(hilo!=0){
		    	printf("Error, no se pudo crear el hilo\n");
			exit(0);
		}
						
	}

	close(sockservidor);
	return 0; 
}

void *servidorchat(void *arg){

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
	
		
