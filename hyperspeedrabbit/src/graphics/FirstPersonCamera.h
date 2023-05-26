#pragma once
#include "Camera.h"
#include "Objet3DPhysique.h"

namespace PM3D {

    class FirstPersonCamera :
        public Camera
    {
        std::shared_ptr<Objet3DPhysique> m_object;

    public:

        FirstPersonCamera(const DirectX::XMVECTOR& position_in
            , const DirectX::XMVECTOR& direction_in
            , const DirectX::XMVECTOR& up_in
            , DirectX::XMMATRIX* pMatView_in
            , DirectX::XMMATRIX* pMatProj_in
            , DirectX::XMMATRIX* pMatViewProj_in
            , std::shared_ptr<Objet3DPhysique> _obj)
            : Camera(position_in
                , direction_in
                , up_in
                , pMatView_in
                , pMatProj_in
                , pMatViewProj_in)
            , m_object(_obj) {}
        void setObject(std::shared_ptr<Objet3DPhysique> _obj) noexcept { m_object = _obj; }
        void updateSpecific(float tempsEcoule);
    };
}