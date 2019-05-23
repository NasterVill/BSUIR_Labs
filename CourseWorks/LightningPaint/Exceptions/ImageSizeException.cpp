#include "ImageSizeException.h"

namespace Draw
{

ImageSizeException::ImageSizeException(const char* message) : domain_error(message)
{
}

ImageSizeException::~ImageSizeException()
{
}

} // namespace Draw
