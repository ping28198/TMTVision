#pragma once

#include <boost/archive/iterators/base64_from_binary.hpp>  
#include <boost/archive/iterators/binary_from_base64.hpp>  
#include <boost/archive/iterators/transform_width.hpp>  
#include <string>  
#include <iostream>  
#include <sstream>  
using namespace std;

class RelicHelper
{
public:
	RelicHelper();
	~RelicHelper();
    static bool Base64Encode(const std::string& input, std::string* output);
	static bool Base64Decode(const std::string& input, std::string* output);
private:

};
