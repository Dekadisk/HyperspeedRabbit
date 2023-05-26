#pragma once
#include "stdafx.h"
#include "CatmullRom.h"
#include <vector>
#include <PxVec3.h>

class Courbe3D
{
private:
	std::vector<physx::PxVec3> m_controlPoints;
	std::vector<PointInfo> m_fullCurve;
	std::vector<DirectX::XMVECTOR> m_controlQuats;
	int m_nbPoints;

public:
	Courbe3D() = default;
	std::vector<PointInfo> getCurve() const { return m_fullCurve; }

	Courbe3D(std::vector<physx::PxVec3> _points, std::vector<DirectX::XMVECTOR> _quats);
	
	std::vector<physx::PxVec3> getPoints() const { return m_controlPoints; }

	void setNbPoints(int _val) noexcept { m_nbPoints = _val; }
	int getNbPoints() noexcept { return m_nbPoints; }
};