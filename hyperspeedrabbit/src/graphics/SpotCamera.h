#pragma once
#include "Camera.h"
#include "Objet3DPhysique.h"

namespace PM3D {

    class SpotCamera : public Camera
    {
    public:

        SpotCamera(const DirectX::XMVECTOR& position_in
            , const DirectX::XMVECTOR& direction_in
            , const DirectX::XMVECTOR& up_in
            , DirectX::XMMATRIX* pMatView_in
            , DirectX::XMMATRIX* pMatProj_in
            , DirectX::XMMATRIX* pMatViewProj_in)
            : Camera(position_in
                , direction_in
                , up_in
                , pMatView_in
                , pMatProj_in
                , pMatViewProj_in)
            {}
        void updateSpecific(float tempsEcoule) override;
    };
}