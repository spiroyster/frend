
#ifndef PPMIMAGE_HPP
#define PPMIMAGE_HPP

#include <string>

namespace frend
{
    class Image;

    static Image ReadPPM(const std::string& filenameAndPath);
    static std::string WritePPM(const std::string& filenameAndPath, const Image& image);
}

#endif // PPMIMAGE_HPP