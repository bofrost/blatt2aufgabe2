/**
 * author: simon
 */

#include <iostream>

#include "server.hpp"
#include "message.hpp"
#include "counter.hpp"


int main(int argc, char **argv) {
	std::cout << "Starte Test..." << std::endl;
	
	// Aktuell noch nicht so viel. Man kann sich mit Telnet drauf verbinden und Nachrichten verschicken.
	// Wenn der erste Buchstabe ein kleines q ist wird der Server beendet
	// Noch kein Weiterleiten des Tokens, oder synchronisiertes Verhalten
	Counter* c1 = new Counter();
	Counter* c2 = new Counter();
	Counter* c3 = new Counter();

	Server* server1 = new Server("1111", "1112", c1);
	Server* server2 = new Server("1112", "1113", c2);
	Server* server3 = new Server("1113", "1111", c3);

	std::thread t1(&Server::start, server1);
	std::thread t2(&Server::start, server2);
	std::thread t3(&Server::start, server3);
	
	t1.join();
	t2.join();
	t3.join();

	delete server1;
	delete server2;
	delete server3;
	server1 == NULL;
	server2 == NULL;
	server3 == NULL;
	delete c1;
	delete c2;
	delete c3;
	c1 == NULL;
	c2 == NULL;
	c3 == NULL;

	return 0;
}
