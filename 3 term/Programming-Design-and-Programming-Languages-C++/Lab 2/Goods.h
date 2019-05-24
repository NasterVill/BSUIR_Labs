#pragma once

#include "MyString.h"

class Goods
{
	private: 
		int id;
		static int last_id;
		MyString name;
		int cost;
	public:
		Goods();							// конструктор по умолчанию
		Goods(const char*, const int);		// конструктор инициализации с параметрами
		Goods(const Goods &);				// конструктор коприования
		~Goods();

		void setCost(int);
		void setName(char*);
		int getCost();
		int getId();

		friend class Vendor;

		friend void showProduct(Goods &object);
};