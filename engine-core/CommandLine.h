#pragma once

#include <memory>
#include <string>

class CommandLine
{
public:
	CommandLine();
	~CommandLine();

	void update();
};

class Command {
public:
	virtual void execute() = 0;

	static std::unique_ptr<Command> parse(std::string cmd);
};

class Echo : public Command {
private:
	std::string echo;
public:
	Echo(std::string str);
	void execute();
};