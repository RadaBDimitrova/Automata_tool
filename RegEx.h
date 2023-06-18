#pragma once
#include "MyString.h"
#include "StringView.h"

class RegExParse {

	class RegExpr {
	public:
		MyString regEx;
		virtual bool evaluate(const StringView& interpretation) const = 0;
		virtual RegExpr* clone() const = 0;
		virtual ~RegExpr() = default;
	};

	class Unary : public RegExpr {
	public:
		char operand;
		RegExpr* expr;
	private:
	};

	class Binary : public RegExpr {
	private:
		char operand;
		RegExpr* left;
		RegExpr* right;
	public:
	};

	RegExpr* expr;
	RegExpr* parseExpression(const StringView& str);
};
