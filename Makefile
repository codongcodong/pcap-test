all: pcap-test

pcap-test: main.o printInfo.o
	g++ -o pcap-test main.o printInfo.o -lpcap

main.o: main.cpp printInfo.h
	g++ -c -o main.o main.cpp 

printInfo.o: printInfo.cpp printInfo.h
	g++ -c -o printInfo.o printInfo.cpp 

clean:
	rm -f pcap-test *.o
