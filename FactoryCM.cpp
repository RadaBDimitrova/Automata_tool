#include "FactoryCM.h"
#pragma warning (disable:4996)

FactoryCM& FactoryCM::getInstance()
{
	static FactoryCM factory;
	return factory;
}

Command* FactoryCM::getCommand() const
{
	int choice;
	std::cin >> choice;

	if (choice == 1) //create
	{
		char buff[1024];
		std::cin >> buff;
		return new CreateAutomataComand(MyString(buff)); 
	}
	else if (choice == 2)
	{
		char buff[1024];
		std::cin >> buff;
		return new ReadAutomataCommand(MyString(buff));
	}

}