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
		Goods();							// ����������� �� ���������
		Goods(const char*, const int);		// ����������� ������������� � �����������
		Goods(const Goods &);				// ����������� �����������
		~Goods();

		void setCost(int);
		void setName(char*);
		int getCost();
		int getId();

		friend class Vendor;

		friend void showProduct(Goods &object);
};