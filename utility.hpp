#ifndef UTILITIES_EXT
#define UTILITIES_EXT

#include <fstream>
#include <string>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <iomanip>

std::string readAll(const std::string &fileName)
{
    std::ifstream ifs;
    ifs.open(fileName);
    ifs.seekg(0, std::ios::end);
    size_t length = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::string buff(length, 0);
    ifs.read(&buff[0], length);
    ifs.close();

    return buff;
}

template<typename T>
std::list<T> first_n(const std::list<T> &in, std::size_t n) {
    return std::list<T> {in.begin(), std::next(in.begin(), std::min(in.size(), n))};
}

bool getparams_fromfile(std::string fileName, std::list<std::pair<std::string, std::string>>& mappair)
{
 
	// Open the File
	std::ifstream in(fileName.c_str());
 
	// Check if object is valid
	if(!in)
	{
		std::cerr << "Cannot open the File : "<<fileName<<std::endl;
		return false;
	}
 
	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if(str.size() > 0)
        {
            std::istringstream iss(str);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                              std::istream_iterator<std::string>());
            mappair.push_back(std::pair<std::string, std::string>(results[0], results[1]));
        }
	}
	//Close The File
	in.close();
	return true;
}



namespace Utility 
{
	
	std::string urlEncode(const std::string &url);
	std::string urlDecode(const std::string &encoded);

	std::string charToHex(unsigned char c);
	unsigned char hexToChar(const std::string &str);

	std::string urlEncode(const std::string &toEncode) 
	{
		std::ostringstream out;

		for(std::string::size_type i=0; i < toEncode.length(); ++i) 
		{
			short t = toEncode.at(i);

			if( t == 45 || // hyphen
				(t >= 48 && t <= 57) || // 0-9
				(t >= 65 && t <= 90) || // A-Z
				t == 95 || // underscore
				(t >= 97 && t <= 122) || // a-z
				t == 126 // tilde
				)
			{
				out << toEncode.at(i);
			}
			else 
			{	
				out << charToHex(toEncode.at(i));
			}	
		}

		return out.str();
	}

	std::string urlDecode(const std::string &toDecode) 
	{
		std::ostringstream out;

		for(std::string::size_type i=0; i < toDecode.length(); ++i) 
		{
			if(toDecode.at(i) == '%') 
			{
				std::string str(toDecode.substr(i+1, 2));
				out << hexToChar(str);
				i += 2;
			}
			else
			{
				out << toDecode.at(i);
			}
		}

		return out.str();
	}

	std::string charToHex(unsigned char c) 
	{
		short i = c;
		std::stringstream s;
		s << "%" << std::setw(2) << std::setfill('0') << std::hex << i;
		return s.str();
	}

	unsigned char hexToChar(const std::string &str) 
	{
		short c = 0;

		if(!str.empty()) 
		{
			std::istringstream in(str);
			in >> std::hex >> c;
			if(in.fail()) 
			{
				throw std::runtime_error("stream decode failure");
			}
		}
		return static_cast<unsigned char>(c);
	}
}
#endif