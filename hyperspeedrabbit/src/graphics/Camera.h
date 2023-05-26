#pragma once
#include "stdafx.h"
#include <vector>

namespace PM3D {
	class Camera
	{
	protected:
		DirectX::XMVECTOR m_position;
		DirectX::XMVECTOR m_direction;
		DirectX::XMVECTOR m_up;

		DirectX::XMFLOAT4 m_quat;

		DirectX::XMMATRIX m_rotation;
		DirectX::XMMATRIX* m_pMatView;
		DirectX::XMMATRIX* m_pMatProj;
		DirectX::XMMATRIX* m_pMatViewProj;

	public:
		bool waitForSwap = false;
		Camera() = default;
		Camera(const DirectX::XMVECTOR& position_in
			, const DirectX::XMVECTOR& direction_in
			, const DirectX::XMVECTOR& up_in
			, DirectX::XMMATRIX* pMatView_in
			, DirectX::XMMATRIX* pMatProj_in
			, DirectX::XMMATRIX* pMatViewProj_in);

		virtual ~Camera() = default;

		void init(const DirectX::XMVECTOR& position_in
			, const DirectX::XMVECTOR& direction_in
			, const DirectX::XMVECTOR& up_in
			, DirectX::XMMATRIX* pMatView_in
			, DirectX::XMMATRIX* pMatProj_in
			, DirectX::XMMATRIX* pMatViewProj_in);

		void updateQuat();

		void setPosition(const DirectX::XMVECTOR& position_in);
		void setDirection(const DirectX::XMVECTOR& direction_in);
		void setUp(const DirectX::XMVECTOR& up_in);

		DirectX::XMFLOAT4 getQuat() { return m_quat; }
		DirectX::XMVECTOR getPosition() const noexcept { return m_position; };
		DirectX::XMVECTOR getRotation() const noexcept;
		void update(float tempsEcoule = 0.0f);

		virtual void updateSpecific(float tempsEcoule) = 0;
		void updateMatrices();

		//getters
		DirectX::XMMATRIX* getMatView() { return m_pMatView; }
		DirectX::XMMATRIX* getMatProj() { return m_pMatProj; }
		DirectX::XMMATRIX* getMatViewProj() { return m_pMatViewProj; }
		
		
		// Legacy
		void updateLevel(float y, float tempsEcoule);
		
		//virtual void setObject(shared_ptr<Objet>);
	};
}