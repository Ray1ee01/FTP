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

	//����socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//����Ŀ��������ip��port
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6789);
	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {			//ת��ip��ַ:���ʮ����-->������
		printf("Error inet_pton(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}
	printf("about to connect\n");
	//������Ŀ����������socket��Ŀ���������ӣ�-- ��������
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

	//����Ŀ��������ip��port
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(25600);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	//����"0.0.0.0"
	//��������ip��port��socket��
	if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		printf("Error bind(): %s(%d)\n", strerror(errno), errno);
		return -1;
	}
		//��ʼ����socket
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
		//��ȡ��������
		if(state==0)
		{
			fgets(sentence, 4096, stdin);
			len = strlen(sentence);
			// sentence[len] = '\n';
			// sentence[len + 1] = '\0';
			printf("%s\n",sentence);
			//�Ѽ�������д��socket
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
		// 	int n = write(sockfd, sentence + p, len + 1 - p);		//write��������֤���е�����д�꣬������;�˳�
		// 	if (n < 0) {
		// 		printf("Error write(): %s(%d)\n", strerror(errno), errno);
		// 		return 1;
		// 	} else {
		// 		p += n;
		// 	}			
		// }
	}

	//ե��socket���յ�������
	p = 0;
	while (1) {
		int n = read(sockfd, sentence + p, 8191 - p);
		if (n < 0) {
			printf("Error read(): %s(%d)\n", strerror(errno), errno);	//read����֤һ�ζ��꣬������;�˳�
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

	//ע�⣺read�����Ὣ�ַ�������'\0'����Ҫ�ֶ����
	sentence[p - 1] = '\0';

	printf("FROM SERVER: %s", sentence);

	close(sockfd);

	return 0;
}
