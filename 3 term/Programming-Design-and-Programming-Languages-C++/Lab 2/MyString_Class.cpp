#include "MyString.h"
#include <iostream>
#include <assert.h>

using namespace std;

MyString::MyString()						//коструктор по умолчанию
{
	string = nullptr;
	size = 0;
}

MyString::MyString(const char* input_str)	//коструктор инициализации
{
	size = stringLen(input_str);
	assert(size > 0);

	string = new char[size];

	for (int i = 0; i < size; i++)
	{
		string[i] = input_str[i];
	}
}

MyString::MyString(const MyString& object)	//коструктор копирования
{
	size = object.size;
	string = new char[size];
	for (int i = 0; i < size; i++)
	{
		string[i] = object.string[i];
	}
}

MyString::~MyString()
{
	if (this->string != nullptr)
		delete[] string;
}

int MyString::getSize() const
{
	return size;
}

void MyString::setString(char* input_str)
{
	assert(stringLen(input_str) > 0);
	this->size = stringLen(input_str);
	this->string = new char[this->size];
	memset(this->string, 0, this->size*sizeof(char));

	for (int i = 0; i < this->size; i++)
	{
		this->string[i] = input_str[i];
	}
}

void MyString::show() const
{
	cout << string << endl;
}

int MyString::operator==(const MyString &object) const
{
	int result, i = 0;
	while (this->string[i] == object.string[i])
	{
		if (this->string[i] == '\0' && object.string[i] == '\0') return 0;
		if (this->string[i] == '\n' && object.string[i] == '\n') return 0;
		i++;
	}
	return result = this->string[i] > object.string[i] ? 1 : -1;
}

int MyString::operator==(const char* str) const
{
	int result, i = 0;
	while (this->string[i] == str[i])
	{
		if (this->string[i] == '\0' && str[i] == '\0') return 0;
		if (this->string[i] == '\n' && str[i] == '\n') return 0;
		i++;
	}
	return result = this->string[i] > str[i] ? 1 : -1;
}

MyString &MyString::operator=(const MyString &object)
{
	assert(&object != this && object.string != nullptr);
	if (this->string != nullptr) delete[] this->string;

	this->string = new char[object.size];
	this->size = object.size;

	for (int i = 0; i < this->size; i++)
	{
		this->string[i] = object.string[i];
	}
	return *this;
}

MyString &MyString::operator=(const char* str)
{
	assert(str != nullptr);
	if (this->string != nullptr) delete[] this->string;

	this->string = new char[stringLen(str)];
	this->size = stringLen(str);

	for (int i = 0; i < this->size; i++)
	{
		this->string[i] = str[i];
	}
	return *this;
}

int stringLen(const char* str)
{
	if (str == nullptr) return 0;

	int i = 0;

	for (; str[i] != '\0' && str[i] != '\n'; i++);
	if (i != 0) i++;

	return i;
}