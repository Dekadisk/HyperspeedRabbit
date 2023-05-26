#include "stdafx.h"
#include "ResourceManager.h"
#include "util.h"
#include "Resource.h"
#include <algorithm>


ResourceManager::~ResourceManager()
{
	std::for_each(std::begin(m_blobs_map), std::end(m_blobs_map),
		[](std::pair<std::wstring,ID3DBlob*> blob) 
		{
			blob.second->Release();
		});
}

PM3D::CChargeurOBJ& ResourceManager::getChargeur(const std::string& _nomChemin,const std::string& _nomFichier, bool _mainGauche, bool _inverserCulling )
{
	auto finder = m_chargeurs_map.find(_nomChemin + _nomFichier);
	if (finder != m_chargeurs_map.end())
	{
		return finder->second;
	}
	else {
		PM3D::CParametresChargement paramChargement{};
		paramChargement.NomChemin = _nomChemin;
		paramChargement.NomFichier = _nomFichier;
		paramChargement.bInverserCulling = _inverserCulling;
		paramChargement.bMainGauche = _mainGauche;
		m_chargeurs_map[_nomChemin + _nomFichier] = PM3D::CChargeurOBJ();
		m_chargeurs_map[_nomChemin + _nomFichier].Chargement(paramChargement);
		return m_chargeurs_map[_nomChemin + _nomFichier];
	}
}

ID3DBlob* ResourceManager::getEffect(const std::wstring& _nomChemin)
{
	auto finder = m_blobs_map.find(_nomChemin);
	if (finder != m_blobs_map.end())
	{
		return finder->second;
	}
	else {
		ID3DBlob* pFXBlob = nullptr;
		PM3D::DXEssayer(D3DCompileFromFile(_nomChemin.c_str(), 0, 0, 0,
			"fx_5_0", 0, 0, &pFXBlob, 0),
			DXE_ERREURCREATION_FX);
		m_blobs_map[_nomChemin] = pFXBlob;
		return pFXBlob;
	}
}
