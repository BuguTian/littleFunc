#include <stdio.h>
#include <string.h>
#include <map>
#include <string>
#include "UDPOperate.h"
#include "FileOperate.h"

#ifdef __linux__
    #include <sys/time.h>
#endif

#define kInt64_Max 0x7FFFFFFFFFFFFFFFLL
typedef long long Int64;
typedef unsigned int UInt32;

std::map<std::string, std::string> mapArgs;
int port;
double bitrate;
int m_pidChild = 0;

Int64 Milliseconds();
void Sleep(UInt32 inMsec);
void Execute();

#ifdef __linux__

int Daemon();
void RunOnDaemon();

#endif


int main(int argc, char* argv[])
{
    //初始化参数
    if (argc != 9 && argc != 10)
    {
        printf("Usage:\n\t-i ip\n\t-p port\n\t-f filepath\n\t-b bitrate\n");
        return 0;
    }   

    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-d") == 0)
        {
            mapArgs["-d"] = std::string("yes");
            i++;
        }
        else
        {
            std::string key = std::string(argv[i++]);
            mapArgs[key] = std::string(argv[i]);
            i++;
        } 
    }

    sscanf(mapArgs["-p"].c_str(), "%d", &port);
    sscanf(mapArgs["-b"].c_str(), "%lf", &bitrate);
    printf("Finish Parse Param\n");

    //初始化UDP
    CUDPOperate::Initialize(mapArgs["-i"], port);
    printf("Finish UDP Initialize %s\n", mapArgs["-i"].c_str());

    //初始化FileRead
    CFileOperate::Initialize(mapArgs["-f"]);
    printf("Finish File Initialize %s\n", mapArgs["-f"].c_str());

    if (mapArgs["-d"] == std::string("yes"))
    {
        Execute();
    }
    else
    {
#ifdef __linux__
               RunOnDaemon();
#endif
    }
    return 0;
}

void Execute()
{
    //主要工作
    char buff[1316];
    int len = 1316;
    int ret = 1;
    Int64 cachetime, sttime, totalSize = 0, cacheTotalSize;

    //设置时间戳
    printf("Go to 0 float count %d\n", ret);
    sttime = Milliseconds();
    printf("Go to 0.1 float count %lld\n", sttime);

    while (1)
    {
        //读取文件
        memset (buff, 0, len);
        ret = CFileOperate::ReadFile(buff, len);
        if (ret == 0)
        {
            CFileOperate::ResetFile();
        }else if (ret < 0)
        {
            printf("ret < 0 ,ret is %d\n", ret);
            break;
        }else
        {
            totalSize += ret;
			//发送消息
			CUDPOperate::SendTo (buff, len);

			//保证码率，延时等待
			Int64 waittime = (Int64)(1.0*totalSize*8*1000/(bitrate*1024*1024) - (Milliseconds() - sttime));
			printf("Waittime is %lld\n", waittime);
			if (waittime > 0)
			{
				Sleep(waittime);
			}
        }

        //为了保证循环播放，当读到文件尾后，重新计算大小和时间
        if (ret == 0)
        {
            totalSize = totalSize - cacheTotalSize;
            sttime = cachetime;
        }
        printf("Go to next loop! TotalSize is %lld\n", totalSize);

        //防止循环播放期间造成巨大抖动
        cacheTotalSize = totalSize;
        cachetime = Milliseconds();

    }
}

Int64 Milliseconds()
{
#ifdef __linux__
    struct timeval t;
    struct timezone tz;
    int theErr = ::gettimeofday(&t, &tz);
    if (theErr != 0)
    {
        printf("theErr is not zero!\n");
    }

    Int64 curTime;
    curTime = t.tv_sec;
    curTime *= 1000;				// sec -> msec
    curTime += t.tv_usec / 1000;	// usec -> msec
    return curTime;
#else
    return 0;
#endif
}

void Sleep(UInt32 inMsec)
{
#ifndef __linux__
    ::Sleep(inMsec);

#else
    struct timeval val;
    val.tv_sec = 0;
    val.tv_usec = inMsec*1000; //微秒
    select(0, NULL, NULL, NULL, &val);
#endif
}

#ifdef __linux__

int Daemon()
{


    int pid = -1;

    if ((pid=fork()))
        exit(0);
    else if (pid < 0)
        exit(1);

    setsid();

    if ((pid=fork()))
        exit(0);
    else if (pid < 0)
        exit(1);

    int fd = open("/dev/null", O_RDWR, 0);
    if (fd != -1) 
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > 2)
            close(fd);
    }

    return 0;
}
void RunOnDaemon()
{
    Daemon();
    pid_t pid = fork();
    if (pid == 0)
    {
        Execute();
        exit(1);
    }
    else if (pid < 0)
    {
        printf("fork() child process failed!");
    }
    else
    {
        //父进程需要进行的操作
        Sleep(5000);
        exit(1);
    }
}

#endif
