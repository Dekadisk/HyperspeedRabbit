#pragma once

namespace PM3D
{

class Chrono
{
public:
	Chrono();

	int64_t GetTimeCount() const noexcept;
	double GetSecPerCount() const noexcept { return m_SecPerCount; }
	// retourne le temps en millisecondes entre deux count.
	double GetTimeBetweenCounts(int64_t start, int64_t stop) const noexcept;

private:
	double m_SecPerCount;
};

} // namespace PM3D
