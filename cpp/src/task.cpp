/*
 * task.cpp
 *
 *  Created on: 02.12.2015
 *      Author: frido
 */

#include "task.h"
#include <ctime>
#include <algorithm>

void task::addInputAction(std::function<bool (task*)> guard, std::function<void (task*)> command){
	guardedCommands.push_back(inputActionPair{guard, command});
}

void task::handleMessage(){
	//TODO
	return;
}

//evaluates all guarded commands and executes all true ones in a random order
void task::execute(){
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

task::task() {
	// TODO Auto-generated constructor stub
}

task::~task() {
	// TODO Auto-generated destructor stub
}
