#pragma once
#include "Objet.h"
#include "ObjetMesh.h"
#include <vector>
#include "DispositifD3D11.h"
#include "Terrain.h"
#include "Courbe3D.h"
#include "Material.h"

class Objet3D : public Objet
{
public:
	Objet3D(DirectX::XMFLOAT3 _position = {}, DirectX::XMFLOAT4 _rotation = { 0.0f, 0.0f, 0.0f, 1.0f }, DirectX::XMFLOAT3 _scale = {1.f,1.f,1.f});

	virtual ~Objet3D() = default;
	
	void InitBuffers() noexcept override;
	void DrawShadowMap(const XMMATRIX) noexcept override;
	void Draw(ID3D11ShaderResourceView* _pShadowMapView, const DirectX::XMMATRIX, const DirectX::XMVECTOR) noexcept override;
	
	void update(float) override;
	bool nearPoint(const physx::PxVec3& _pt1, const physx::PxVec3& _pt2, const float _epsilon) const;
	void setNextPosition(physx::PxVec3 _pointSuivi) { m_nextPosition = _pointSuivi; }
	Courbe3D getCurve() const { return m_curve3D; }

	void addMaterial(std::shared_ptr<PM3D::Material>) noexcept;

	bool isFar() const noexcept;
protected:
	std::vector<std::unique_ptr<PM3D::CObjetMesh>> m_meshs;
	std::vector<std::unique_ptr<PM3D::CTerrain>> m_terrains;

	Courbe3D m_curve3D;
	XMFLOAT3 m_scale; // X, Y, Z
	physx::PxVec3 m_nextPosition;
};
