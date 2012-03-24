#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>
#include <sstream>
using namespace std;

class FatalError: public exception //comme sur Windows!
{
public:
	FatalError(const std::string& description) throw()
		:description(description)
	{}

	virtual const char* what() const throw()
	{
		return description.c_str();
	}

	virtual ~FatalError() throw()
	{}

private:
	std::string description;
};


class SyntaxError: public exception
{
public:
	SyntaxError(int line,int column, const std::string& description) throw()
		:line(line),column(column),description(description)
	{}

	virtual const char* what() const throw()
	{
		ostringstream buf;
		buf<<"Erreur\n"<<line<<":"<<column<<":\n"<<description<<endl;
		return buf.str().c_str();
	}

	virtual ~SyntaxError() throw()
	{}

private:
	int line, column;
	std::string description;
};

class RunTimeError: public exception
{
public:
	RunTimeError(const std::string& description) throw()
		:description(description)
	{}

	virtual const char* what() const throw()
	{
		ostringstream buf;
		buf<<description<<endl;
		return buf.str().c_str();
	}

	virtual ~RunTimeError() throw()
	{}

private:
	int line, column;
	std::string description;
};

#endif /* EXCEPTION_HPP */

