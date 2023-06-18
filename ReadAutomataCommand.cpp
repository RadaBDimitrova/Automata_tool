#include "ReadAutomataCommand.h"
void ReadAutomataCommand::execute(std::ostream& ofs) {
	readAutomataFromBinary(name.c_str());
	ofs << "Executed!" << std::endl;
}