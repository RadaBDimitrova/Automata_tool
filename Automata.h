#pragma once
#include "Vector.hpp"
#include "StringView.h"
#include "Helper.hpp"
#include "Pair.hpp"
#include "RegEx.h"
#include "Queue.hpp"
#include "Stack.hpp"

class Automata
{
private:
	template <typename T, typename D, typename K>
	struct Triple {
		T first;
		D rel;
		K second;
		Triple() = default;
		Triple(const T& first, const D& rel, const K& second) : first(first), rel(rel), second(second) {}
	};

	Vector <Triple<size_t, char, size_t>> links;
	Vector <size_t> start;
	Vector <size_t> end;
	Vector <char> alphabet;
	size_t states;
	bool isDeterministic;

public:
	Automata() = default; // ?
	Automata(const MyString& expr);

	void addState();
	void removeState(size_t state);

	bool accepts(const StringView& word) const;
	bool isEmptyLanguage() const;
	void Determinate();
	void reverse();
	void Minimize();
	void makeTotal();

	friend Automata Union(const Automata& first, const Automata& second);
	friend Automata Concatenation(const Automata& first, const Automata& second);
	friend Automata Kleene(const Automata& autom);
	friend Automata Complement(const Automata& autom);

private:
	bool isFinal(size_t state) const;
	bool isStart(size_t state) const;

	bool accepts(size_t current, const StringView& word) const;
	void addAlphabet(const Automata& other);

	Automata rpnToAutomata(const MyString& rpn);
	explicit Automata(char c);
	MyString regExToRPN(const MyString& regEx);

};


Automata Union(const Automata& first, const Automata& second);
Automata Concatenation(const Automata& first, const Automata& second);
Automata Kleene(const Automata& autom);
Automata Complement(const Automata& autom);
