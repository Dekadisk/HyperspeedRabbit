#include "stdafx.h"
#include "PointInfo.h"
#include "Objet.h"
#include <src\Converter.h>



PointInfo::PointInfo(physx::PxVec3 _position, float _time, DirectX::XMVECTOR _rotation, bool _hasRotation) :
    m_position(_position),
    m_time(_time),
    m_rotation(_rotation),
    m_hasRotation(_hasRotation) {}

void PointInfo::setRotation(DirectX::XMVECTOR _rotation) noexcept {
    m_rotation = _rotation;
    m_hasRotation = true;
}

DirectX::XMVECTOR PointInfo::getRotation() const noexcept {
    return m_rotation;
}

void PointInfo::setPosition(physx::PxVec3 _position) {
    m_position = _position;
}

physx::PxVec3 PointInfo::getPosition() const {
    return m_position;
}

bool PointInfo::hasRotation() noexcept {
    return m_hasRotation;
}
void PointInfo::setTime(float _time) noexcept {
    m_time = _time;
}

float PointInfo::getTime() noexcept {
    return m_time;
}

bool PointInfo::operator==(const PointInfo& other) const
{
    return((m_position - other.m_position).magnitude() <= 1e-2 && m_hasRotation == other.m_hasRotation && abs(m_time - other.m_time) < 1e-2);
}

std::istream& operator>>(std::istream& is, PointInfo& pt) {
    if (!is) return is;
    float x_pos, y_pos, z_pos, x_rot, y_rot, z_rot, w_rot;
    if (is >> x_pos >> y_pos >> z_pos >> x_rot >> y_rot >> z_rot >> w_rot)
        pt = PointInfo{ {x_pos, y_pos, z_pos}, -1.0f, {x_rot,y_rot,z_rot,w_rot} };
    return is;
}

std::ostream& operator<<(std::ostream& os, const PointInfo& pt) {
    DirectX::XMFLOAT4 rot = caster<DirectX::XMFLOAT4>(pt.getRotation());
    return os << pt.getPosition().x << " " << pt.getPosition().y << " " << pt.getPosition().z << " " << rot.x << " " << rot.y << " " << rot.z << " " << rot.w << " ";
}