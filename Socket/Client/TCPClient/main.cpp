
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define MAXLINE 4096

int main(int argc, char** argv)
{
    int    sockfd, n;
    char    recvline[4096], sendline[4096];
    struct sockaddr_in    servaddr;

    if( argc != 2){
        printf("usage: ./client <ipaddress>\n");
        exit(0);
    }

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    // 创建套接字描述符给sockfd
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    //结构体清零
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);//端口号
    if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
    //Linux下IP地址转换函数，可以在将IP地址在“点分十进制”和“整数”之间转换
        printf("inet_pton error for %s\n",argv[1]);
        exit(0);
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
    //连接请求
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
    }

    printf("send msg to server: \n");
    fgets(sendline, 4096, stdin);
    if( send(sockfd, sendline, strlen(sendline), 0) < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    close(sockfd);
    exit(0);
}
