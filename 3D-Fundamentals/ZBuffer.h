#pragma once
#include <cassert>
#include <limits>
#include <algorithm>
class ZBuffer {
public:
	ZBuffer(int h, int w) 
		: 
		height(h), 
		width(w), 
		pBuffer(new float[width * height])
	{}
	~ZBuffer() {
		delete[] pBuffer;
		pBuffer = nullptr;
	}
	ZBuffer(const ZBuffer&) = delete;
	ZBuffer& operator=(const ZBuffer&) = delete;
	ZBuffer(const ZBuffer&&) = delete;
	ZBuffer& operator=(const ZBuffer&&) = delete;
	void Clear()
	{
		const int nDepths = width * height;
		for (int i = 0; i < nDepths; i++)
		{
			pBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	float& At(int x, int y) {
		assert(x >= 0 && x < width);
		assert(y >= 0 && y < height);
		return pBuffer[(y * width) + x];
	}
	const float& At(int x, int y) const
	{
		return const_cast<ZBuffer*>(this)->At(x, y);
	}
	bool TestAndSet(int x, int y, float depth)
	{
		float& depthInBuffer = At(x, y);
		if (depth < depthInBuffer)
		{
			depthInBuffer = depth;
			return true;
		}
		return false;
	}
	int GetWidth() const
	{
		return width;
	}
	int GetHeight() const
	{
		return height;
	}
	auto GetMinMax() const
	{
		return std::minmax_element(pBuffer, pBuffer + width * height);
	}
private:
	const int height;
	const int width;
	float* pBuffer;
};