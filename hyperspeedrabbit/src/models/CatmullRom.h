#pragma once
#include "stdafx.h"
#include <pxshared\foundation\PxVec3.h>
#include <vector>
#include <DirectXMath.h>
#include "PointInfo.h"

class CatmullRom
{

private:

    std::vector<physx::PxVec3> m_pointsControle;
    std::vector<DirectX::XMVECTOR> m_quatControle;
    std::vector<PointInfo> m_listPoints;

    int m_nbPoints;

public:

    CatmullRom() = default;
    CatmullRom(std::vector<physx::PxVec3> _pointsControle, std::vector<DirectX::XMVECTOR> _quatControle, int _nbPoints);
    
    ~CatmullRom() = default;

    

    physx::PxVec3 computePoint(float temps, physx::PxVec3 p1, physx::PxVec3 p2, physx::PxVec3 p3, physx::PxVec3 p4);
    void addPoint(physx::PxVec3 P, DirectX::XMVECTOR Q);

    int getNbPoints() noexcept;
    std::vector<PointInfo> getListPoints();
};

