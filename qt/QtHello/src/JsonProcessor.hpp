#pragma once

#include <simdjson.h>

namespace QtSample
{
class JsonProcessor
{
	public: 
		JsonProcessor();
		~JsonProcessor();

		void ProcessJsonText(std::string_view json, simdjson::dom::element* jsonOutput);
		void ProcessJsonFile(std::string_view jsonpath, simdjson::dom::element* jsonOutput);
	private:
		simdjson::dom::parser m_Parser;
	};
}