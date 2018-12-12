#pragma once
#include <sys/types.h> 
#include <stdio.h>
#include <string>

#ifdef __linux__
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#endif

class CUDPOperate
{
public:
    CUDPOperate(void);
    ~CUDPOperate(void);

    static void Initialize(std::string ip, int port);
    static bool SendTo(const char* buff, int len);

private:
#ifdef __linux__
    static struct sockaddr_in m_destAddr;
    static int m_sockfd;
#endif
    
};
