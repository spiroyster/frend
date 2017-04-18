#include "ppm.hpp"

#include "frend.hpp"

#include <iostream>
#include <fstream>

namespace frend
{

    namespace
    {
        std::string ReadTextLineIgnoringComment(std::ifstream& file)
        {
            std::string line;
            if ( std::getline(file, line, "\n") )
            {
                std::size_t isComment = line.find("#");
                if ( isComment != std::string::npos )
                    return line.substr( line.begin(), isComment );
            }
            return line;
        }
    }
    
    Image ReadPPM(const std::string& filenameAndPath)
    { 
        std::ifstream file(filenameAndPath.c_str());
        if ( file )
        {
            // First line should be magic
            std::string line = ReadTextLineIgnoringComment(file);

            if ( line == std::string("P3") )
            {
                // Next should be dimensions so tokenise

                // Next should be range maximum for the channel

                // And then the data

            }




        }






        FILE* ppmfile = fopen(filenameAndPath.c_str(), "r");
        if (ppmfile)
        {
            // First line should be magic
            ReadTextFileLineIgnoringComment(ppmfile);
            if (strncmp(buffer, "P3", 2) == 0)
            {
                // Next should be dimensions, so tokenise
                char* token = new char[10];
                token = strtok(ReadTextFileLineIgnoringComment(ppmfile), " ");
                for (unsigned int i = 0; i < 2; ++i)
                {
                    width_ ? height_ = atoi(token) : width_ = atoi(token);
                    token = strtok(NULL, " ");
                }
                delete token;

                // Next should be the range maximum for the channel.
                unsigned int imageChannelRange = atoi(ReadTextFileLineIgnoringComment(ppmfile));

                // And then the data...
                data_ = new unsigned char[width_ * height_ * 3];
                unsigned char* itr = data_;
                unsigned int gfgf = 0;
                while (ReadTextFileLineIgnoringComment(ppmfile))
                {
                    *itr = static_cast<unsigned char>(atoi(buffer));
                    ++itr;
                    ++gfgf;
                }
            }
            fclose(ppmfile);
        }
    }

    static std::string WritePPM(const Image& image)
    {
        
    }
}