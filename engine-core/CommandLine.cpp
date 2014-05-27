#include "CommandLine.h"

#include <iostream>

CommandLine::CommandLine()
	: running(true)
	, reader(&CommandLine::readCommands, this)
{}


CommandLine::~CommandLine() {
	this->running = false;
}

void CommandLine::readCommands() {
	std::string str;
	
	std::cout << "> " << std::endl;

	while (this->running && std::getline(std::cin, str)) {
		Command* cmd = Command::parse(str);
		
		queue_lock.lock();
		queue.push(cmd);
		queue_lock.unlock();

		std::cout << "> " << std::endl;
	}
}

void CommandLine::update() {
	this->queue_lock.lock();
	while (!this->queue.empty()) {
		Command* cmd = this->queue.front();
		this->queue.pop();
		cmd->execute();
		delete cmd;
	}
	this->queue_lock.unlock();;
}

Command* Command::parse(std::string cmd) {
	if (cmd.substr(0, 4) == std::string("echo"))
		return new Echo(cmd.substr(4));
	else
		return nullptr;
}

Echo::Echo(std::string cmd) : echo(cmd) {}
void Echo::execute() {
	std::cout << this->echo << std::endl;
}