#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>  
#include <stdlib.h>    
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const char *logpath = "/data/reader2.log";
char filename[64]={0};

#define kInt64_Max 0x7FFFFFFFFFFFFFFFLL
#define DEBUG_LOG(format, ...) do{ \
char msg[256]; \
FILE *fp = fopen(logpath, "a+"); \
sprintf(msg, format, ##__VA_ARGS__); \
fprintf(fp, "%s\n", msg); \
fflush(fp); \
fclose(fp); \
}while(0)

typedef unsigned int UInt32;
bool ifDeamon = true;

long long Milliseconds()
long long Microseconds();
void Sleep(UInt32 inMsec);
void Execute();
int Daemon();
void RunOnDaemon();

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage:./reader filename [-d]");
		return 0;
	}
	strcpy(filename, argv[1]);
    //初始化参数
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-d") == 0)
        {
            ifDeamon = false;
            i++;
        }
    }

    if (!ifDeamon)
    {
        Execute();
    }
    else
    {
        RunOnDaemon();
    }
    return 0;
}

void Execute()
{
   char buf[2048];
   int n;
   long long count=0,t1=0,t2=0;
   //int fd = open(argv[1],O_RDONLY);
   int fd = open(filename,O_RDWR);
   if(fd < 0)
   {
       printf("error!");
       return 0; 
   }
   t1 = Milliseconds();
   while(1)
   {
       n = read(fd, buf, 1024);
       if (n < 0) 
       {
          perror("read STDIN_FILENO");
          exit(1);
       }
       count = n + count;
       printf("%lld\n",count);
       if(n<=0){
          break;
       }
   }
   t2 = Milliseconds();
   FILE *fp = fopen(logpath, "a+");
   DEBUG_LOG("time=%lld %lld\n",t1,t2);
   long long df =  t2 - t1;
   DEBUG_LOG("%lld %lld %lf\n",df,count, 1.0* count*1000 /df);
}

long long Milliseconds()
{
    struct timeval t;
    struct timezone tz;
    gettimeofday(&t, &tz);
    return (t.tv_sec * 1000 + t.tv_usec / 1000);
}
long long Microseconds()
{
    struct timeval t;
    struct timezone tz;
    gettimeofday(&t, &tz);
    return (t.tv_sec * 1000*1000 + t.tv_usec);
}

void Sleep(UInt32 inMsec)
{
    struct timeval val;
    val.tv_sec = 0;
    val.tv_usec = inMsec*1000; //微秒
    select(0, NULL, NULL, NULL, &val);
}

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

