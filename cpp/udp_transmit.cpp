#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s port\n", argv[0]);
        exit(1);
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int sock;
    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
	
    //to stream
    struct sockaddr_in addr2;
    int sock2;
    if ( (sock2=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("socket");
        exit(1);
    }
    char arr[]="172.30.17.4";
    addr2.sin_family = AF_INET;
    addr2.sin_port = htons(1234);
    addr2.sin_addr.s_addr = inet_addr(arr);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock2);
        exit(1);
    }
	
    char buff[4096];
    struct sockaddr_in clientAddr;
    int n;
    unsigned int len = sizeof(clientAddr);
    while (1)
    {
        memset(buff,0,sizeof(buff));
        n = recvfrom(sock, buff, 4096, 0, (struct sockaddr*)&clientAddr, &len);
        //printf("n=%d\n",n);
        if (n>0)
        {
            n = sendto(sock2, buff, n, 0, (struct sockaddr *)&addr2, sizeof(addr2));
            if (n < 0)
            {
                perror("sendto");
                break;
            }
        }
        else
        {
            perror("recv");
            break;
        }
    }
    return 0;
}
