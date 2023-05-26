#pragma once
#include "Camera.h"
#include "Objet.h"
#include "HyperShip.h"

namespace PM3D {

    class ThirdPersonCamera :
        public Camera
    {
        std::shared_ptr<HyperShip> m_hyperShip;
        HWND m_handler;

    public:

        ThirdPersonCamera(const DirectX::XMVECTOR& position_in
            , const DirectX::XMVECTOR& direction_in
            , const DirectX::XMVECTOR& up_in
            , DirectX::XMMATRIX* pMatView_in
            , DirectX::XMMATRIX* pMatProj_in
            , DirectX::XMMATRIX* pMatViewProj_in
            , std::shared_ptr<HyperShip> obj)
            : Camera(position_in
                , direction_in
                , up_in
                , pMatView_in
                , pMatProj_in
                , pMatViewProj_in)
        {
            m_handler = FindWindowA(nullptr, "PetitMoteur3D");
            m_hyperShip = obj;
        }
        void setHyperShip(std::shared_ptr<HyperShip> obj) noexcept { m_hyperShip = obj; }
        void updateSpecific(float tempsEcoule);
    };
}
