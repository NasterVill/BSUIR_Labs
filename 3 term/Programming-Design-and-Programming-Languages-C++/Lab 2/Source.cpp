#include "MyString.h"
#include "Goods.h"
#include "Vendor.h"
#include <iostream>

using namespace std;

int main()
{
	const int max_size = 100;
	const int upper_border = 20;
	char buff[max_size] = { 0 };
	bool error = true; 
	int amount = 0;
	int input_cost = 0;

	Goods object_1, object_2("Cheese", 10), object_3("Meat", 40), object_4;
	Goods* obj_arr = nullptr;

	object_4.setName("Oil");														// проверка set
	object_4.setCost(15);															// методов

	cout << "Please input first object's name: ";									// инициализация имени первого объекта
	while (error)
	{
		error = false;
		memset(buff, 0, max_size * sizeof(char));
		rewind(stdin);
		cin.getline(buff, max_size);
		if (!stringLen(buff))
		{
			error = true;
			cout << "Error! Bad input! Please repeat" << endl;
			system("pause");
		}
	}
	object_1.setName(buff);
	error = true;

	cout << "Please input first object's cost: ";									// инициализация цены первого объекта
	while (error)
	{
		error = false;
		rewind(stdin);
		cin >> input_cost;
		if (input_cost < 0 || cin.good() == 0)
		{
			error = true;
			cout << "Error! Bad input! Please repeat" << endl;
			system("pause");
		}
	}
	object_1.setCost(input_cost);
	error = true;
	system("cls");


	cout << "Please input the number of elements in the array of objects" << endl;		// выделение памяти под массив объектов
	while (error)
	{
		error = false;
		rewind(stdin);
		cin >> amount;
		if (amount <= 0 || amount > upper_border || cin.good() == 0)
		{
			error = true;
			cout << "Error! Bad input! Please repeat" << endl;
			system("pause");
		}
	}
	error = true;
	obj_arr = new Goods[amount];
	system("cls");


	for (int i = 0; i < amount; i++)
	{
		cout << "Please input object's name: ";									// инициализация имени объекта
		while (error)
		{
			error = false;
			memset(buff, 0, max_size * sizeof(char));
			rewind(stdin);
			cin.getline(buff, max_size);
			if (!stringLen(buff))
			{
				error = true;
				cout << "Error! Bad input! Please repeat" << endl;
				system("pause");
			}
		}
		obj_arr[i].setName(buff);
		error = true;

		cout << "Please input object's cost: ";									// инициализация цены объекта
		while (error)
		{
			error = false;
			rewind(stdin);
			cin >> input_cost;
			if (input_cost < 0 || cin.good() == 0)
			{
				error = true;
				cout << "Error! Bad input! Please repeat" << endl;
				system("pause");
			}
		}
		obj_arr[i].setCost(input_cost);
		error = true;
	}
	system("cls");

	showProduct(object_1);
	showProduct(object_2);
	showProduct(object_3);
	showProduct(object_4);

	for(int i = 0; i < amount; i++)
		showProduct(obj_arr[i]);

	system("pause");

	cout << endl << endl << endl;
	Vendor::setProductCost(object_1, 20);
	showProduct(object_1);
	
	system("pause");
	return 0;
}