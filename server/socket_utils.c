#include "socket_utils.h"


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
	if (listen(listen_fd, 10) == -1) {
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
