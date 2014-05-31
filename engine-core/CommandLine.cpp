#include "CommandLine.h"

#include <iostream>
#include <sstream>

CommandLine::CommandLine()
	: running( true )
	, first( true )
	, error( false )
	, reader(&CommandLine::readCommands, this)
{	
	this->registerCommand( "echo", new Echo() );
	this->registerCommand( "help", new Help(&this->commands) );
}

CommandLine::~CommandLine() {
	this->running = false;
	this->reader.join();
	
	//TODO clean up map command pointers
}

void CommandLine::readCommands() {
	std::string str;

	while (this->running && std::getline(std::cin, str)) {
		std::pair<Command*, std::string> cmd = this->parse( str );
		
		if (cmd.first != nullptr) {
			queue_lock.lock();
			queue.push(cmd);
			queue_lock.unlock();
		}
		else {
			this->error = true;
		}
	}
}

void CommandLine::update() {
	if( this->first ) {
		this->first = false;
		std::cout << "Type help for a list of available commands" << std::endl;
		std::cout << std::endl << "> ";
	}

	if( this->error ) {
		this->error = false;
		std::cout << "error: Unknown command" << std::endl;
		std::cout << std::endl << "> ";
	}

	this->queue_lock.lock();
	while (!this->queue.empty()) {
		std::pair<Command*, std::string> cmd = this->queue.front();
		this->queue.pop();
		cmd.first->execute(cmd.second);
		std::cout << std::endl << "> ";
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


// builtin commands
void Echo::execute( std::string args ) {
	std::cout << args << std::endl;
}

void Help::execute( std::string args ) {
	std::map<std::string, Command*>::const_iterator begin = this->commands->cbegin();
	std::map<std::string, Command*>::const_iterator end = this->commands->cend();
	std::map<std::string, Command*>::const_iterator curr;
	std::cout << std::endl;
	for( curr = begin; curr != end; curr++ ) {
		std::cout << curr->first << std::endl;
	}
}