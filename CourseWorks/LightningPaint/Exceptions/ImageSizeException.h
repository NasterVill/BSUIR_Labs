#ifndef IMAGESIZEEXCEPTION_H
#define IMAGESIZEEXCEPTION_H

#include <stdexcept>

using namespace std;

namespace Draw
{

class ImageSizeException : public domain_error
{
public:
    explicit ImageSizeException(const char* message);
    ~ImageSizeException();
};

} // namespace Draw

#endif // IMAGESIZEEXCEPTION_H
