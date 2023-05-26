#pragma once
#include "stdafx.h"
#include "ChargeurOBJ.h"
#include <unordered_map>
#include <string>
#include <memory>
#include "ObjetMesh.h"
class ResourceManager
{
private:
	std::unordered_map<std::string, PM3D::CChargeurOBJ>  m_chargeurs_map;
	std::unordered_map<std::wstring, ID3DBlob*>  m_blobs_map;

private:
	ResourceManager() = default;

public:
	ResourceManager(ResourceManager&) = delete;
	~ResourceManager();

public:
	ResourceManager& operator=(ResourceManager&) = delete;

public:
	static ResourceManager& get() noexcept {
		static ResourceManager gameManager;
		return gameManager;
	}

	PM3D::CChargeurOBJ& getChargeur(const std::string& _nomChemin, const std::string& _nomFichier, bool _mainGauche = true, bool _inverserCulling = false);
	ID3DBlob* getEffect(const std::wstring& _nomChemin);

};

