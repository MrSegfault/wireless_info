#include<unistd.h>
#include<ifaddrs.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<net/if.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<linux/wireless.h>
#include<string.h>



int main(int argc, char** argv){
	struct ifaddrs *w_iface, *i;
	int sockfd;
	
	if(getifaddrs(&w_iface)==-1){
		perror("ERROR BUSCANDO LA INTERFAZ WIRELESS");
		exit(-1);
	}
	
	for(i=w_iface; i!=NULL; i=i->ifa_next){
		if(i->ifa_addr==NULL || i->ifa_addr->sa_family!=AF_PACKET) continue;
		
		struct iwreq req;
		strncpy(req.ifr_name,i->ifa_name,IFNAMSIZ);
		

		if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        	perror("ERROR CREANDO SOCKET");
    	    exit(1);
    	}
		
		if(!ioctl(sockfd,SIOCGIWNAME,&req)){
			printf("\nNombre de la interfaz: %s\nProtocolo: %s\n", i->ifa_name,req.u.name);
			
			if(!ioctl(sockfd,SIOCGIWMODE,&req)){
				printf("Modo de operación: %d\n",req.u.mode);
			}else{
				perror("NO SE PUEDE DETERMINAR EL MODO DE OPERACIÓN");
			}
			if(!ioctl(sockfd,SIOCGIWAP,&req)){
				printf("Dirección HW del PA : %02x:%02x:%02x:%02x:%02x:%02x\n",
  					(unsigned char)req.u.ap_addr.sa_data[0],
  					(unsigned char)req.u.ap_addr.sa_data[1],
  					(unsigned char)req.u.ap_addr.sa_data[2],
  					(unsigned char)req.u.ap_addr.sa_data[3],
  					(unsigned char)req.u.ap_addr.sa_data[4],
  					(unsigned char)req.u.ap_addr.sa_data[5]);
			}else{
				perror("NO SE PUEDE DETERMINAR LA DIRECCIÓN HW DEL PA");
			}
			if(!ioctl(sockfd,SIOCGIWRATE,&req)){
				printf("Tasa de transmisión (bps): %d\n",req.u.bitrate.value);
			}else{
				perror("NO SE PUEDE DETERMINAR BITRATE");
			}
			if(!ioctl(sockfd,SIOCGIWTXPOW,&req)){
				printf("Potencia de transmisión: (bps): %d\n",req.u.txpower.value);
			}else{
				perror("NO SE PUEDE DETERMINAR LA POTENCIA");
			}
			if(!ioctl(sockfd,SIOCGIWSENS,&req)){
				printf("Sensibilidad: %d\n",req.u.sens.value);
			}else{
				perror("NO SE PUEDE DETERMINAR SENSIBILIDAD");
			}
			if(!ioctl(sockfd,SIOCGIWRETRY,&req)){
				printf("Parámetros de retransmisión: %d\n",req.u.retry.value);
			}else{
				perror("NO SE PUEDE DETERMINAR RETRANSMISIÓN");
			}
			if(!ioctl(sockfd,SIOCGIWFRAG,&req)){
				printf("Umbral de fragmentación: %d\n",req.u.frag.value);
			}else{
				perror("NO SE PUEDE DETERMINAR UMBRAL DE FRAGMENTACIÓN");
			}
			if(!ioctl(sockfd,SIOCGIWRTS,&req)){
				printf("Parámetros RTS: %d\n",req.u.rts.value);
			}else{
				perror("NO SE PUEDE DETERMINAR RTS");
			}
			if(!ioctl(sockfd,SIOCGIWFREQ,&req)){
				printf("Frecuencia/canal: mantisa -> %u exponente -> %u\n",req.u.freq.m,req.u.freq.e);
			}else{
				perror("NO SE PUEDE DETERMINAR FRECUENCIA/CANAL");
			}
			
			req.u.essid.pointer=(caddr_t*)malloc(IW_ESSID_MAX_SIZE*sizeof(char));
			req.u.essid.length=IW_ESSID_MAX_SIZE;
			req.u.essid.flags=0;
			if(!ioctl(sockfd,SIOCGIWESSID,&req)){
				printf("ESSID: %s\n",(char*)req.u.essid.pointer);
			}else{
				perror("NO SE PUEDE DETERMINAR SSID");
			}
			free(req.u.essid.pointer);
			
			printf("\n");



//EJERCICIO 2

			req.u.data.pointer=(char*)malloc(IW_SCAN_MAX_DATA*sizeof(char));
			req.u.data.length=IW_SCAN_MAX_DATA;
			req.u.data.flags=0;
			if(!ioctl(sockfd,SIOCGIWSCAN,&req)){
				int i;
				for(i=0;i<req.u.data.length;i++){
					printf("%c",((char*)req.u.data.pointer)[i]);
				}
			}else{
				perror("NO SE PUEDE ESCANEAR");
			}
			free(req.u.data.pointer);
			
			printf("\n");


		}
	}
	freeifaddrs(w_iface);
	
	return 0;
}
