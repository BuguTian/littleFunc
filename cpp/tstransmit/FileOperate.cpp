#include "FileOperate.h"

int CFileOperate::m_fd = 0;

CFileOperate::CFileOperate(void)
{
}


CFileOperate::~CFileOperate(void)
{
#ifdef __linux__
    ::close (m_fd);
#endif
}

bool CFileOperate::Initialize(std::string filepath)
{
#ifdef __linux__
    m_fd = ::open(filepath.c_str(), O_RDONLY|O_NONBLOCK);
#endif
}

int CFileOperate::ReadFile(char* buff, int len)
{
    int n = -1;
#ifdef __linux__
    n = ::read(m_fd, buff, len);
#endif
    return n;
}

int CFileOperate::ResetFile()
{
	int ret = 0;
	if (m_fd == 0){
		ret = -1;
	}else{
		ret = ::lseek(m_fd,0,SEEK_SET);
	}
	return ret;
}
