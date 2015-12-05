/**
 * author:
 */

#ifndef TASK
#define TASK

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

class Task {
public:
	/**
	 * Konstruktor
	 * @param port Portnummer für den Server
	 * @param nextPort Portnummer für den Server an welchen Token weitergeleitet wird
	 */
	Task(std::vector<int> &in, std::vector<int> &out) {
		this->in = in;
		this->out = out;
	}

	/**
	 * Schleife, welche Verbindungen und Nachrichten annimmt
	 * @param port Portnummer für den Server
	 */
	virtual void loop() {
		std::cout << "gestartet" << std::endl;
		fd_set fds;
		int max = 0;
		while(true)
		{
			FD_ZERO (&fds);
			for (auto i : this->in) {
				FD_SET(i, &fds);
				if (i > max)
					max = i;

			}
			select(max + 1, &fds, NULL, NULL, NULL);
			for (auto i : this->in) {
				if (FD_ISSET(i, &fds))
					this->handleMessage(i);
			}
		}
	}

	/**
	 * Destruktor
	 */
	~Task() {
	}


protected:
	std::vector<int> in;
	std::vector<int> out;



	/**
	 * Behandlung der Nachrichten
	 * Zur Zeit nur in die standart ausgabe schreiben
	 * @param buffer Puffer mit Nachricht
	 */
	void handleMessage(int i) {
		std::cout << "handleMessage" << std::endl;
		char buffer[255];
		int n;
		n = read(i,buffer,sizeof buffer);
		if (n < 0) {
			close(i);
			error("ERROR reading from socket");
		}
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
