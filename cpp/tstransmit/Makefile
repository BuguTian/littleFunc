CC=g++ -g -c

TsTransmit : main.o UDPOperate.o FileOperate.o
	g++ -o TsTransmit main.o UDPOperate.o FileOperate.o
main.o : main.cpp UDPOperate.h FileOperate.h
	$(CC) main.cpp     
UDPOperate.o : UDPOperate.h UDPOperate.cpp
	$(CC) UDPOperate.cpp
FileOperate.o : FileOperate.h FileOperate.cpp
	$(CC) FileOperate.cpp
clean : 
	rm -rf TsTransmit *.o
