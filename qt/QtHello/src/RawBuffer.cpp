#include "RawBuffer.hpp"
#include <cstring>
#include <cstdlib>
#include <algorithm>

QtSample::RawBuffer::RawBuffer() : 
	m_Data(nullptr), m_Size(0)
{
}

QtSample::RawBuffer::RawBuffer(const uint8_t* data, size_t size) : 
	m_Data(new uint8_t[size]), m_Size(size)
{
	std::copy(data, data + size, m_Data);
}

QtSample::RawBuffer::~RawBuffer()
{
	if (m_Data != nullptr)
		delete[] m_Data;
}

void QtSample::RawBuffer::RecreateBuffer(const uint8_t* data, size_t size)
{
	if (m_Data != nullptr)
		delete[] m_Data;

	m_Size = size;
	m_Data = new uint8_t[m_Size];
	std::copy(data, data + size, m_Data);
}

const uint8_t* QtSample::RawBuffer::GetData() const 
{
	return m_Data;
}

const size_t QtSample::RawBuffer::GetSize() const
{
	return m_Size;
}

std::string QtSample::RawBuffer::ToString() const
{
	if (m_Data != nullptr)
		return std::string(reinterpret_cast<const char*>(m_Data), m_Size);
	return std::string("");
}
