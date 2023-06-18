#pragma once
#include "Command.h"
class CreateAutomataComand : public Command
{
	MyString expr;

public:
	CreateAutomataComand(const MyString& expr) : expr(expr) {};
	CreateAutomataComand(const CreateAutomataComand&) = delete;
	CreateAutomataComand& operator=(const CreateAutomataComand&) = delete;
	void execute(std::ostream& ofs) override;
	~CreateAutomataComand() = default;

};

