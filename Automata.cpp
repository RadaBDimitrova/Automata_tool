#include "Automata.h"

bool isValidChar(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '+' || c == '*' || c == '.' || c == '(' || c == ')';
}

Automata Union(const Automata& first, const Automata& second) {
	Automata result = first;
	result.isDeterministic = false;

	for (size_t i = 0; i < second.start.getSize(); i++) {
		result.start.push_back(second.start[i] + result.states);
	}
	for (size_t i = 0; i < second.end.getSize(); i++) {
		result.end.push_back(second.end[i] + result.states);
	}
	for (size_t i = 0; i < second.states; i++) {
		result.links.push_back({ second.links[i].first + result.states, second.links[i].rel, second.links[i].second + result.states });
	}
	result.states += second.states;
	result.addAlphabet(second);

	return result;
}

Automata Concatenation(const Automata& first, const Automata& second) {
	Automata result = first;
	result.isDeterministic = false;

	for (size_t i = 0; i < second.links.getSize(); i++) {
		if (second.isStart(result.links[i].first)) { //1. every final from first has to do the job of the start from second
			for (size_t j = 0; j < result.end.getSize(); j++) {
				result.links.push_back({ result.end[j], second.links[i].rel, second.links[i].second + result.states });
			}
		}
		else {
			result.links.push_back({ second.links[i].first + result.states, second.links[i].rel, second.links[i].second + result.states }); //pushes the other links
		}
	}
	bool clearEnd = true;
	for (size_t i = 0; i < second.start.getSize(); i++) {
		//2. end in first stays only if there is a state in second that is both final and start
		if (second.isFinal(second.start[i])) {
			clearEnd = false;
		}
	}
	if (clearEnd) {
		result.end.clear();
		for (size_t i = 0; i < second.end.getSize(); i++) {
			result.end.push_back(second.end[i] + first.states);
		}
	}
	result.addAlphabet(second);
	result.states += second.states;

	return result;
}

Automata Kleene(const Automata& aut) {
	Automata result = aut;
	result.isDeterministic = false;
	for (size_t i = 0; i < result.links.getSize(); i++) {
		if (result.isStart(result.links[i].first)) { //1. every final has to do the job of the start
			for (size_t j = 0; j < result.end.getSize(); j++) {
				result.links.push_back({ result.end[j], result.links[i].rel, result.links[i].second });
			}
		}
	}
	for (size_t i = 0; i < result.start.getSize(); i++) { //
		if (!result.end.contains(result.start[i])) {
			result.end.push_back(result.start[i]);
		}
	}
	result.isDeterministic = false;
	return result;
}

Automata Complement(const Automata& autom) {
	Automata result = autom;
	Vector <size_t> newFinal;
	for (size_t i = 0; i < result.states; i++) {
		if (!result.isFinal(i)) {
			newFinal.push_back(i);
		}
	}
	result.end = std::move(newFinal);
	return result;
}

Automata::Automata(const MyString& expr) {
	(*this) = std::move(rpnToAutomata(expr));
}


bool Automata::isFinal(size_t state) const {
	if (end.contains(links[state].second)) {
		return true;
	}
	return false;
}

bool Automata::isStart(size_t state) const {
	if (start.contains(links[state].first)) {
		return true;
	}
	return false;
}

void Automata::addState() {
	states++;
}

void Automata::addAlphabet(const Automata& other) {
	for (size_t i = 0; i < other.alphabet.getSize(); i++) {
		if (!alphabet.contains(other.alphabet[i])) {
			alphabet.push_back(other.alphabet[i]);
		}
	}
}

bool Automata::accepts(const StringView& word) const {
	for (size_t i = 0; i < start.getSize(); i++) {
		if (accepts(start[i], word)) {
			return true;
		}
	}
	return false;
}

bool Automata::accepts(size_t current, const StringView& word) const {
	if (word.length() == 0 && isFinal(current)) {
		return true;
	}
	else if (word.length() == 0) {
		return false;
	}
	for (size_t i = 0; i < links.getSize(); i++) {
		if (links[i].first == current && links[i].rel == word[0] && accepts(links[i].second, word.substr(1, word.length() - 1))) {
			return true;
		}
	}
	return false;
}

bool Automata::isEmptyLanguage() const {
	Vector<size_t> reachableStates = start;
	Vector<size_t> newReachable;
	do {

		for (size_t i = 0; i < newReachable.getSize(); i++) {
			if (isFinal(newReachable[i])) {
				return false;
			}
			reachableStates.push_back(newReachable[i]);
		}
		newReachable.clear();

		for (size_t i = 0; i < links.getSize(); i++) {
			if (reachableStates.contains(links[i].first) && !reachableStates.contains(links[i].second)) {
				newReachable.push_back(links[i].second);
			}
		}

	} while (!newReachable.getSize() != 0);

	return true;
}

void Automata::Determinate() {

	if (isDeterministic) {
		return;
	}

	Automata result;
	result.addState();
	result.start.push_back(0);

	Vector<Vector<size_t>> oldToNew; //vector for saving corresponding set of states from old to state in new
	oldToNew.push_back(start);

	MyQueue<Vector<size_t>> creationQueue;
	creationQueue.push(start);

	while (!creationQueue.isEmpty()) {
		Vector<size_t> currentState = creationQueue.peek();
		size_t currentInd = oldToNew.find(currentState);

		for (size_t i = 0; i < alphabet.getSize(); i++)
		{
			Vector<size_t> goTo;
			for (size_t j = 0; j < links.getSize(); j++)
			{
				if (currentState.contains(links[j].first) && alphabet[i] == links[j].rel)
				{
					goTo.push_back(links[j].second);

				}
			}

			size_t goToInd = oldToNew.find(goTo);
			if (goToInd == oldToNew.nopos)
			{
				oldToNew.push_back(goTo);
				result.addState();
				for (size_t j = 0; j < end.getSize(); j++)
				{
					if (goTo.contains(end[j]))
					{
						end.push_back(states - 1);
					}
				}
				creationQueue.push(std::move(goTo));
				goToInd = states - 1;
			}
			result.links.push_back({ currentInd, alphabet[i], goToInd });
		}
	}
	isDeterministic = true;
	(*this) = std::move(result);
}

void Automata::reverse() {
	start.swap(end);
	for (size_t i = 0; i < links.getSize(); i++) {
		std::swap(links[i].first, links[i].second);
	}
}

void Automata::Minimize() {
	Determinate();
	reverse();
	Determinate();
	reverse();
	Determinate();
}

void Automata::makeTotal() {
	bool hasEmpty = false;
	for (size_t i = 0; i < states; i++) {

		for (size_t j = 0; j < alphabet.getSize(); j++) {

			bool hasLink = false;
			for (size_t k = 0; k < links.getSize(); k++) {
				if (links[k].rel == alphabet[j] && links[k].first == i) {
					hasLink = true;
				}
			}
			if (!hasLink && !hasEmpty) {
				addState();
				hasEmpty = true;
			}
			if (!hasLink) {
				links.push_back({ i, alphabet[j], states - 1 });
			}
		}
	}
}

Automata Automata::rpnToAutomata(const MyString& rpn) { //using Reverse Polish Notation (RPN)
	Stack<Automata> buildStack;

	for (size_t i = 0; i < rpn.length(); i++) {
		if (rpn[i] == '+') {
			Automata a1 = buildStack.peek();
			buildStack.pop();
			Automata a2 = buildStack.peek();
			buildStack.pop();

			Automata result = Union(a1, a2);
			buildStack.push(std::move(result));
		}
		else if (rpn[i] == '.') {
			Automata a1 = buildStack.peek();
			buildStack.pop();
			Automata a2 = buildStack.peek();
			buildStack.pop();

			Automata result = Concatenation(a1, a2);
			buildStack.push(std::move(result));
		}
		else if (rpn[i] == '*') {
			Automata a1 = buildStack.peek();
			buildStack.pop();

			Automata result = Kleene(a1);
			buildStack.push(std::move(result));
		}
		else {
			buildStack.push(Automata(rpn[i]));
		}
	}
	return buildStack.peek();
}

Automata::Automata(char c) {
	isDeterministic = true;
	alphabet.push_back(c);
	addState();
	start.push_back(0);
	addState();
	end.push_back(1);
	links.push_back({ 0, c, 1 });
}

MyString Automata::regExToRPN(const MyString& regEx) {
	if (regEx.length() == 1)
	{
		if (!isValidChar(regEx[0])) {
			throw std::invalid_argument("Symbol is invalid!");
		}
		return regEx;
	}
	size_t bracketCtr = 0;
	for (size_t i = 0; i < regEx.length(); i++)
	{
		if (regEx[i] == '(') {
			bracketCtr++;
			continue;
		}
		if (regEx[i] == ')') {
			bracketCtr--;
			continue;
		}

		if (bracketCtr == 1) {
			if (regEx[i] == '.' || regEx[i] == '+') {
				return regExToRPN(regEx.substr(1, i - 2)) + regExToRPN(regEx.substr(i + 1, regEx.length() - i - 2) + regEx[i]);
			}

			if (regEx[i] == '*') {
				return regExToRPN(regEx.substr(1, i - 2)) + regEx[i];

			}
			throw std::invalid_argument("Invalid regEx!");
		}
	}
}

Automata readAutomataFromBinary(const char* name) {
	std::ifstream file(name, std::ios::binary);
	if (!file.is_open()) {
		throw std::exception("File is not open!");
	}
	Automata result;

	size_t sizeLinks;
	file.read((char*)(&sizeLinks), sizeof(size_t));
	result.links.resize(sizeLinks);

	for (size_t i = 0; i < sizeLinks; i++) {
		size_t first, second;
		char rel;
		file.read((char*)(&first), sizeof(size_t));
		file.read((char*)(&rel), sizeof(char));
		file.read((char*)(&second), sizeof(size_t));
		result.links.push_back({ first, rel, second });
	}

	size_t sizeStart;
	file.read((char*)(&sizeStart), sizeof(size_t));
	result.start.resize(sizeStart);
	for (size_t i = 0; i < sizeStart; i++) {
		size_t state;
		file.read((char*)(&state), sizeof(size_t));
		result.start.push_back(state);
	}

	size_t sizeEnd;
	file.read((char*)(&sizeEnd), sizeof(size_t));
	result.end.resize(sizeEnd);
	for (size_t i = 0; i < sizeEnd; i++) {
		size_t state;
		file.read((char*)(&state), sizeof(size_t));
		result.end.push_back(state);
	}

	size_t sizeAlpha;
	file.read((char*)(&sizeAlpha), sizeof(size_t));
	result.alphabet.resize(sizeAlpha);
	for (size_t i = 0; i < sizeAlpha; i++) {
		char ch;
		file.read((char*)(&ch), sizeof(size_t));
		result.alphabet.push_back(ch);
	}

	size_t states;
	file.read((char*)(&states), sizeof(size_t));
	result.states = states;

	bool det;
	file.read((char*)(&det), sizeof(bool));
	result.isDeterministic = det;

}
void writeAutomataToBinary(const char* name, const Automata& toWrite) {
	std::ofstream file(name, std::ios::binary);
	if (!file.is_open()) {
		throw std::exception("File is not open!");
	}

	size_t sizeLinks = toWrite.links.getSize();
	file.write((const char*)(&sizeLinks), sizeof(size_t));
	for (size_t i = 0; i < sizeLinks; i++){
		file.write((const char*)(&toWrite.links[i].first), sizeof(size_t));
		file.write((const char*)(&toWrite.links[i].rel), sizeof(char));
		file.write((const char*)(&toWrite.links[i].second), sizeof(size_t));
	}

	size_t sizeStart = toWrite.start.getSize();
	file.write((const char*)(&sizeStart), sizeof(size_t));
	for (size_t i = 0; i < sizeStart; i++) {
		file.write((const char*)(&toWrite.start[i]), sizeof(size_t));
	}

	size_t sizeEnd = toWrite.end.getSize();
	file.write((const char*)(&sizeEnd), sizeof(size_t));
	for (size_t i = 0; i < sizeEnd; i++) {
		file.write((const char*)(&toWrite.end[i]), sizeof(size_t));
	}

	file.write((const char*)(&toWrite.states), sizeof(size_t));
	file.write((const char*)(&toWrite.isDeterministic), sizeof(bool));

}