/**
 * author: simon
 */

#ifndef COUNTER
#define COUNTER

/**
 * Counter Klasse welche für eine hochlaufende ID geanutzt wird.
 */
class Counter {
public:
	Counter() : _value(0), _start(0) {}
	Counter(unsigned int& start) : _value(start), _start(start) {}
	~Counter() {}

	unsigned int getValue() {
		return (_value);
	}

	void incValue() {
		++_value;
	}

	void reset() {
		_value = _start;
	}

private:
	unsigned int _value;
	unsigned int _start;
};

#endif