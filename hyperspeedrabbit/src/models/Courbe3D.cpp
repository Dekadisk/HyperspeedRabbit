#include "stdafx.h"
#include "Courbe3D.h"
#include "BezierJ.h"
#include <algorithm>

using namespace physx;

Courbe3D::Courbe3D(std::vector<physx::PxVec3> _points, std::vector<DirectX::XMVECTOR> _quats) : m_controlPoints{ _points }, m_controlQuats{ _quats }
{	
	CatmullRom catmullrom(m_controlPoints, m_controlQuats, 30);

	m_fullCurve = catmullrom.getListPoints();
	m_nbPoints = catmullrom.getNbPoints();

	auto last = std::unique(m_fullCurve.begin(), m_fullCurve.end());
	m_fullCurve.erase(last, m_fullCurve.end());

	std::reverse(m_fullCurve.begin(), m_fullCurve.end());
}
