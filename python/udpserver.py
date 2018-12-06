import os
import socket
import sys

#############---config---###################
PORT = 12170

############################################

filename = './log/udpserver.log'
f = open(filename, 'a')
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('0.0.0.0', PORT))
print('Bind UDP on %s...' % PORT)
count = 0
while True:
    data, addr = s.recvfrom(1024)
    #f.write(data)
    print('Received: %s' % data, addr)
    s.sendto(b'Hello!', addr)
    count = count + 1
    print(count)
s.close()
f.close()
