#ifndef IASYNC_READ_WRITE_H
#define IASYNC_READ_WRITE_H

#include <string>

class IAsyncReadWrite
{
public:
	virtual int ConcatFiles(std::string source_folder_path, std::string output_file_path) = 0; 
	virtual void Release() = 0;
	virtual ~IAsyncReadWrite();
};

#endif
