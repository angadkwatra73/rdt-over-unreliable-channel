//udp server - relaiable w packet loss

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 512
#define PORT 8882 

void die(char *s){
	perror(s);
	exit(1);
}

typedef struct packet1{
	int sq_no;
}ACK_PKT;

typedef struct packet2{
	int sq_no;
	char data[BUFLEN];
}DATA_PKT;

int main(void){

	struct sockaddr_in si_other,si_me;
	int s,i, slen=sizeof(si_other),recv_len;
	char buf[BUFLEN];
	DATA_PKT rcv_pkt;
	ACK_PKT ack_pkt;
	
	if( (s=socket (AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1){ die("socket()");}

 	memset( (char *) &si_me, 0,sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ( bind(s, (struct sockaddr *) &si_me, sizeof(si_me) ) == -1){ die("bind");}
	
	int state=0;

	while(1){
		switch(state){
			case 0 : 
				printf("Waiting for packet 0 form sender ... \n");
				fflush(stdout);
					if((recv_len = recvfrom(s, &rcv_pkt, BUFLEN , 0, (struct sockaddr *) &si_other, &slen) )== -1) {die("recvfrom()");}
		
					if (rcv_pkt.sq_no==0){
						printf("Packet recieved : seq no %d and Packet content is = %s \n", rcv_pkt.sq_no, rcv_pkt.data);
						ack_pkt.sq_no=0;
						if  (rand()%10<5) {
						
							if(sendto(s, &ack_pkt, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1) { die("sendto()");}
							state=1;
							break;
						}
						else{printf("Packed dropped \n");
						state=0;
						break;
				}
					}

				if (rcv_pkt.sq_no==1){
					printf("Packet Recieved with seqno %d , Resent ACK ",rcv_pkt.sq_no);
					ack_pkt.sq_no=1;
					if(sendto(s, &ack_pkt, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1) { die("sendto()");}
					state=0;
					break;
				
				}
			case 1 :
				printf("Waiting for packet 1 form sender ... \n");
				fflush(stdout);

				if((recv_len = recvfrom(s, &rcv_pkt, BUFLEN , 0, (struct sockaddr *) &si_other, &slen)) == -1) {	die("recvfrom()");}

				if (rcv_pkt.sq_no==1){
					printf("Packet recieved withseq no %d and Packet content is = %s \n", rcv_pkt.sq_no, rcv_pkt.data);
					ack_pkt.sq_no=1;
					if(sendto(s, &ack_pkt, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1) { die("sendto()");}
					state=0;
					break;
					}

				if (rcv_pkt.sq_no==0){
					printf("Packet Recieved with seqno %d , Resent ACK ",rcv_pkt.sq_no);
					ack_pkt.sq_no=0;
					if(sendto(s, &ack_pkt, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1) { die("sendto()");}
					state=1;
					break;
				}
		}
	}

	close(s);
	return 0;
}
			



























