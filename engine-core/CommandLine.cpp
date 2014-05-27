#include "CommandLine.h"

#include <iostream>

CommandLine::CommandLine() {}


CommandLine::~CommandLine() {}

void CommandLine::update() {
	std::string str;
	while (std::getline(std::cin, str)) {
		std::unique_ptr<Command> cmd = Command::parse(str);
		if(cmd) cmd->execute();
		std::cout << "> " << std::endl;
	}
}

std::unique_ptr<Command> Command::parse(std::string cmd) {
	if (cmd.substr(0, 4) == std::string("echo"))
		return std::unique_ptr<Command>(new Echo(cmd.substr(4)));
	else
		return nullptr;
}

Echo::Echo(std::string cmd) : echo(cmd) {}
void Echo::execute() {
	std::cout << this->echo << std::endl;
}