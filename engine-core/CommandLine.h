#pragma once

#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <utility>
#include <map>
#include <functional>

class Command {
private:
public:
	virtual void execute(std::string args) = 0;
};

class CommandLine
{
private:
	bool running;
	bool first;
	bool error;
	std::thread reader;
	std::mutex queue_lock;
	std::queue<std::pair<Command*, std::string>> queue;
	std::map<std::string, Command*> commands;

	void readCommands();

public:
	CommandLine();
	~CommandLine();

	bool registerCommand( std::string word, Command* cmd);
	std::pair<Command*, std::string> parse( std::string cmd );

	void update();
};


class Echo : public Command {
private:
public:
	Echo() {}
	void execute( std::string args );
};

class Help : public Command {
private:
	std::map<std::string, Command*>* commands;

public:
	Help( std::map<std::string, Command*>* commands) : commands( commands ) {}
	void execute( std::string args );
};