#include "stdafx.h"
#include "CatmullRom.h"
#include <vector>


CatmullRom::CatmullRom(std::vector<physx::PxVec3> _pointsControle, std::vector<DirectX::XMVECTOR> _quatControle, int _nbPoints)
	: m_nbPoints(_nbPoints) 
{
	_quatControle.erase(_quatControle.begin());
	_quatControle.erase(_quatControle.end() - 1);
	for (size_t numPoint = 0; numPoint < _pointsControle.size(); numPoint++)
	{
		addPoint(_pointsControle.at(numPoint), *_quatControle.begin());
		if (numPoint >= 3)
			_quatControle.erase(_quatControle.begin());
	}
	m_listPoints.push_back(PointInfo(_pointsControle.at(_pointsControle.size() - 2), 0.f, _quatControle.back(), true));
	m_listPoints.back().setTime(0);
}

int CatmullRom::getNbPoints() noexcept
{
	return m_nbPoints;
}

physx::PxVec3 CatmullRom::computePoint(float t, physx::PxVec3 P0, physx::PxVec3 P1, physx::PxVec3 P2, physx::PxVec3 P3)
{

    physx::PxVec3 point;
	point = t * t * t * ((-1) * P0 + 3 * P1 - 3 * P2 + P3) / 2;
	point += t * t * (2 * P0 - 5 * P1 + 4 * P2 - P3) / 2;
	point += t * ((-1) * P0 + P2) / 2;
	point += P1;
	return point;
}

void CatmullRom::addPoint(physx::PxVec3 P, DirectX::XMVECTOR Q)
{
	m_quatControle.push_back(Q);
	m_pointsControle.push_back(P);

	if (m_pointsControle.size() < 4)
		return;

	const int newIndex = m_quatControle.size() - 1;
	const int currentIndex = newIndex - 2;
	float t;
	for (int i = 0; i <= m_nbPoints; i++)
	{
		t = static_cast<float>(i) / m_nbPoints;
		if (t == 0.f)
			m_listPoints.push_back(PointInfo{ computePoint(t
				, m_pointsControle[currentIndex - 1]
				, m_pointsControle[currentIndex]
				, m_pointsControle[currentIndex + 1]
				, m_pointsControle[currentIndex + 2]), t, Q, true });
		else
			m_listPoints.push_back(PointInfo(computePoint(t
				, m_pointsControle[currentIndex - 1]
				, m_pointsControle[currentIndex]
				, m_pointsControle[currentIndex + 1]
				, m_pointsControle[currentIndex + 2]), t));
	}
}

std::vector<PointInfo> CatmullRom::getListPoints() { return m_listPoints; }
