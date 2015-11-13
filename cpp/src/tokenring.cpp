/**
 * author: simon
 */

#include <iostream>

#include "server.hpp"
#include "message.hpp"


int main(int argc, char **argv) {
	std::cout << "Starte Test..." << std::endl;
	
	// Aktuell noch nicht so viel. Man kann sich mit Telnet drauf verbinden und Nachrichten verschicken.
	// Wenn der erste Buchstabe ein kleines q ist wird der Server beendet
	// Noch kein Weiterleiten des Tokens, oder synchronisiertes Verhalten
	Server* server = new Server("1111", "1112");
	server->start();
	delete server;

	return 0;
}
