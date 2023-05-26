
#include "stdafx.h"
#include "Chrono.h"

namespace PM3D
{

Chrono::Chrono()
{
	LARGE_INTEGER counterFrequency;
	::QueryPerformanceFrequency(&counterFrequency);
	m_SecPerCount = 1.0 / static_cast<double>(counterFrequency.QuadPart);
}

int64_t Chrono::GetTimeCount() const noexcept
{
	LARGE_INTEGER countNumber;
	::QueryPerformanceCounter(&countNumber);
	return countNumber.QuadPart;
}

double Chrono::GetTimeBetweenCounts(int64_t start, int64_t stop) const noexcept
{
	return static_cast<double>(stop - start) * m_SecPerCount;
}
} // namespace PM3D
