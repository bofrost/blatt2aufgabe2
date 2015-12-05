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
	struct inputActionPair{
		std::function<bool (Task*)> guard;
		std::function<void (Task*)> command;
	};

	std::vector<inputActionPair> guardedCommands;
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

	void addInputAction(std::function<bool (Task*)> guard, std::function<void (Task*)> command){
		guardedCommands.push_back(inputActionPair{guard, command});
	}

	//evaluates all guarded commands and executes all true ones in a random order
	void execute(){
		std::srand ( unsigned ( std::time(0) ) );
		//holds indices of guards evaluated to true
		std::vector<int> trueGuards;
		//evaluate guards
		for(int i=0; i<guardedCommands.size(); i++){
			if(guardedCommands[i].guard(this)){
				trueGuards.push_back(i);
			}
		}
		std::random_shuffle(trueGuards.begin(), trueGuards.end());
		for(int i=0; i<trueGuards.size(); i++){
			guardedCommands[trueGuards[i]].command(this); //execute commands
		}
	}

	virtual void init(){};

	/**
	 * Schleife, welche Verbindungen und Nachrichten annimmt
	 * @param port Portnummer für den Server
	 */
	virtual void loop() {
		std::cout << "gestartet" << std::endl;
		init();
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

	//Kommunikation mit den InputActionPairs über diese Variablen? (vorerst)
	int origin; //aus welcher Kante kommt die Nachricht
	int message;    //die nachricht //TODO int -> Message

	/**
	 * Behandlung der Nachrichten
	 * Zur Zeit nur in die standart ausgabe schreiben
	 * @param buffer Puffer mit Nachricht
	 */
	void handleMessage(int i) {
		std::cout << "handleMessage" << std::endl;
		//char buffer[255];
		int n;
		n = read(i,&message,sizeof message);
		if (n < 0) {
			close(i);
			error("ERROR reading from socket");
		}
		std::cout << message << std::endl;
		execute();
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
