#include "StringView.h"

StringView::StringView(const char* begin, const char* end) : begin(begin), end(end) {}

StringView::StringView(const char* string) : StringView(string, string + strlen(string)) {}

StringView::StringView(const MyString& string) : StringView(string.c_str()) {}

size_t StringView::length() const {
	return end - begin;
}
char StringView::operator[](size_t ind) const {
	return begin[ind];
}

StringView StringView::substr(size_t from, size_t length) const {
	if (begin + from + length > end)
		throw std::length_error("Substring out of range!");

	return StringView(begin + from, begin + from + length);
}

std::ostream& operator<<(std::ostream& os, const StringView& strView) {
	const char* it = strView.begin;

	while (it != strView.end) {
		os << *it;
		it++;
	}
	return os;
}