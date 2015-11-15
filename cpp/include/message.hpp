/**
 * author: simon
 */

#ifndef MESSAGE
#define MESSAGE

#include <sys/socket.h>

/**
 * Message-Format welches zur Zeit nur fürs speichern verwendet werden kann,
 * da Serialisierung und Deserialisierung nicht implementiert sind.
 * Dies wäre durchaus cool.
 */
class Message {

public:
	/**
	 * Konstrukort
	 */
	Message(unsigned int id, std::string message) :
		_id(id),
		_message(message) {}

	// Message(void* buffer) {
	// 	_id = (unsigned int) buffer;
	// 	_message = *(buffer + sizeof(int)) bis maxsize;
	// }

	/**
	 * Destruktor
	 */
	~Message() {}

	unsigned int getID() {
		return _id;
	}

	std::string getMessage() {
		return _message;
	}

	sockaddr getFromAddr() {
		return _from;
	}

	int getMessageSize() {
		return _message.size();
	}

	// void* getData() {
	// 	void* data;
	// 	// data* = _id;

	// 	// (data + sizeof(int))* copy message;
	// }
	
private:
	// Name der Nachricht, bisher auch Inhalt
	unsigned int _id;
	std::string _message;
	struct sockaddr _from;
};

	

#endif
	