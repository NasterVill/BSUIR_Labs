#ifndef SMART_POINTER_H
#define SMART_POINTER_H

#include "IMemoryManager.h"
#include "AbsentMemoryManagerException.h"
#include "MemoryAllocationException.h"
#include <stdexcept>
#include <iostream>
#include <list>

using namespace memorymanager;
using namespace std;


/// <summary>
/// SmartPointer class serves to ease the work with MemoryManagr class, by providing
/// an API, to store any type of data (from standart data types to user's structures and classes)
/// and to controll the allocated memory (which will be automaticly released after the destruction of
/// SmartPointer object) 
/// </summary>
template <typename Type>
class SmartPointer
{
private:
	/// <summary>
	/// This class will serve as a counter for references, in order to provide garbage collection in SmartPointer destructor.
	/// </summary>
	class ReferenceCounter
	{
	public:
		int add_reference()
		{
			// Increment the reference count
			return ++reference_count_;
		}
		int release()
		{
			// Decrement the reference count and
			// return the reference count.
			return --reference_count_;
		}

	public:
		ReferenceCounter() : reference_count_(0)
		{
		}
		~ReferenceCounter()
		{
		}

	private:
		int reference_count_;
	};

public:
	/// <summary>
	/// Serves to quickly move through the allocated array, and to modify it's elements (if needed).
	/// </summary>
	/// <param name="index">
	/// Index of an element.
	/// </param>
	/// <returns>
	/// Returns a value from the array by reference, in order to be able, to change it's content
	/// </returns>
	Type& operator[](const int index)
	{
		if (index < 0 || index >= *(this->size_)) 
			throw out_of_range("The value is out of range");
		
		int offset = 0;
		for (auto& block : (*this->connected_memory_))
		{
			int converted_size = block->byte_size / sizeof(Type);
			if (index < offset + converted_size)
			{
				Type* data_pointer_ = reinterpret_cast<Type*>(block->offset_pointer);
				return data_pointer_[index - offset];
			}
			offset += converted_size;
		}

		return this->stub_;
	}

	Type& operator*()
	{
		if (this->connected_memory_->empty())
			throw MemoryAllocationException("Memory is not allocated!");

		Type* data_pointer_ = reinterpret_cast<Type*>(this->connected_memory_->front()->offset_pointer);
		return *data_pointer_;
	}

	SmartPointer<Type>& operator=(const SmartPointer<Type>& other)
	{
		// Assignment operator
		if (this != &other) // Avoid self assignment
		{
			// Decrement the old references count
			// if references count become zero delete the old data
			if (this->references_->release() == 0)
			{
				if (!this->connected_memory_->empty())
					memory_manager_->free(*this->connected_memory_);
				delete this->connected_memory_;
				delete this->size_;
			}

			// Copy the data and references pointer
			// and increment the references count
			this->connected_memory_ = other.connected_memory_;
			this->size_ = other.size_;
			this->references_ = other.references_;
			this->memory_manager_ = other.memory_manager_;
			this->references_->add_reference();
		}
		return *this;
	}

	int size() const
	{
		return *(this->size_);
	}

	/// <summary>
	/// This method gives an ability to quickly initialize the allocated memory
	/// </summary>
	/// <param name="source">
	/// Source arraym to copy from
	/// </param>
	/// <param name="source_size">
	/// Size of source array
	/// </param>
	/// <returns>
	/// Returns amount of copied elements
	/// </returns>
	int copyData(Type* source, int source_size)
	{
		if (this->memory_manager_ == nullptr)
			throw AbsentMemoryManagerException("Memory manger is ansent!");

		int min_size = (*(this->size_) > source_size) ? source_size : *(this->size_);

		int i = 0;
		std::list<IMemoryManager::MemoryBlockData*>::iterator iterator;
		for(iterator = this->connected_memory_->begin(); iterator != this->connected_memory_->end(); iterator++)
		{
			if (i == source_size)
				break;

			Type* data_pointer = reinterpret_cast<Type*>((*iterator)->offset_pointer);
			for (int j = 0; j < (*iterator)->byte_size / (int)sizeof(Type); j++)
			{
				data_pointer[j] = source[i++];
				if (i == source_size)
					break;
			}
		}
		return i;
	}

	void allocateMemory(size_t size)
	{
		if (this->memory_manager_ == nullptr)
			throw AbsentMemoryManagerException("Memory manger is ansent!");

		if (!this->connected_memory_->empty())
		{
			this->freeMemory();
		}

		try
		{
			*this->connected_memory_ = this->memory_manager_->allocate(size * sizeof(Type), sizeof(Type));
			*(this->size_) = size;
		}
		catch (MemoryAllocationException &exception)
		{
			cout << endl << exception.what() << endl;
			system("pause");
			return;
		}
	}	

	void freeMemory()
	{
		if (this->memory_manager_ == nullptr)
			throw AbsentMemoryManagerException("Memory manger is ansent!");

		if (!this->connected_memory_->empty())
		{
			this->memory_manager_->free(*this->connected_memory_);
			*(this->size_) = 0;
			this->connected_memory_->clear();
		}
	}
	void setMemoryManager(IMemoryManager* memory_manager)
	{
		if(this->memory_manager_ != nullptr)
			this->freeMemory();

		this->memory_manager_ = memory_manager;
	}


public:
	SmartPointer() : memory_manager_(nullptr), stub_(0)
	{
		this->connected_memory_ = new std::list<IMemoryManager::MemoryBlockData*>;
		// Create a new reference 
		this->references_ = new ReferenceCounter();
		// Increment the references count
		this->references_->add_reference();

		this->size_ = new int(0);
	}
	SmartPointer(IMemoryManager* memory_manager) : memory_manager_(memory_manager), stub_(0)
	{
		this->connected_memory_ = new std::list<IMemoryManager::MemoryBlockData*>;
		// Create a new reference 
		this->references_ = new ReferenceCounter();
		// Increment the references count
		this->references_->add_reference();

		this->size_ = new int(0);
	}
	SmartPointer(const SmartPointer<Type>& other) :
		connected_memory_(other.connected_memory_), references_(other.references_), stub_(0)
	{
		this->memory_manager_ = other.memory_manager_;

		// Copy constructor
		// Copy the data and references pointer
		// and increment the references count

		this->size_ = other.size_;

		this->references_->add_reference();
	}
	~SmartPointer()
	{
		// Destructor
		// Decrement the references count
		// if references count becomex zero delete the data
		if (this->references_->release() == 0)
		{
			//delete this;
			this->freeMemory();
			delete this->references_;
			delete this->connected_memory_;
			delete this->size_;
		}
	}

private:
	IMemoryManager * memory_manager_;
	ReferenceCounter* references_;

	std::list<IMemoryManager::MemoryBlockData*>* connected_memory_;
	int* size_;

	Type stub_;
};

#endif