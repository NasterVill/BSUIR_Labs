#ifndef MODEL_H
#define MODEL_H

#include "IMemoryManager.h"
#include "SmartPointer.h"
#include <vector>

/// <summary>
/// MVC model in this project is pretty bad, because it was written for about 2 and a half hours,
/// so i recommend rewriting the GUI, if you intend to use this memory model
/// </summary>
struct Model
{
private:
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

public:
	Model();
	~Model();

public:
	std::vector<SmartPointer<char>> char_vector_;
	std::vector<SmartPointer<int>> int_vector_;
	std::vector<SmartPointer<float>> float_vector_;
	std::vector<SmartPointer<double>> double_vector_;

	memorymanager::IMemoryManager* memory_manager_;
};

#endif