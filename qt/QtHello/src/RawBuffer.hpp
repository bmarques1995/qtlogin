#pragma once
#include <cstdint>

namespace QtSample
{
	class RawBuffer
	{
	public:
		RawBuffer(const RawBuffer&) = delete;
		RawBuffer& operator=(const RawBuffer&) = delete;

		RawBuffer();
		RawBuffer(const uint8_t* data, size_t size);
		~RawBuffer();

		void RecreateBuffer(const uint8_t* data, size_t size);

		const uint8_t* GetData() const;
		const size_t GetSize() const;

	private:
		uint8_t* m_Data;
		size_t m_Size;
	};
}
