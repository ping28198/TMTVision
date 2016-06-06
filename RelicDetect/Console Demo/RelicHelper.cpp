#include "RelicHelper.h"

RelicHelper::RelicHelper()
{
}

RelicHelper::~RelicHelper()
{
}
bool RelicHelper::Base64Encode(const std::string& input, std::string* output) 
{
	typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8> > Base64EncodeIterator;
	std::stringstream result;
	std::copy(Base64EncodeIterator(input.begin()), Base64EncodeIterator(input.end()), std::ostream_iterator<char>(result));
	size_t equal_count = (3 - input.length() % 3) % 3;
	for (size_t i = 0; i < equal_count; i++) {
		result.put('=');
	}
	*output = result.str();
	return output->empty() == false;
}

bool RelicHelper::Base64Decode(const std::string& input, std::string* output) 
{
	typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;
	std::stringstream result;
	bool bPaded = false;
	int iLength = input.size();
	if (iLength && input[iLength - 1] == '=')
	{
		bPaded = true;
		--iLength;
		if (iLength && input[iLength - 1] == '=')
		{
			--iLength;
		}
	}
	if (iLength == 0)
	{
		return false;
	}

	if (bPaded)
	{
		--iLength;
	}

	try {
		std::copy(Base64DecodeIterator(input.begin()), Base64DecodeIterator(input.begin() + iLength), std::ostream_iterator<char>(result));
	}
	catch (...) {
		return false;
	}

	*output = result.str();
	return output->empty() == false;
}