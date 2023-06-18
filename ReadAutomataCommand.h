#pragma once
#include "Command.h"
class ReadAutomataCommand :public Command
{
	MyString name;

public:
	ReadAutomataCommand(const MyString& name) : name(name) {};
	ReadAutomataCommand(const ReadAutomataCommand&) = delete;
	ReadAutomataCommand& operator=(const ReadAutomataCommand&) = delete;
	void execute(std::ostream& ofs) override;
	~ReadAutomataCommand() = default;
};

