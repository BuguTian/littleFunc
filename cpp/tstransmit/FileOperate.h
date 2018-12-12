#include <fcntl.h>
#include <stdlib.h>
#include <string>

#ifdef __linux__
#include <unistd.h>
#endif


class CFileOperate
{
public:
    CFileOperate(void);
    ~CFileOperate(void);

    static bool Initialize(std::string filepath);
    static int ReadFile(char* buff, int len);
	static int ResetFile();

private:
    static int m_fd;
};

