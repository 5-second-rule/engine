#include "CommandLine.h"

#include <iostream>
#include <sstream>

CommandLine::CommandLine()
	: running(true)
	, reader(&CommandLine::readCommands, this) {
	
	this->registerCommand( "echo", new Echo() );
}

CommandLine::~CommandLine() {
	this->running = false;
	//TODO clean up map command pointers
}

void CommandLine::readCommands() {
	std::string str;
	
	std::cout << std::endl << "> ";

	while (this->running && std::getline(std::cin, str)) {
		std::pair<Command*, std::string> cmd = this->parse( str );
		
		if (cmd.first != nullptr) {
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
		std::pair<Command*, std::string> cmd = this->queue.front();
		this->queue.pop();
		cmd.first->execute(cmd.second);
	}
	this->queue_lock.unlock();
}

std::pair<Command*, std::string> CommandLine::parse( std::string cmd ) {
	std::stringstream stream(cmd);
	std::string word;
	std::string args;
	stream >> word;
	stream >> args;

	if (this->commands.count(word) != 0) {
		return std::pair<Command*, std::string>(this->commands[word], args);
	} else {
		return std::pair<Command*, std::string>( nullptr, args );
	}

}

bool CommandLine::registerCommand(std::string word, Command* cmd) {
	if( this->commands.count( word ) != 0 ) {
		return false;
	} else {
		this->commands[word] = cmd;
		return true;
	}
}

Echo::Echo() {}
void Echo::execute( std::string args ) {
	std::cout << args << std::endl;
}