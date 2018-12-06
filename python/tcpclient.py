import socket

#############---config---###################
SERVER_IP = '172.30.13.252'
PORT = 32180

############################################
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(('%s' % SERVER_IP,PORT))
s.send(b'GET /handle?IP=172.30.13.172:23358&userCode=12346 HTTP/1.1\r\nHost: 172.30.14.240:32171\r\nConnection: keep-alive\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.122 Safari/537.36 SE 2.X MetaSr 1.0\r\nAccept-Encoding: gzip,deflate,sdch\r\nAccept-Language: zh-CN,zh;q=0.8\r\n\r\n')
buffer = []
while True:
    d = s.recv(1024)
    if d:
        print(d.decode('utf-8'))
        print("\n")
        s.send(b'KEEPLIVE.........');
    else:
        break
s.close()

