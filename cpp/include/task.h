/*
 * task.h
 *
 *  Created on: 02.12.2015
 *      Author: frido
 */

#include <vector>
#include <functional>
#ifndef TASK_H_
#define TASK_H_

class task {
	struct inputActionPair{
		std::function<bool (task*)> guard;
		std::function<void (task*)> command;
	};

	std::vector<inputActionPair> guardedCommands;

public:
	task();
	virtual ~task();

	void addInputAction(std::function<bool (task*)>, std::function<void (task*)>);

	void handleMessage();

	void execute();
};

#endif /* TASK_H_ */
