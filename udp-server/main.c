#include "server.h"


int main(){
	struct sockaddr_in addr ={
		.sin_family=AF_INET,
		.sin_port=htons(PORT),
	};
	int fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if((bind(fd,(struct sockaddr *)&addr,sizeof(addr)))==-1){
		perror("bind");
		exit(EXIT_FAILURE);
	}

init_lockers();

	int ret;
	char buf[BUFSIZ]={};
	struct sockaddr_in clinet_addr;
	while(1){
		memset(buf,0,sizeof(buf));
		socklen_t len=sizeof(clinet_addr);
		ret=recvfrom(fd,buf,BUFSIZ-1, 0,  (struct sockaddr *)&clinet_addr,&len);
		if(ret<0){
			perror("recvfrom");
			exit(EXIT_FAILURE);
		}
		if(ret==0){
			printf("收到的数据数据错误\n");
			continue;
		}
		buf[ret]='\0';
		printf("recv data:%s\n",buf);
		switch(buf[0]){
		case 'l':
			ret=do_login(&buf[1]);
			break;
		case 'r':
			ret=do_register(&buf[1]);
			break;
		case 'p':
			ret=do_pickup(&buf[1]);
			break;
		case 'd':
			ret=do_deliver(&buf[1]);
			break;
		default :
			fprintf(stderr,"不存在的消息类型:%s\n",buf);
		}
		sendto(fd,buf,strlen(&buf[1])+2,0,(struct sockaddr *)&clinet_addr,len);
	}
	return 0;
} 
