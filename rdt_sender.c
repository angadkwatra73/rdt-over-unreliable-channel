//simple udp - stop and wait, packet loss 

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>

#define BUFLEN 512
#define PORT 8882

typedef struct packet1{
	int sq_no;
} ACK_PKT;

typedef struct packet2{
	int sq_no;
	char data[BUFLEN];
} DATA_PKT;

void die(char *s){
	perror(s);
	exit(1);
}

int main (void){
	
	struct sockaddr_in si_other;
	int s,i,slen=sizeof(si_other);
	
	char buf[BUFLEN];
	char message[BUFLEN];

	DATA_PKT send_pkt;
	ACK_PKT rcv_ack;

	if( (s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1) { die("socket"); }
	memset((char *) &si_other, 0,sizeof(si_other));
	si_other.sin_family=AF_INET;
	si_other.sin_port=htons(PORT);
	si_other.sin_addr.s_addr=inet_addr("127.0.0.1");

	int state=0;
	while(1){

		fd_set readfds;
        	FD_ZERO(&readfds);
        	FD_SET(s, &readfds);

       	 	struct timeval tv;
        	tv.tv_sec = 5;      
        	tv.tv_usec = 0;

		switch(state){

			case 0: 
				printf("Enter Message 0: ");
				fgets(send_pkt.data,sizeof(send_pkt),stdin);
				send_pkt.sq_no=0;
				if ( sendto(s, &send_pkt, sizeof(send_pkt), 0,(struct sockaddr *) &si_other,slen)== -1){ die("sendto()");}
				state=1;
				break;
			case 1:
				int activity= select( s+1,&readfds,NULL, NULL, &tv);
				if ( activity== -1) { die("select");}
				else if (activity == 0 ) {
				       	sendto(s, &send_pkt, sizeof(send_pkt), 0,(struct sockaddr *) &si_other,slen); 
					printf("Timeout: Resent \n");	
					break;}
				printf(" FD_ISSET: %d \n",FD_ISSET(s,&readfds));
				if (FD_ISSET(s,&readfds)){
					if ( recvfrom(s, &rcv_ack, sizeof(rcv_ack), 0, (struct sockaddr *) &si_other,  &slen)== -1 ){ die("recvfrom()") ;}
					if ( rcv_ack.sq_no==0){
						printf("Recieved ack [%d] \n",rcv_ack.sq_no);
						state=2;
						break;
						}
					if ( rcv_ack.sq_no==1){
						printf("Recieved ack [%d] \n",rcv_ack.sq_no);
						state=1;
						break;
						}
				}
			case 2:
				printf("Enter Message 1: ");
				fgets(send_pkt.data,sizeof(send_pkt),stdin);
				send_pkt.sq_no=1;
				if ( sendto(s, &send_pkt, sizeof(send_pkt), 0,(struct sockaddr *) &si_other,slen)== -1){ die("sendto()");}
				state=3;
				break;
			case 3:
				if ( recvfrom(s, &rcv_ack, sizeof(rcv_ack), 0, (struct sockaddr *) &si_other,  &slen)== -1 ){ die("recvfrom()") ;}
				if ( rcv_ack.sq_no==1){
					printf("Recieved ack [%d] \n",rcv_ack.sq_no);
					state=0;
					break;
					}
				if ( rcv_ack.sq_no==0){
					printf("Recieved ack [%d] \n",rcv_ack.sq_no);
					break;
					}

				
				}
			}

	close(s);
	return 0;
	
	}












