#pragma once
#include <pxshared\foundation\PxVec3.h>
#include <fstream>

class PointInfo {

    physx::PxVec3 m_position;
    float m_time{};
    DirectX::XMVECTOR m_rotation;
    bool m_hasRotation{ false };

public:

    PointInfo(physx::PxVec3 _position = {}, float _time = -1.f, DirectX::XMVECTOR _rotation = {}, bool _hasRotation = false);

    void setRotation(DirectX::XMVECTOR _rotation) noexcept;

    DirectX::XMVECTOR getRotation() const noexcept;

    void setPosition(physx::PxVec3 _position);

    physx::PxVec3 getPosition() const;

    bool hasRotation() noexcept;

    void setTime(float _time) noexcept;

    float getTime() noexcept;

    bool operator==(const PointInfo& other) const;

    friend std::istream& operator>>(std::istream& is, PointInfo& pt);
    friend std::ostream& operator<<(std::ostream& os, const PointInfo& pt);
};