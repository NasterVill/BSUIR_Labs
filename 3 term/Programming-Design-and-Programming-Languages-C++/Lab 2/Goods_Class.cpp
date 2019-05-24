#include "Goods.h"
#include "MyString.h"
#include <iostream>
#include <assert.h>

using namespace std;

int Goods::last_id = 0;

Goods::Goods() : name()	// конструктор по умолчанию
{
	last_id++;
	id = last_id;
	cost = 0;
}

Goods::Goods(const char* input_name, const int input_cost) : name(input_name)	// конструктор инициализации с параметрами
{
	assert(input_cost >= 0);
	last_id++;
	id = last_id;
	cost = input_cost;
}

Goods::Goods(const Goods &object)	// конструктор коприования
{
	this->id = object.id;
	this->cost = object.cost;
	this->name = object.name;
}

Goods::~Goods()
{
}

void Goods::setCost(int input_cost = 0)
{
	assert(input_cost >= 0);
	cost = input_cost;
}

void Goods::setName(char* input_str)
{
	assert(stringLen(input_str) > 0);
	this->name = input_str;
}

int Goods::getCost()
{
	return cost;
}

int Goods::getId()
{
	return id;
}

void showProduct(Goods &object)
{
	cout << endl << "Product's id: " << object.id << endl;
	cout << "Product's name: ";
	object.name.show();
	cout << "Product's cost: " << object.cost << endl;
}