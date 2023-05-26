#pragma once
#include "Objet.h"
#include <vector>
#include <memory>
#include "AfficheurSprite.h"
#include "AfficheurTexte.h"
#include "DispositifD3D11.h"

class Objet2D : public Objet
{
public:
	Objet2D(PM3D::CDispositifD3D11* _pDispositif);
	virtual ~Objet2D() = default;
	virtual void anime(float);
	virtual void update(float);
	virtual void draw(DirectX::XMVECTOR _camPosition);
	void InitBuffers() noexcept {}
	void DrawShadowMap(const DirectX::XMMATRIX) noexcept {}
	void Draw(ID3D11ShaderResourceView* _pShadowMapView, const DirectX::XMMATRIX, const DirectX::XMVECTOR) noexcept {}

protected:
	std::unique_ptr<PM3D::CAfficheurSprite> m_afficheurSprite;
	std::unique_ptr<PM3D::CAfficheurTexte> m_afficheurTexte;

	std::vector<std::unique_ptr<Gdiplus::Font>> m_polices;
};