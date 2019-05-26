#pragma once

int stringLen(const char*);

class MyString
{
	private:
		char* string;
		int size;
	public:
		MyString();					//коструктор по умолчанию
		MyString(const char*);		//коструктор инициализации
		MyString(const MyString&);	//коструктор копирования
		~MyString();

		int getSize() const;
		void MyString::setString(char* input_str);
		void show() const;
		int operator==(const MyString &) const;
		int operator==(const char*) const;
		MyString &operator=(const MyString &);
		MyString &operator=(const char* str);
};