#ifndef I_MEMORY_MANAGER_H
#define I_MEMORY_MANAGER_H

#include <list>

namespace memorymanager
{
	/// <summary>
	/// An interface, which gives opportunity to use different
	///  MemoryManager implementatioms (in this project there is only one implementation)
	/// </summary>
	class IMemoryManager
	{
	public:
		typedef char byte;

		struct MemoryBlockData
		{
			byte* offset_pointer;
			int byte_size;

			MemoryBlockData() : offset_pointer(nullptr), byte_size(0)
			{
			}

			bool operator==(const MemoryBlockData& second) const
			{
				return (offset_pointer == second.offset_pointer && byte_size == second.byte_size);
			}
		};

	public:
		virtual std::list<MemoryBlockData*> allocate(size_t size_in_bytes, size_t type_size) = 0;
		virtual void free(std::list<MemoryBlockData*>) = 0;
	};
}

#endif