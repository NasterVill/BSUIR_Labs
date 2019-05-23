#include "MemoryManager.h"
#include <windows.h>

using namespace std;
using namespace memorymanager;

const int MemoryManager::MAX_DATA_SIZE_IN_BYTES = 10240;
const string MemoryManager::disk_file_name_ = "disk";


/// <summary>
/// This method allocates memory blocks to the requesting object
/// </summary>
/// <param name="size_in_bytes">
/// Requesting memory size in bytes
/// </param>
/// <param name="type_size">
/// The size of requesting object's type in bytes
/// </param>
/// <returns>
/// Returns a list of allocated memory blocks.
/// </returns>
list<IMemoryManager::MemoryBlockData*> MemoryManager::allocate(size_t size_in_bytes, size_t type_size)
{
	// checking if there is enough memory space left
	if ((this->distributed_space_ + size_in_bytes) > this->MAX_DATA_SIZE_IN_BYTES)
		throw MemoryAllocationException("Not enough memory space!");

	// a lsit of memory blocks, which will be returbed to a calling SmartPointer
	list<MemoryBlockData*> allocated_list;

	int unallocated_size = size_in_bytes;

	std::vector<std::list<MemoryBlockData*>::iterator> delete_iterators;
	std::list<MemoryBlockData*>::iterator iterator;

	// checking all free memory blocks
	for (iterator = this->free_memory_blocks_.begin(); iterator != this->free_memory_blocks_.end(); iterator++)
	{
		// if block's size is less than size of requsting object's type
		if ((*iterator)->byte_size < type_size)
			continue;


		this->allocated_memory_blocks_.push_back(new MemoryBlockData);
		this->allocated_memory_blocks_.back()->offset_pointer = (*iterator)->offset_pointer;

		allocated_list.push_back(this->allocated_memory_blocks_.back());

		// calculating a modulo, in order to allocate as many bytes, as it is requested,
		// not more (so 2 ints will have 8 bytes, even if the suited block has size of 9 bytes)
		int modulo = (*iterator)->byte_size % type_size;
		int iteration_available_size = (*iterator)->byte_size - modulo;

		// iteration_available_size -- part of block's size, multiple to type_size
		if (iteration_available_size > unallocated_size)
		{
			this->allocated_memory_blocks_.back()->byte_size = unallocated_size;

			// cutting the free block, if it is bigger, than needed
			(*iterator)->offset_pointer += unallocated_size;
			(*iterator)->byte_size -= unallocated_size;

			this->distributed_space_ += unallocated_size;

			unallocated_size = 0;

			break;
		}
		else
		{
			if (iteration_available_size == (*iterator)->byte_size)
			{
				// push the iterator of a block to delete
				delete_iterators.push_back(iterator);
			}
			else
			{
				// cutting the free block, if it is bigger, than needed
				(*iterator)->offset_pointer += iteration_available_size;
				(*iterator)->byte_size -= iteration_available_size;
			}

			this->distributed_space_ += iteration_available_size;

			this->allocated_memory_blocks_.back()->byte_size = iteration_available_size;
			unallocated_size -= iteration_available_size;

			if (unallocated_size == 0)
				break;
		}
	}

	if (allocated_list.empty() || unallocated_size != 0)
	{ 
		if (!allocated_list.empty())
		{
			for (auto& block : allocated_list)
			{
				// if we've reached this point, and some memory is still unalocated
				// that means, that there is free memory, but all the blocks, that are left
				// are smaller, than needed (example : required int, all the blocks have size of 3 bytes).
				// Removing all allocated blocks, and repeat, right after free memory defragmentation
				this->allocated_memory_blocks_.remove(block);
			}
		}
		this->DefragmentateFreeMemory();
		return this->allocate(size_in_bytes, type_size);
	}

	// delete blocks form free list
	for (auto& iterator_to_delete : delete_iterators)
	{
		this->free_memory_blocks_.erase(iterator_to_delete);
	}

	return allocated_list;
}

/// <summary>
/// This method deletes all the given blocks form allocated blocks list, and moves them to free blocks list
/// </summary>
/// <param name="blocks_to_delete">
/// A list of blocks, which were previously allocated by MemoryManager
/// </param>
void MemoryManager::free(list<IMemoryManager::MemoryBlockData*> blocks_to_delete)
{
	for (auto& deleting_block : blocks_to_delete)
	{
		eraseBlock(deleting_block);
		this->free_memory_blocks_.push_front(deleting_block);
		this->allocated_memory_blocks_.remove(deleting_block);
		this->distributed_space_ -= deleting_block->byte_size;
	}
}

/// <summary>
/// Clears the given block, by setting it's data to zero (for each byte)
/// </summary>
/// <param name="block">
/// A pointer to the block to clear
/// </param>
void MemoryManager::eraseBlock(MemoryBlockData* block)
{
	for (int i = 0; i < block->byte_size; i++)
	{
		block->offset_pointer[i] = 0;
	}
}

/// <summary>
/// Defragmnetates free memory by flushing all th data on disk, and then restoring it.
/// Thus all the free blocks transform into one big free block, which follows the area
/// of a data pool, fully filled with data (no free space).
/// It is needed to do so to prevent a situation, when all the free blocks have a small size (3 and less bytes for example)
/// which is smaller than a requested type_size. The memory won't be allocated in this case, even though were might be
/// enough free space in bytes (but it is fragmentated). Defragmentation of free memory solves this problem.
/// </summary>
void MemoryManager::DefragmentateFreeMemory()
{
	// flush all the data to file (disk)
	ofstream ostream(this->disk_file_name_, ios::out | ios::trunc | ios::binary);
	for (auto& block : this->allocated_memory_blocks_)
	{
		ostream.write(block->offset_pointer, block->byte_size);
	}
	ostream.close();

	// clear the memory
	this->free_memory_blocks_.clear();
	SecureZeroMemory(this->data_pool_, this->MAX_DATA_SIZE_IN_BYTES);

	// restore all the data from disk, and update all allocated blocks' content
	byte* temp_ptr = reinterpret_cast<byte*>(this->data_pool_);
	ifstream istream(this->disk_file_name_, ios::in | ios::binary);
	for (auto& block : this->allocated_memory_blocks_)
	{
		istream.read(temp_ptr, block->byte_size);
		block->offset_pointer = temp_ptr;
		temp_ptr += block->byte_size;
	}
	istream.close();

	// save one "big" free block
	this->free_memory_blocks_.push_front(new MemoryBlockData);
	this->free_memory_blocks_.front()->offset_pointer = temp_ptr;
	this->free_memory_blocks_.front()->byte_size = this->MAX_DATA_SIZE_IN_BYTES - this->distributed_space_;
}

MemoryManager::MemoryManager()
{
	this->distributed_space_ = 0;
	this->data_pool_ = new byte[this->MAX_DATA_SIZE_IN_BYTES];

	this->free_memory_blocks_.push_front(new MemoryBlockData);
	this->free_memory_blocks_.front()->offset_pointer = reinterpret_cast<byte*>(this->data_pool_);
	this->free_memory_blocks_.front()->byte_size = this->MAX_DATA_SIZE_IN_BYTES;

	SecureZeroMemory(this->data_pool_, this->MAX_DATA_SIZE_IN_BYTES);
}

MemoryManager::~MemoryManager()
{
	for (auto& block : this->free_memory_blocks_)
	{
		delete block;
	}
	for (auto& block : this->allocated_memory_blocks_)
	{
		delete block;
	}

	delete this->data_pool_;
}
