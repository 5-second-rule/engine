#include "CommandLine.h"

#include <iostream>
#include <sstream>

CommandLine::CommandLine()
	: running(true)
	, reader(&CommandLine::readCommands, this)
{
	Command::registerCommand<Echo>("echo");
}

CommandLine::~CommandLine() {
	this->running = false;
}

void CommandLine::readCommands() {
	std::string str;
	
	std::cout << std::endl << "> ";

	while (this->running && std::getline(std::cin, str)) {
		Command* cmd = Command::parse(str);
		
		if (cmd != nullptr) {
			queue_lock.lock();
			queue.push(cmd);
			queue_lock.unlock();
		}
		else {
			std::cout << "error: Unknown command" << std::endl;
		}

		std::cout << std::endl << "> ";
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
	this->queue_lock.unlock();
}

std::map<std::string, std::function<Command*(std::string)>> Command::commands;

Command* Command::parse(std::string cmd) {
	std::stringstream stream(cmd);
	std::string word;
	stream >> word;

	if (Command::commands.count(word) != 0) {
		return Command::commands[word](cmd.substr(word.length()));
	} else {
		return nullptr;
	}

}

Echo::Echo(std::string cmd) : echo(cmd) {}
void Echo::execute() {
	std::cout << this->echo << std::endl;
}