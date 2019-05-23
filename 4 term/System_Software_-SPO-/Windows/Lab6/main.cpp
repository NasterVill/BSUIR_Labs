#include "SmartPointer.h"
#include "MemoryManager.h"
#include "Controller.h"
#include "Model.h"
#include "View.h"
#include <iostream>

using namespace std;

int main()
{
	MemoryManager memmory_manager;

	Model* model = new Model;
	model->memory_manager_ = dynamic_cast<IMemoryManager*>(&memmory_manager);
	Controller* controller = new Controller(model);
	View* view = new View(controller);

	view->Run();

	delete view;
	delete controller;
	delete model;
}