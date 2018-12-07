# littleFunc
使用Linux服务端通常有各种工具进行统计。但是通常有以下问题:  
1.有的工具移植到arm平台很复杂;  
2.有的脚本重新编写调试很麻烦;  
3.写其他脚本或测试程序时需要借鉴此以往的思路  
   
#shell脚本  
cpurate.sh //统计一次cpu信息  
memrate.sh //统计一次内存信息   
network.sh //统计一次网卡信息   
dstat.sh   //每隔1s统计一次指定的网卡的流量,使用方式为"sh dstat.sh ethX"   
clrotfile.sh //循环清理过期文件的脚本   
   
#python脚本   
tcpclient.py   
tcpserver.py   
udpclient.py   
udpserver.py   
   
#C/C++测试程序(linux下)   
nodetree.c    //构造用户目录树，当时用于测试moosefs在不经过fuse无读写的情况下,mfsmaster所能达到的最大并发，经测试约为6万   
readerd.cpp   //在后台运行并统计读写指定文件的耗时(当时用于测试arm平台下moosefs客户端的读写速度)   
tcp_client.cpp  //tcp客户端示例代码  
tcp_server.cpp  //tcp服务端epoll示例代码  
udp_client.cpp  //udp客户端示例代码  
udp_server.cpp  //udp服务端示例代码  
udp_transmit.cpp //udp转发示例代码  
inotify.cpp      //检测目录内文件是否发生读写等事件  
