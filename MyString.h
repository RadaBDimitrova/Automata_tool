#pragma once
#include <iostream>
#include "SSO.h"

class MyString
{
private:
	StringUnion str;

	void copyFrom(const MyString& data);
	void free();

	bool isLong() const;
	void useLong();
	void useShort();
	explicit MyString(size_t capacity); //for substr function
	void setData(const char* newData);
public:

	MyString();
	MyString(const char* data);
	MyString(char c);
	MyString(const MyString& other);
	MyString& operator=(const MyString& other);
	~MyString();

	size_t length() const;
	MyString& operator+=(const MyString& other);

	MyString substr(size_t begin, size_t howMany) const;

	char& operator[](size_t index);
	char operator[](size_t index) const;

	const char* c_str() const;

	friend MyString operator+(const MyString& lhs, const MyString& rhs);
	friend std::istream& operator>>(std::istream&, MyString& str);
};

MyString operator+(const MyString& lhs, const MyString& rhs);

std::ostream& operator<<(std::ostream& os, const MyString& str);

std::istream& operator>>(std::istream& os, MyString& str);

bool operator<(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);
bool operator>=(const MyString& lhs, const MyString& rhs);
bool operator>(const MyString& lhs, const MyString& rhs);
bool operator==(const MyString& lhs, const MyString& rhs);
bool operator!=(const MyString& lhs, const MyString& rhs);


