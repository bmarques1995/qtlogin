#include "JsonProcessor.hpp"

QtSample::JsonProcessor::JsonProcessor()
{
}

QtSample::JsonProcessor::~JsonProcessor()
{
}

void QtSample::JsonProcessor::ProcessJsonText(std::string_view json, simdjson::dom::element* jsonOutput)
{
	*jsonOutput = m_Parser.parse(json);
}

void QtSample::JsonProcessor::ProcessJsonFile(std::string_view jsonpath, simdjson::dom::element* jsonOutput)
{
	*jsonOutput = m_Parser.load(jsonpath);
}
