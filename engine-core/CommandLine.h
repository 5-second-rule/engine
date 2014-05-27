#pragma once

#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

class Command {
public:
	virtual void execute() = 0;

	static Command* parse(std::string cmd);
};

class CommandLine
{
private:
	bool running;
	std::thread reader;
	std::mutex queue_lock;
	std::queue<Command*> queue;

	void readCommands();

public:
	CommandLine();
	~CommandLine();

	void update();
};


class Echo : public Command {
private:
	std::string echo;
public:
	Echo(std::string str);
	void execute();
};