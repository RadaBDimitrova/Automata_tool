#pragma once
unsigned const SIZE_SSO = 16;

union StringUnion
{
	struct ShortString {
		char string[SIZE_SSO];
	};

	struct LongString {
		char* longStr = nullptr;
		size_t len = 0; //if largest bit is 0, switch to short
	};

	ShortString _short;
	LongString _long;
	StringUnion() : _long() {}
};