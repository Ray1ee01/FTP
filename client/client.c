#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <errno.h>

#include <string.h>
#include <memory.h>
#include <stdio.h>

int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in addr;
	char sentence[8192];
	int len;
	int p;

	//创建socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//设置目标主机的ip和port
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6789);
	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {			//转换ip地址:点分十进制-->二进制
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}
	printf("about to connect\n");
	//连接上目标主机（将socket和目标主机连接）-- 阻塞函数
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printf("Error connect(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}
	printf("connect success\n");

	int listen_fd;
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//设置目标主机的ip和port
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(25600);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	//监听"0.0.0.0"
	//将本机的ip和port与socket绑定
	if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
		//开始监听socket
	if (listen(listen_fd, 128) == -1) {// Max Backlog
		printf("Error listen(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
	int state=0;
	int trans_fd;
	while(1)
	{	
		char response[256];
		memset(&response,0,sizeof(response));
		if(state==0)read(sockfd,response,255);
		if(state==1)
		{
			read(trans_fd,response,255);
			printf("recv %s\n",response);
			close(trans_fd);
			break;
		}
		//获取键盘输入
		if(state==0)
		{
			fgets(sentence, 4096, stdin);
			len = strlen(sentence);
			// sentence[len] = '\n';
			// sentence[len + 1] = '\0';
			printf("%s\n",sentence);
			//把键盘输入写入socket
			int n;
			n = write(sockfd,sentence,255);
			printf("write success\n");
			if(n<0)
			{
				printf("Error write(): %s(%d)\n", strerror(errno), errno);
				return 1;
			}
		}
		if (sentence[1]=='E')
		{
			printf("Get Port\n");
			if ((trans_fd = accept(listen_fd, NULL, NULL)) == -1) {
				printf("Error accept(): %s(%d)\n", strerror(errno), errno);
				return -1;
			}
			printf("Data connection build\n");
			state=1;
			memset(&sentence,0,sizeof(sentence));
		}

		// p = 0;
		// while (p < len) {
		// 	int n = write(sockfd, sentence + p, len + 1 - p);		//write函数不保证所有的数据写完，可能中途退出
		// 	if (n < 0) {
		// 		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		// 		return 1;
		// 	} else {
		// 		p += n;
		// 	}			
		// }
	}

	//榨干socket接收到的内容
	p = 0;
	while (1) {
		int n = read(sockfd, sentence + p, 8191 - p);
		if (n < 0) {
			printf("Error read(): %s(%d)\n", strerror(errno), errno);	//read不保证一次读完，可能中途退出
			return 1;
		} else if (n == 0) {
			break;
		} else {
			p += n;
			if (sentence[p - 1] == '\n') {
				break;
			}
		}
	}

	//注意：read并不会将字符串加上'\0'，需要手动添加
	sentence[p - 1] = '\0';

	printf("FROM SERVER: %s", sentence);

	close(sockfd);

	return 0;
}
