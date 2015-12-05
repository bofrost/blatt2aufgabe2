/**
 * author: matthias
 */

#ifndef LOADGENERATOR
#define LOADGENERATOR

class LoadGenerator : public Task {

public:
	LoadGenerator(std::vector<int> &in, std::vector<int> &out) : Task(in, out) {}
	/**
	 * Destruktor
	 */
	~LoadGenerator() {}


	void loop()
	{
		int load = LOAD;
		while(true)
		{
			for(int i : this->out)
			{
				write(i, &load, sizeof(load));
			}

			std::cout << "alle gesendet" << std::endl;
			sleep(42);
		}
	}

private:
};

#endif
