#pragma once
#include "MyString.h"

namespace helper {
	template <typename T>
	void swap(T& first, T& second) {
		T temp = first;
		first = second;
		second = temp;
	}

	bool isValidChar(char c) {
		return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c=='+' || c=='*' || c== '.' || c == '(' || c == ')';
	}

	MyString regExToRPN(const MyString& regEx) {
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
			if (regEx[i] == ')')
			{
				bracketCtr--;
				continue;
			}
			if (bracketCtr == 1) {
				if (regEx[i] == '.' || regEx[i] == '+') {
					return regExToRPN(regEx.substr(1, i - 2)) + regExToRPN(regEx.substr(i + 1, regEx.length() - i - 2) + regEx[i]);
				}
				if (regEx[i] == '*')
					return regExToRPN(regEx.substr(1, i - 2)) + regEx[i];

				throw std::invalid_argument("Invalid regEx!");
			}
		}
	}
}

