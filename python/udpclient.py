import socket
import time

#############---config---###################
YFP_IP = '172.30.13.252'
PORT = 12170 

############################################

count = 0
sttime = time.time()
while True:
    data = b'POST http://www.example.com HTTP/1.1Content-Type: application/x-www-form-urlencoded;charset=utf-8keyvalue=21&functionkey=0&userCode=12346&YNBModIP=172.30.14.240:23358&cougou8'
    s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    s.sendto(data, ('%s' % YFP_IP, PORT))
    count = count + 1
    if(count > 10):
        break
    
endtime = time.time()
difftime = endtime - sttime
print "sttime:%f,\nendtime:%f,\ndifftime%f\n" % (sttime,endtime,difftime)
s.close()

