tcpclient: tcpclient.o
	gcc -Wall tcpclient.o -o tcpclient
tcpclient.o: tcpclient.c
	gcc -Wall -c tcpclient.c
clean:
	@rm -rf *.o
cleanall:
	@rm -rf *.o tcpclient
