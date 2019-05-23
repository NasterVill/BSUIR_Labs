#include "View.h"

using namespace std;

void View::Run()
{
	while (true)
	{
		system("cls");
		this->ShowMenuOptions();
		this->ShowModelData();

		switch (_getch())
		{
			case '0':
			{
				return;
			}
			case '1':
			{
				this->AddNewObject();
				break;
			}
			case '2':
			{
				this->AddNewReference();
				break;
			}
			case '3':
			{
				this->RemoveOldReference();
				break;
			}
			case '4':
			{
				this->AllocateMemoryToExisting();
				break;
			}
			case '5':
			{
				this->FreeAllocatedMemory();
				break;
			}
			case '6':
			{
				this->InitiazlieAllocated();
				break;
			}
			default:
			{
				continue;
			}
		}
	}
}

void View::AddNewObject()
{
	TYPE type = this->SelectType();

	cout << endl;

	int size = -1;
	do {
		cout << "Plesae, select object's size (in number of elements, not in bytes) : ";
		cin >> size;

		if (cin.good())
		{
			break;
		}
		else
		{
			cout << "Error! Size must be a positive number!";
			cin.clear();
			string garbage;
			getline(cin, garbage);
		}
	} while (!cin.good() || size <= 0);

	this->controller_->PushNewObject(type, size);
}

void View::AddNewReference()
{
	TYPE type = this->SelectType();
	int index = this->SelectIndex();
	this->controller_->AddReference(type, index);
}

void View::RemoveOldReference()
{
	TYPE type = this->SelectType();
	int index = this->SelectIndex();
	this->controller_->RemoveReference(type, index);
}

void View::AllocateMemoryToExisting()
{
	TYPE type = this->SelectType();
	int index = this->SelectIndex();

	int size = -1;
	do {
		cout << "Plesae, select object's size (in number of elements, not in bytes) : ";
		cin >> size;

		if (cin.good())
		{
			break;
		}
		else
		{
			cout << "Error! Size must be a positive number!";
			cin.clear();
			string garbage;
			getline(cin, garbage);
		}
	} while (!cin.good() || size <= 0);

	this->controller_->AllocateTo(type, index, (size_t)size);
}

void View::FreeAllocatedMemory()
{
	TYPE type = this->SelectType();
	int index = this->SelectIndex();
	this->controller_->FreeFrom(type, index);
}

void View::InitiazlieAllocated()
{
	TYPE type = this->SelectType();
	int index = this->SelectIndex();

	string garbage;
	getline(cin, garbage);
	cout << "Please, enter the data, to initialize an object" << endl;

	string str;
	getline(cin, str);

	this->controller_->InitializeAt(type, index, str);
}

int View::SelectIndex()
{
	int index = -1;
	cout << endl;
	while (true)
	{
		cout << endl << "Plesae, select elemnt's index : ";
		cin >> index;
		if (index >= 0)
			return index;
		else
			cout << endl << "Error! Index must be a non-negative number!";
	}
}

TYPE View::SelectType()
{
	cout << "Available types : " << endl;
	cout << "0. char" << endl;
	cout << "1. int" << endl;
	cout << "2. float" << endl;
	cout << "3. double" << endl;

	cout << endl << "Please select elemnt's type : " << endl;
	while (true)
	{
		switch (_getch())
		{
		case '0':
		{
			return CHAR;
		}
		case '1':
		{
			return INT;
		}
		case '2':
		{
			return FLOAT;
		}
		case '3':
		{
			return DOUBLE;
		}
		default:
		{
			continue;
		}
		}
	}
}

void View::ShowMenuOptions()
{
	cout << "Menu options: " << endl;
	cout << "===================================================================" << endl;
	cout << "1. Add a new object" << endl;
	cout << "2. Add a new reference" << endl;
	cout << "3. Remove an existing reference" << endl;
	cout << "4. Allocate the memory to an existing object" << endl;
	cout << "5. Free the memory of an existing object" << endl;
	cout << "6. Initialize an existing object" << endl;
	cout << "0. Exit" << endl;
	cout << "===================================================================" << endl << endl;
}

void View::ShowModelData()
{
	cout << "Chars : ";
	int i = 0;
	for (auto& element : this->controller_->GetModel()->char_vector_)
	{
		cout << endl;
		cout << i << ".\t" << "size : " << element.size() << endl;
		cout << "Data : ";
		for (int j = 0; j < element.size(); j++)
		{
			if (element[j] != '\0')
				cout << element[j];
			else
				cout << 0;
		}
		cout << endl;
		i++;
	}
	cout << endl << endl; 

	i = 0;
	cout << "Ints : ";
	for (auto& element : this->controller_->GetModel()->int_vector_)
	{
		cout << endl;
		cout << i << ".\t" << "size : " << element.size() << endl;
		cout << "Data : ";
		for (int j = 0; j < element.size(); j++)
			cout << element[j] << ' ';
		cout << endl;
		i++;
	}
	cout << endl << endl;

	i = 0;
	cout << "Floats : ";
	for (auto& element : this->controller_->GetModel()->float_vector_)
	{
		cout << endl;
		cout << i << ".\t" << "size : " << element.size() << endl;
		cout << "Data : ";
		for (int j = 0; j < element.size(); j++)
			cout << element[j] << ' ';
		cout << endl;
		i++;
	}
	cout << endl << endl;

	i = 0;
	cout << "Doubles : ";
	for (auto& element : this->controller_->GetModel()->double_vector_)
	{
		cout << endl;
		cout << i << ".\t" << "size : " << element.size() << endl;
		cout << "Data : ";
		for (int j = 0; j < element.size(); j++)
			cout << element[j] << ' ';
		cout << endl;
		i++;
	}
	cout << endl << endl;
}

View::View()
{
}

View::View(Controller* controller) : controller_(controller)
{
}

View::~View()
{
}