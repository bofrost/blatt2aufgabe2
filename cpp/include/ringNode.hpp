/**
 * author: matthias
 */

#ifndef RINGNODE
#define RINGNODE

class RingNode : public Task {

public:
	RingNode(std::vector<int> &in, std::vector<int> &out, bool token = false) : Task(in, out),
		_token(token) {
	}

	void init(){
		addInputAction(std::bind(&RingNode::inputToken,this),std::bind(&RingNode::actionToken,this));
		addInputAction(std::bind(&RingNode::inputLoad,this),std::bind(&RingNode::actionLoad,this));
	}

	bool inputToken()
	{
		return message==TOKEN;
	}
	void actionToken()
	{
		int val = TOKEN;
		_in = origin;
		_demand = false;
		for(int i : out)
		{
			if(i!=_in && i!=_lg)
				write(i,&val,sizeof(val));
		}
	}
	bool inputLoad()
	{
		return message==LOAD;
	}
	void actionLoad()
	{
		int val = TOKEN;
		_demand = true;
		_lg = origin;
		if(_token)
			for(int i : out)
				if(i!=_lg)
				{
					write(i, &val, sizeof(val));
					return;
				}
	}
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
