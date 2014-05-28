#pragma once

#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <functional>

class Command {
private:
	static std::map<std::string, std::function<Command*(std::string)>> commands;
public:
	virtual void execute() = 0;

	static Command* parse(std::string cmd);
	
	template<typename T>
	static void registerCommand(std::string cmd) {
		if (Command::commands.count(cmd) == 0)
			Command::commands[cmd] = [](std::string c) {
			return new T(c);
		};
	}
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