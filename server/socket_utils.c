#include "socket_utils.h"
#include "global.h"


// 创建socket,并进行listen,bind 
// args: 端口号
// return: listenfd，标识已listen的socket
int ListenBind(int port)
{
    int listen_fd;
    struct sockaddr_in addr;
    //创建socket
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error socket(): %s(%d)\n", strerror(errno), errno);
		return 1;
	}

	//设置本机的ip和port
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
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
    // -1 意味着中间报错了

    return listen_fd;
}

int AcceptConnection(int listen_fd)
{
    int conn_fd;
    //等待client的连接 -- 阻塞函数
	if ((conn_fd = accept(listen_fd, NULL, NULL)) == -1) {
		printf("Error accept(): %s(%d)\n", strerror(errno), errno);
        return -1;
	}
    return conn_fd;
}


//https://cloud.tencent.com/developer/article/1177071
char* GetLocalIP(){
    int inet_sock;  
    struct ifreq ifr;  
	char ip[32]={'\0'};  
    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);  
    strcpy(ifr.ifr_name, "eth0");  
    ioctl(inet_sock, SIOCGIFADDR, &ifr);  
    strcpy(ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));  
    return ip;
}


//https://mp.weixin.qq.com/s?src=11&timestamp=1634096929&ver=3371&signature=SoOkuNuOvHwwYB2G7LjXQ6BjRypt-G1oQVev8YkJPA-ugkDm6iQUGpsRap3vr6o6rb7FQx6LxL3yZ1qZgzklltvo-N1z8rtpcw-4FZkgKYSMryxljJOopKQ*tSBslORG&new=1
// bind指定端口并非纯随机，且倾向于选择小端口，而作业要求20000-65536，则bind自动分配可用端口的方法很可能不work

// int GetAvailPort(int lowerbound,int upperbound)
// {
//     // 1. 创建一个socket
//     int sock = socket(AF_INET, SOCK_STREAM, 0);

//     // 2. 创建一个sockaddr，并将它的端口号设为0
//     struct sockaddr_in addr;
//     addr.sin_family = AF_INET;
//     inet_pton(AF_IENT,"0,0,0,0",&addr.sin_addr);
//     addr.sin_port = 0;        // 若port指定为0,则调用bind时，系统会为其指定一个可用的端口号
// 	int port;
// 	while(true)
// 	{
//     	// 3. 绑定
// 		int ret = bind(sock, (struct sockaddr*)&addr, sizeof addr);

// 		// 4. 利用getsockname获取
// 		struct sockaddr_in connAddr;
// 		int len = sizeof connAddr;
// 		ret = getsockname(sock, (struct sockaddr*)&connAddr, &len);
// 		port = ntohs(connAddr.sin_port); // 获取端口号
// 		if(port>=lowerbound && port<=upperbound)
// 		{	
// 			close(sock)
// 			return port;
// 		}
// 	}
// }

int CheckAvailPort(int port)
{
	struct sockaddr_in addr;
	int fd =socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET,INADDR_ANY,&addr.sin_addr);
	int ret = bind(fd,(struct sockaddr*)(&addr),sizeof(addr));
	close(fd);
	return ret;
}