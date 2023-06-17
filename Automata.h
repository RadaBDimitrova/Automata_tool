#pragma once
//#include "RelationTripple.hpp"
#include "Vector.hpp"
#include "StringView.h"

class Automata
{
private:
	template <typename T, typename D, typename K>
	struct Triple {
		T first;
		D rel;
		K second;
		Triple(const T& first, const D& rel, const K& second) : first(first), rel(rel), second(second) {}
	};

	Vector <Triple<size_t, char, size_t>> links;
	Vector <size_t> start;
	Vector <size_t> end;
	size_t states;
	bool isDeterministic;

public:
	Automata() = default; // ?
	Automata(Vector <Triple<size_t, char, size_t>>& otherLinks, Vector <size_t>& otherStart, Vector <size_t>& otherEnd, size_t otherStates, bool isDet);

	void addState(size_t state);
	void removeState(size_t state);

	void Union(const Automata& other);
	void Concatenation(const Automata& second);
	void Kleene();
	bool accept(const StringView& word) const;
	bool isEmptyLanguage() const;


private:
	bool isFinal(size_t state) const;
	bool isStart(size_t state) const;
	bool _accept(size_t current, const StringView& word) const;
};



Automata Union(const Automata& first, const Automata& second) {
	Automata result;
	result = first;
	result.Union(second);
	return result;
}

Automata Concatenation(const Automata& first, const Automata& second) {
	Automata result;
	result = first;
	result.Concatenation(second);
	return result;
}

void Automata::addState(size_t state) {

}


void Automata::Union(const Automata& other) {
	isDeterministic = false;

	for (size_t i = 0; i < other.start.getSize(); i++) {
		start.push_back(other.start[i] + states);
	}
	for (size_t i = 0; i < other.end.getSize(); i++) {
		end.push_back(other.end[i] + states);
	}
	for (size_t i = 0; i < other.states; i++) {
		links.push_back({ other.links[i].first + states, other.links[i].rel, other.links[i].second + states });
	}
	states += other.states;
}

bool Automata::isFinal(size_t state) const {
	for (size_t i = 0; i < end.getSize(); i++) {
		if (links[state].first == end[i]) { //second?
			return true;
		}
	}
	return false;
}
bool Automata::isStart(size_t state) const {
	for (size_t i = 0; i < start.getSize(); i++) {
		if (links[state].first == start[i]) {
			return true;
		}
	}
	return false;
}

void Automata::Concatenation(const Automata& other) {
	isDeterministic = false;

	for (size_t i = 0; i < other.links.getSize(); i++) {
		if (other.isStart(other.links[i].first)) { //1. every final from first has to do the job of the start from second
			for (size_t j = 0; j < end.getSize(); j++) {
				links.push_back({ end[j], other.links[i].rel, other.links[i].second + states });
			}
		}
		else {
			links.push_back({ other.links[i].first + states, other.links[i].rel + states, other.links[i].second + states }); //pushes the other links
		}
	}
	for (size_t i = 0; i < other.states; i++) {
		//2. end in first stays only if there is a state in second that is both final and start
		if (other.isStart(i) && other.isFinal(i)) {
			end.clear();
		}
	}

}

void Automata::Kleene() {
	isDeterministic = false;
	for (size_t i = 0; i < links.getSize(); i++) {
		if (isStart(links[i].first)) { //1. every final has to do the job of the start
			for (size_t j = 0; j < end.getSize(); j++) {
				links.push_back(Triple<size_t, char, size_t>(end[j], links[i].rel, links[i].second));
			}
		}
	}
	for (size_t i = 0; i < start.getSize(); i++) { //
		if (!isFinal(start[i])) {
			end.push_back(start[i]);
		}
	}
}

bool Automata::accept(const StringView& word) const {
	for (size_t i = 0; i < start.getSize(); i++) {
		if (_accept(start[i], word)){
			return true;
		}
	}
	return false;
}

bool Automata::_accept(size_t current, const StringView& word) const {
	if (word.length() == 0 && isFinal(current)) {
		return true;
	}
	else if (word.length() == 0) {
		return false;
	}
	for (size_t i = 0; i < links.getSize(); i++) {
		if (links[i].first == current && links[i].rel == word[0] && _accept(links[i].second, word.substr(1, word.length() - 1))) {
			return true;
		}
	}
	return false;
}

bool Automata::isEmptyLanguage() const {
	Vector<size_t> reachableStates(start);
	for (size_t i = 0; i < links.getSize(); i++){
		if (isStart(i)) {
			reachableStates.push_back(links[i].second);
		}
	}
	for (size_t i = 0; i < reachableStates.getSize(); i++){
		if (isFinal(i)) {
			return false;
		}
	}
	return true;
}

