#include "CreateAutomataComand.h"
void CreateAutomataComand::execute(std::ostream& ofs) {
	Automata(expr);
	ofs << "Executed!" << std::endl;
}
