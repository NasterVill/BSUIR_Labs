#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "IMemoryManager.h"
#include "MemoryAllocationException.h"
#include <fstream>
#include <cstring>
#include <list>
#include <vector>
#include <iostream>

namespace memorymanager
{
	/// <summary>
	/// MemoryManager, is a class, which emulates work wtih memory managment, 
	/// by distributing free space from previously allocated byte* data_pool_
	/// among the requsting objects
	/// </summary>
	class MemoryManager : public IMemoryManager
	{
	private:
		static const int MAX_DATA_SIZE_IN_BYTES;
		static const std::string disk_file_name_;

	public:
		std::list<MemoryBlockData*> allocate(size_t size_in_bytes, size_t type_size) override;
		void free(std::list<IMemoryManager::MemoryBlockData*> blocks_to_delete) override;
		void DefragmentateFreeMemory();

		MemoryManager();
		~MemoryManager();

	private:
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;

		void eraseBlock(MemoryBlockData* block);

	private:
		void* data_pool_;
		std::list<MemoryBlockData*> free_memory_blocks_;
		std::list<MemoryBlockData*> allocated_memory_blocks_;
		int distributed_space_;
	};
}

#endif
