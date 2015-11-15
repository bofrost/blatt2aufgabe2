/**
 * author: simon
 */

#ifndef SERVER
#define SERVER

#include "message.hpp"
#include "counter.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> 
#include <list>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
// #include <iostream>
// #include <functional>
// #include <atomic>

class Server {
public:
	/**
	 * Konstruktor
	 * @param port Portnummer für den Server
	 * @param nextPort Portnummer für den Server an welchen Token weitergeleitet wird
	 */
	Server(const std::string& port, const std::string& nextPort, Counter* c) : 
		_port(port),
		_nextPort(nextPort),
		_c(c) {}
	
	/**
	 * Destruktor
	 */
	~Server() {
		delete _c;
		_c = NULL;
	}

	/**
	 * startet Server in eigenem thread
	 */
	void start() {
		loop();
		return;
	}

	void startSynchronized(unsigned int tickLength, unsigned int startTime) {
		_tick = 0;

		std::thread t(&Server::synchronizedLoop, this, tickLength, startTime);
		t.join();
		return;
	}

private:
	std::string _port;
	std::string _nextPort;
	// Queue mit den Messages
	std::list<Message> buffer_messageQueue;
	// Anzahl der Tokens, welche der Server besitzt
	unsigned int _numberOfToken;
	Counter* _c;
	unsigned int _tick;

	/**
	 * Schleife, welche Verbindungen und Nachrichten annimmt
	 * @param port Portnummer für den Server
	 */
	void loop() {
		int sockfd, newsockfd, portno;
		socklen_t len;
		char buffer[256];
		struct sockaddr_in serv_addr, cli_addr;

		int n;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
		   error("ERROR opening socket");
		
		bzero((char *) &serv_addr, sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(atoi(_port.c_str()));
		if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
			error("ERROR on binding");
		
		if (listen(sockfd,5) < 0) {
			close(sockfd);
			error("ERROR on listen");
		}

		while(buffer[0] != 'q') {
			len = sizeof(cli_addr);

			newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &len);
			if (newsockfd < 0) {
				close(sockfd);
			    error("ERROR on accept");
			}
			bzero(buffer,256);
			n = read(newsockfd,buffer,255);
			if (n < 0) {
				close(newsockfd);
				close(sockfd);
				error("ERROR reading from socket");
			}
			handleMessage(buffer);
			// _messageQueue.push_back(Message(c->getValue(), buffer, (struct sockaddr*) &cli_addr));
			
			// TODO: Thread asynchron starten, whatever
			// std::thread t(&Server::handleMessage, this, buffer); t.join();
			close(newsockfd);
		}
		close(sockfd);
		return; 
	}

	/**
	 * Schleife welche in unterschiedlichen ticks Verbindungen annimmt.
	 * @param tickLength Länge eines Ticks in ms.
	 * @param startTime Startzeit, bei der Ticks angefangen werden zu zählen.
	 */
	void synchronizedLoop(unsigned int tickLength, unsigned int startTime) {
		int sockfd, newsockfd, portno;
		socklen_t len;
		char buffer[256];
		struct sockaddr_in serv_addr, cli_addr;

		int n;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
		   error("ERROR opening socket");
		
		bzero((char *) &serv_addr, sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(atoi(_port.c_str()));
		if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
			error("ERROR on binding");
		
		if (listen(sockfd,5) < 0) {
			close(sockfd);
			error("ERROR on listen");
		}

		while(buffer[0] != 'q') {
			// TODO:
			// if (getTime() > startTime + _tick * tickLength) {
			// 	++_tick;
			// }

			len = sizeof(cli_addr);

			newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &len);
			if (newsockfd < 0) {
				close(sockfd);
			    error("ERROR on accept");
			}
			bzero(buffer,256);
			n = read(newsockfd,buffer,255);
			if (n < 0) {
				close(newsockfd);
				close(sockfd);
				error("ERROR reading from socket");
			}
			handleMessage(buffer);
			// _messageQueue.push_back(Message(c->getValue(), buffer, (struct sockaddr*) &cli_addr));
			
			// TODO: Thread asynchron starten, whatever
			// std::thread t(&Server::handleMessage, this, buffer); t.join();
			close(newsockfd);
		}
		close(sockfd);
		return;
	}

	/**
	 * Behandlung der Nachrichten
	 * Zur Zeit nur in die standart ausgabe schreiben
	 * @param buffer Puffer mit Nachricht
	 */
	void handleMessage(char buffer[]) {
		std::cout << _port << ": " << buffer;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		sendMessage(Message(0, buffer));

		return;
	}

	/**
	 * Funktion welche als Thread alle Messages aus der Warteschlange abarbeitet.
	 */
	void handleMessages() {
		// TODO:
		// while(1) {
		// 	Message m = _messageQueue.pop_front();
		// 	handleMessage(m.getMessage);
		// }
		return;
	}

	int sendMessage(Message m) {
		int sockfd, portno, n;
	    struct sockaddr_in serv_addr;
	    struct hostent *server;

	    char buffer[256];
	    strncpy(buffer, m.getMessage().c_str(), std::min(m.getMessageSize(), 256));

	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0) {
	        error("ERROR opening socket");
	        exit(-1);
	    }
	    server = gethostbyname("localhost");

	    if (server == NULL) {
	        fprintf(stderr,"ERROR, no such host\n");
	        return(0);
	    }

	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char *)server->h_addr, 
	         (char *)&serv_addr.sin_addr.s_addr,
	         server->h_length);
	    serv_addr.sin_port = htons(atoi(_nextPort.c_str()));
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
	        error("ERROR connecting");
	    }
	    
	    n = write(sockfd,buffer,strlen(buffer));
	    if (n < 0) {
	    	close(sockfd);
	        error("ERROR writing to socket");
	    }
	    close(sockfd);
	    return 0;
	}

	/**
	 * Schreibt Error message und schließt Programm
	 * @param errorMessage 
	 */
	void error(std::string errorMessage) {
		std::cout << errorMessage << std::endl;
		exit(1);
	}


};

#endif