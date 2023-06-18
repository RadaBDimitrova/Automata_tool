#pragma once
#include "Command.h"
#include "CreateAutomataComand.h"
#include "ReadAutomataCommand.h"

class FactoryCM
{
	FactoryCM() = default;
	FactoryCM(const FactoryCM&) = delete;
	FactoryCM& operator=(const FactoryCM&) = delete;

public:
	Command* getCommand() const;
	static FactoryCM& getInstance();
};

