#include "UDPOperate.h"
#include <string.h>

#ifdef __linux__
struct sockaddr_in CUDPOperate::m_destAddr;
int CUDPOperate::m_sockfd = 0;
#endif

CUDPOperate::CUDPOperate(void)
{
}

CUDPOperate::~CUDPOperate(void)
{
}

void CUDPOperate::Initialize(std::string ip, int port)
{
#ifdef __linux__
    m_sockfd = ::socket(AF_INET,SOCK_DGRAM,0);
    bzero(&m_destAddr, sizeof(m_destAddr));
    m_destAddr.sin_family=AF_INET;
    m_destAddr.sin_port=htons(port);
    m_destAddr.sin_addr.s_addr=inet_addr(ip.c_str());
#endif  
}

bool CUDPOperate::SendTo(const char* buff, int len)
{
    int ret = 0;
#ifdef __linux__
    ret = sendto(m_sockfd, buff, len, 0, (struct sockaddr *)&m_destAddr, sizeof(m_destAddr));
#endif
    
    if (ret < 0)
    {
        perror ("write error!");
        return false;
    }
    return true;
}

