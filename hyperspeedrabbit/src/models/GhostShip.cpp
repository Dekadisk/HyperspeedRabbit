#include "stdafx.h"
#include "GhostShip.h"
#include <fstream>
#include "ResourceManager.h"
#include "src/Converter.h"

GhostShip::GhostShip(PM3D::CDispositifD3D11* _pDispositif) : Objet3D({}, { 0.0f, 0.0f, 0.0f, 1.0f } , {10.f,10.f ,10.f }) {
	m_pDispositif = _pDispositif;

	//add mesh
	PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur("assets/modeles/Ships/", "PointyShip2.obj");
	m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(chargeurObj, m_pDispositif, "assets/modeles/Ships/", "PointyShip2"));

}

void GhostShip::anime(float) {
	if (!m_curve.empty()) {
		
		m_position = caster<XMFLOAT3>(m_curve.back().getPosition());
		m_rotation = caster<XMFLOAT4>(m_curve.back().getRotation());
		m_curve.pop_back();
	}
}

void GhostShip::loadCurve(const std::string& _filename) {
	m_curve.clear();
	std::ifstream is{ _filename };
	std::copy(std::istream_iterator<PointInfo> {is}, std::istream_iterator<PointInfo> {}, std::back_inserter(m_curve));

	// Derniers elements du fichier sont les premiers à lire. On place vers la fin du vector pour faciliter les suppressions (pop_back)
	std::reverse(std::begin(m_curve), std::end(m_curve));
}