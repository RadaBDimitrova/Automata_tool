#pragma once
#include <iostream>
#include "Automata.h"


class Command
{
public:
	friend class Automata;
	virtual ~Command() = default;
	void virtual execute(std::ostream& ofs) = 0;
};


