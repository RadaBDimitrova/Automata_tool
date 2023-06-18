#include "MyString.h"
#pragma warning (disable:4996)

MyString::MyString(size_t capacity) {
	if (capacity > SIZE_SSO - 1) {
		str._long.len = capacity;
		str._long.longStr = new char[capacity + 1];
		useLong();
	}
	else {
		str._short.string[SIZE_SSO - 1] = capacity;
		useShort();
	}
}

MyString::MyString(char c) {
	str._short.string[SIZE_SSO - 1] = 1;
	str._short.string[0] = c;
	useShort();
}


MyString operator+(const MyString& lhs, const MyString& rhs) {
	MyString result(lhs);
	result += rhs;
	return result;
}

MyString& MyString::operator+=(const MyString& other) {
	size_t len = strlen(c_str()) + strlen(other.c_str());
	char* result = new char[len + 1];
	result[0] = '\0';
	strcat(result, c_str());

	if (other.isLong()) {
		strcat(result, other.str._long.longStr);
	}
	else {
		strcat(result, other.str._short.string);
	}
	setData(result);
	return *this;
}

void MyString::setData(const char* newData) {
	size_t len = strlen(newData);
	free();
	if (len >= SIZE_SSO - 1) {
		str._long.len = len;
		str._long.longStr = new char[len + 1];
		strcpy(str._long.longStr, newData);
		useLong();
	}
	else {
		strcpy(str._short.string, newData);
		str._short.string[SIZE_SSO - 1] = len; // logic: (SIZE_SSO - 1 - len) << 1
		useShort();
	}
}

MyString::MyString() {
	str._short.string[0] = '\0';
	str._short.string[SIZE_SSO - 1] = 0;
}
void MyString::useLong() {
	str._short.string[SIZE_SSO - 1] |= (1 << 7);
}

void MyString::useShort() {
	str._short.string[SIZE_SSO - 1] &= ~(1 << 7);
}

MyString::MyString(const char* data) {
	setData(data);
}

MyString::MyString(const MyString& other) {
	copyFrom(other);
}

MyString& MyString::operator=(const MyString& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}
void MyString::copyFrom(const MyString& data) {
	setData(data.c_str());
}

void MyString::free() {
	if (isLong()) {
		delete[] str._long.longStr;
		str._long.longStr = nullptr;
		str._long.len = 0;
	}
}

MyString::~MyString() {
	free();
}

bool MyString::isLong() const {
	return str._short.string[SIZE_SSO - 1] >> 7; //logic: 0 for short 1 for long
}

size_t MyString::length() const {
	if (isLong()) {
		return str._long.len;
	}
	return str._short.string[SIZE_SSO - 1];
}

char& MyString::operator[](size_t index) {
	if (isLong()) {
		return str._long.longStr[index];
	}
	return str._short.string[index];
}

char MyString::operator[](size_t index) const {
	if (isLong()) {
		return str._long.longStr[index];
	}
	return str._short.string[index];
}

MyString MyString::substr(size_t begin, size_t howMany) const {
	if (begin + howMany > strlen(c_str())) {
		throw std::length_error("Error! Substr out of range!");
	}

	char* result = new char[howMany + 1];
	for (size_t i = 0; i < howMany; i++) {
		result[i] = *(c_str() + begin + i);
	}
	result[howMany] = '\0';
	MyString res(result);

	return res;
}

const char* MyString::c_str() const {
	if (isLong()) {
		return str._long.longStr;
	}
	return str._short.string;
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, MyString& str) {
	char buff[1024];
	is >> buff;
	str.setData(buff);
	return is;
}

bool operator<(const MyString& lhs, const MyString& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator<=(const MyString& lhs, const MyString& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) <= 0;
}
bool operator>=(const MyString& lhs, const MyString& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) >= 0;
}
bool operator>(const MyString& lhs, const MyString& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}
bool operator==(const MyString& lhs, const MyString& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}
bool operator!=(const MyString& lhs, const MyString& rhs) {
	return strcmp(lhs.c_str(), rhs.c_str()) != 0;
}