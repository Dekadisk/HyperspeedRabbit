#pragma once
#include "ObjDecoration.h"
#include "MeshSkybox.h"
#include "HyperShip.h"
class Skybox : public Objet
{
public:
	Skybox(PM3D::CDispositifD3D11* _pDispositif, std::shared_ptr<HyperShip> _hypership, const char* _directory, const char* _filename);

	void anime(float) override;
	// Hérité via ObjDecoration
	virtual void update(float) override;

	// Hérité via Objet
	void InitBuffers() noexcept override;
	void DrawShadowMap(const DirectX::XMMATRIX) noexcept override;
	void Draw(ID3D11ShaderResourceView* _pShadowMapView, const DirectX::XMMATRIX, const DirectX::XMVECTOR) noexcept override;
private:
	std::unique_ptr<PM3D::CMeshSkybox> m_skybox;
	std::shared_ptr<HyperShip> m_hypership;
};

