/**
 * author: simon
 */

#ifndef MESSAGE
#define MESSAGE

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
	Message(std::string name) : _name(name) {}

	/**
	 * Destruktor
	 */
	~Message() {}

private:
	// Name der Nachricht, bisher auch Inhalt
	std::string _name;
	
};

#endif