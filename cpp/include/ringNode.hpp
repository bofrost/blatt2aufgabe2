/**
 * author: matthias
 */

#ifndef RINGNODE
#define RINGNODE

class RingNode : public Task {

public:
	RingNode(std::vector<int> &in, std::vector<int> &out, bool token = false) : Task(in, out),
		_token(token) {}


	/**
	 * Destruktor
	 */
	~RingNode() {}

private:
	bool _token = false;
	bool _demand = false;
	int _lg = -1;	//Kante zu Lastgenerator
	int _in = -1;	//Kante von der Token kommt
	int _out = -1;	//Kante zu der Token geschickt wird
};

#endif
