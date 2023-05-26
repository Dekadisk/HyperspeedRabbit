#pragma once
#include "Singleton.h"
#include "dispositif.h" 

#include <vector>
#include "Objet3D.h"
#include "Terrain.h"
#include "Camera.h"
#include "InputManager.h" 
#include "GameManager.h"
#include "GestionnaireDeTextures.h"
#include "LumierePoint.h"
#include "AfficheurTexte.h"
#include <memory>
#include <iostream>

#include <fstream>

namespace PM3D
{
template <class T, class TClasseDispositif> class CMoteur : public Singleton<T>
{

private:
	double m_stepTime = 1.0 / 60;

public:
	virtual void run()
	{
		bool bBoucle = true;

		while (bBoucle)
		{
			bBoucle = runSpecific();

			if (bBoucle)
				bBoucle = animation();
		}
	}

	virtual bool Initialisations()
	{
		if (!initSpecific())
			return false;

		m_pDispositif = creationDispositifSpecific(CDS_FENETRE); // CDS_FENETRE // CDS_PLEIN_ECRAN
		CAfficheurTexte::Init();

		if (!preInitGame())
			return false;

		if (!initGame())
			return false;

		// Initialisation des paramètres de l'animation et préparation de la première image
		if (!initAnimation())
			return false;

		return true;
	}

	virtual bool animation()
	{
		const int64_t TempsCompteurCourant = getTimeSpecific();
		const double TempsEcoule = getTimeIntervalsInSec(m_prevTime, TempsCompteurCourant);

		if (TempsEcoule > m_stepTime) // Ready to render
		{
			m_pDispositif->Present();

			// Prepare next frame
			animeScene(static_cast<float>(TempsEcoule));

			// Render current frame
			RenderScene();

			m_prevTime = TempsCompteurCourant;
		}
		return true;
	}

	const XMMATRIX& GetMatView() const { return m_matView; }
	const XMMATRIX& GetMatProj() const { return m_matProj; }
	const XMMATRIX& GetMatViewProj() const { return m_matViewProj; }

	CGestionnaireDeTextures& GetTextureManager() { return TexturesManager; }
	void addLight(std::shared_ptr<CLumierePoint>light) { m_lights.push_back(std::move(light)); }
	std::vector<std::shared_ptr<CLumierePoint>>& getLights() { return m_lights; };

protected:
	virtual ~CMoteur()
	{
		cleanUp();
	}

	virtual bool runSpecific() = 0;
	virtual bool initSpecific() = 0;

	virtual int64_t getTimeSpecific() const = 0;
	virtual double getTimeIntervalsInSec(int64_t start, int64_t stop) const = 0;

	virtual TClasseDispositif* creationDispositifSpecific(const CDS_MODE cdsMode) = 0;
	virtual void beginRenderSceneSpecific() = 0;
	virtual void endRenderSceneSpecific() = 0;

	virtual bool initAnimation()
	{
		m_nextTime = getTimeSpecific();
		m_prevTime = m_nextTime;

		// First frame
		if (!RenderScene())
			return false;
		return true;
	}

	// Fonctions de rendu et de présentation de la scène
	virtual bool RenderScene()
	{


		beginRenderSceneSpecific();


		GameManager::get().renderScene();


		endRenderSceneSpecific();
		
		
		return true;
	}

	virtual void cleanUp() noexcept
	{
		// détruire les objets
		GameManager::get().cleanUp();
		CAfficheurTexte::Close();

		// Détruire le dispositif
		if (m_pDispositif)
		{
			delete m_pDispositif;
			m_pDispositif = nullptr;
		}
	}

	virtual bool preInitGame()
	{
		const float champDeVision = XM_PI / 4; 	// 45 degrés
		const float ratioDAspect = static_cast<float>(m_pDispositif->GetLargeur()) / static_cast<float>(m_pDispositif->GetHauteur());
		const float planRapproche = 2.0f;
		const float planEloigne = 800000.0f;

		m_matView = XMMatrixLookAtLH(XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

		m_matProj = XMMatrixPerspectiveFovLH(champDeVision, ratioDAspect, planRapproche, planEloigne);

		m_matViewProj = m_matView * m_matProj;

		if (!GameManager::get().preInitScenes(m_pDispositif, &m_matProj, &m_matView, &m_matViewProj))
			return false;

		RenderScene();
		m_pDispositif->Present();
		return true;
	}

	virtual bool initGame()
	{
		return GameManager::get().initScenes();
	}

	bool animeScene(float tempsEcoule)
	{
		// Shortcuts
		InputManager::get().acquireKeys();
		InputManager::get().acquireMouse();
		InputManager::get().updateKeyStates();
		InputManager::get().updateMouseState();

		GameManager::get().animeScene(tempsEcoule);

		return true;
	}

protected:
	// Variables pour le temps de l'animation
	int64_t m_nextTime;
	int64_t m_prevTime;

	// Le dispositif de rendu
	TClasseDispositif* m_pDispositif;

	// Les matrices
	XMMATRIX m_matView;
	XMMATRIX m_matProj;
	XMMATRIX m_matViewProj;

	// Le gestionnaire de texture 
	CGestionnaireDeTextures TexturesManager;

	//le gestionnaire des matérieux

	// Lumières
	std::vector<std::shared_ptr<CLumierePoint>> m_lights;
};
} // namespace PM3D
