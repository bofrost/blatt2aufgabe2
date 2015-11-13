/**
 * author: simon
 */

#ifndef SERVER
#define SERVER

#include "message.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <list>
#include <iostream>
#include <utility>
#include <thread>
// #include <chrono>
// #include s<functional>
// #include <atomic>

class Server {
public:
	/**
	 * Konstruktor
	 * @param port Portnummer für den Server
	 * @param nextPort Portnummer für den Server an welchen Token weitergeleitet wird
	 */
	Server(const std::string& port, const std::string& nextPort): 
		_port(port),
		_nextPort(nextPort) {}
	
	/**
	 * Destruktor
	 */
	~Server() {}

	/**
	 * startet Server in eigenem thread
	 */
	void start() {
		std::thread t(&Server::loop, this, _port);
		t.join();
		return;
	}

private:
	std::string _port;
	std::string _nextPort;
	// Queue mit den Messages
	std::list<Message> _messageQueue;
	// Anzahl der Tokens, welche der Server besitzt
	unsigned int _numberOfToken;

	/**
	 * Schleife, welche Verbindungen und Nachrichten annimmt
	 * @param port Portnummer für den Server
	 */
	void loop(const std::string& port) {
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
		serv_addr.sin_port = htons(atoi(port.c_str()));
		if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
			error("ERROR on binding");
		
		if (listen(sockfd,5) < 0) {
			close(sockfd);
			error("ERROR on listen");
		}

		while(buffer[0] != 'q') {
			len = sizeof(cli_addr);

			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &len);
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
			// Für Asynchrone Variante könnten hier Nachrichten gespeichert werden.
			// _messageQueue.push_back(Message(buffer));
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
		std::cout << buffer << std::endl;
		return;
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