CC = g++
CFLAGS = -Wall -Werror

all: loadbalancer

loadbalancer: main.o Requests.o Webserver.o LoadBalancer.o Firewall.o
	$(CC) $(CFLAGS) -o loadbalancer main.o Requests.o Webserver.o LoadBalancer.o Firewall.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Requests.o: Requests.cpp
	$(CC) $(CFLAGS) -c Requests.cpp

Webserver.o: Webserver.cpp
	$(CC) $(CFLAGS) -c Webserver.cpp

LoadBalancer.o: LoadBalancer.cpp
	$(CC) $(CFLAGS) -c LoadBalancer.cpp

Firewall.o: Firewall.cpp
	$(CC) $(CFLAGS) -c Firewall.cpp

clean:
	rm -f loadbalancer *.o