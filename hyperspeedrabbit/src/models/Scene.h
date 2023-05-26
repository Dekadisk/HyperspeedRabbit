#pragma once
#include "stdafx.h"

#include "PxContactModifyCallback.h"
#include "PxSimulationEventCallback.h"
#include "PxRigidActor.h"
#include "PxPhysicsAPI.h"
#include "PxFoundation.h"
#include "PxPhysicsVersion.h"
#include "SnippetPrint.h"
#include "SnippetPVD.h"
#include "SnippetUtils.h"

#include "DispositifD3D11.h"

#include "Objet.h"
#include "Terrain.h"
#include "Camera.h"
#include "HyperShip.h"
#include "LumierePoint.h"
#include "UI.h"
#include "LevelCreator.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Missile.h"
#include "ShadowEffect.h"
#include "Collectable.h"
#include "Obstacle.h"
#include "GhostShip.h"


#include "SoundManager.h"

#include "PanneauPE.h"

#include <vector>
#include <chrono>


class Scene : public physx::PxContactModifyCallback, public physx::PxSimulationEventCallback
{
private:

	using collectable_value = std::shared_ptr<Collectable>;
	using obstacle_value = std::shared_ptr<Obstacle>;
	using missile_value = std::shared_ptr<Missile>;


	DirectX::XMMATRIX* m_matView;
	DirectX::XMMATRIX* m_matProj;
	DirectX::XMMATRIX* m_matViewProj;

	std::unique_ptr<PM3D::ShadowEffect> m_shadowEffect;
	std::shared_ptr<HyperShip> m_hyperShip;
	std::shared_ptr<GhostShip> m_ghostShip;
	std::vector<std::shared_ptr<Objet>> m_objectList;

	std::vector<collectable_value> m_collectableList;
	std::vector<obstacle_value> m_obstacleList;

	std::vector<collectable_value> m_collectableTrash;
	std::vector<obstacle_value> m_obstacleTrash;

	std::vector<missile_value> m_missileList;

	std::vector<std::shared_ptr<PM3D::Camera>> m_cameraList;
	std::unique_ptr<UI> m_ui;
	//Random tool
	physx::PxTransform m_graveyard = {666000666.f,66600666.f,666000666.f};

	std::unique_ptr<PM3D::PanneauPE> m_pPanneau;

	//Level
	LevelCreator m_level;

	// Graphic
	PM3D::CDispositifD3D11* m_pDispositif;

	// PhysX
	physx::PxPhysics* m_physics = NULL;
	physx::PxScene* m_scenePhysic = NULL;
	physx::PxMaterial* m_material = NULL;

	physx::PxDefaultAllocator		m_allocator;
	physx::PxDefaultErrorCallback	m_errorCallback;

	physx::PxFoundation* m_foundation = NULL;
	physx::PxDefaultCpuDispatcher* m_dispatcher = NULL;

	physx::PxPvd* m_pvd = NULL;

	// Sound
	std::unique_ptr<SoundManager> m_soundManager;
	int m_currentCameraIndex = 0;
	bool m_waitForSwapCamera{ false };
	bool m_waitForPause{ false };
	bool m_waitForFire{ false };
	bool m_waitForQuaternion{ false };
	bool m_waitForSavePosition{ false };
	bool m_waitForSaveGhost{ false };
	bool m_collisionObstacle{ false };

	// States
	bool m_ingame; // in-game
	bool m_endgame; // end-game
	bool m_pause; // break in-game
	bool m_loading; // loading ressources
	bool m_setting; // in settings menu
	bool m_help; // in help menu
	bool m_credit; // in credit menu
	bool m_timer{ false }; // chronometer before starting

	long long m_introDuration = 4000;
	std::chrono::steady_clock::time_point m_startTime;
	std::chrono::steady_clock::time_point m_currentTime;

	float m_bestScore = 0.f;
	float m_resultShipScore = 10'000.f;

	//Loading states;
	enum class ELoadingState {
		NOT_STARTED,
		loadPhysics,
		loadShipAndCameras,
		loadField,
		loadObstacles,
		loadLights,
		loadMateriaux,
		loadUI,
		COMPLETED
	};
	ELoadingState m_gameState;
	void animeGame();
	// Timer
	std::chrono::steady_clock::time_point m_previousTime;
	long long m_elapsedTime = 0;

	//Missiles
	void clearFarMissiles();

	void clearExplodedMissiles();

public:
	Scene(PM3D::CDispositifD3D11* _pDispositif,
		XMMATRIX* _proj,
		XMMATRIX* _view,
		XMMATRIX* _viewProj);
	~Scene();
	void init();
	void initShip();
	void initField();
	void initObstacles();
	void initCameras(XMMATRIX* _view, XMMATRIX* _proj, XMMATRIX* _viewProj);
	void initLights();
	void initMateriaux();
	void initUI();
	void preInitUI();
	void adjustLights();

	void addLight(const XMVECTOR &_position,
		const XMVECTOR &_valA = { .4f, .4f, .4f, 1.0f },
		const XMVECTOR &_valD = { 0.2f,0.2f,0.2f,1.f },
		const XMVECTOR &_valS = { 0.4f,0.4f, 0.4f,1.f },
		const float _r1 = std::numeric_limits<float>::infinity(),
		const float _r2 = std::numeric_limits<float>::infinity());

	bool initPhysics();
	void cleanupPhysics();
	bool render();
	void setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask);
	bool anime(float _tempEcoule);

	// For updates
	void updateElapsedTime(const std::chrono::steady_clock::time_point &_newTime) noexcept;
	void updatePostEffect();
	void updateMatProj(float _champDeVision, float _ratioDAspect, float _planRapproche = 2.0f, float _planEloigne = 800000.f);
	void readShortcuts();
	void swapCamera();

	// Getters - most of them are used for the UI
	int getCurrentCameraIndex() const noexcept { return m_currentCameraIndex; }
	DirectX::XMFLOAT3 getShipPosition() const noexcept { return m_hyperShip->getPosition(); }
	DirectX::XMFLOAT4 getShipRotation() const noexcept { return m_hyperShip->getRotationQuaternion(); }
	DirectX::XMFLOAT4 getCamRotation() const noexcept { return m_cameraList[m_currentCameraIndex]->getQuat(); }
	double getShipTime() const noexcept { return m_hyperShip->getT(); }
	float getShipSpeed() const noexcept { return m_hyperShip->getSpeed(); }
	float getShipCoefSpeed() const noexcept { return m_hyperShip->getCoeffSpeed(); }
	float getShipCoefMaxSpeed() const noexcept { return m_hyperShip->getCoeffMaxSpeed(); }
	float getShipCoefMinSpeed() const noexcept { return m_hyperShip->getCoeffMinSpeed(); }
	float getProgression() const noexcept { return m_hyperShip->getProgression(); }
	float getCamPositionX() const noexcept { return DirectX::XMVectorGetX(m_cameraList[m_currentCameraIndex]->getPosition()); }
	float getCamPositionY() const noexcept { return DirectX::XMVectorGetY(m_cameraList[m_currentCameraIndex]->getPosition()); }
	float getCamPositionZ() const noexcept { return DirectX::XMVectorGetZ(m_cameraList[m_currentCameraIndex]->getPosition()); }
	DirectX::XMFLOAT3 getCamPosition() const {
		return caster<XMFLOAT3>(m_cameraList[m_currentCameraIndex]->getPosition());
	}

	float getCamRotationX() const noexcept { return DirectX::XMVectorGetW(m_cameraList[m_currentCameraIndex]->getRotation()); }
	long long getElapsedTime() const noexcept { return m_elapsedTime; }
	bool isNewRecord() const noexcept { return m_bestScore <= m_resultShipScore; }

	float getResultShipScore() const noexcept { return m_resultShipScore; }

	auto getTest() { return m_hyperShip->getDirection().x; }
	void savePosition();

	bool isInGame() const noexcept { return m_ingame; }
	bool isEndGame() const noexcept { return m_endgame; }
	bool isPaused() const noexcept { return m_pause; }
	bool isLoading() const noexcept { return m_loading; }
	bool isSettingMode() const noexcept { return m_setting; }
	bool isHelpMode() const noexcept { return m_help; }
	bool isCreditMode() const noexcept { return m_credit; }
	bool isTimer() const noexcept { return m_timer; }
	
	void updateCurrentTime() { m_currentTime = std::chrono::high_resolution_clock::now(); }

	void startTimer() { m_startTime = m_currentTime = std::chrono::high_resolution_clock::now();	}
	long long getTimer() { return (m_introDuration - std::chrono::duration_cast<std::chrono::milliseconds>(m_currentTime - m_startTime).count())/1000; }
	bool checkTimerFinished();

	void setTimer(bool _val) { m_timer = _val; }

	////// Missiles
	void shoot();
	DirectX::XMFLOAT3 mouseToDir();
	DirectX::XMVECTOR rayCast(physx::PxVec3 _position, physx::PxVec3 _direction, float _distance = 800.f);

	// Settings
	void lowerMusic();
	void higherMusic();
	void lowerSound();
	void higherSound();
	std::wstring getVolumeMusicString() const;
	std::wstring getVolumeSoundString() const;

	////// Game steps
	void setAllModeToFalse() noexcept;
	void goToMainMenu();
	void goToTimerScreen();
	void goToEndGameMenu();
	void goToSettingsMenu();
	void goToHelpMenu();
	void goToCreditMenu();
	void launchLevel();

	void togglePause();
	void toggleQuaternion();

	void restart();
	void quit();

	////// PhysX - Contacts
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) override { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override {}
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {PX_UNUSED(pairHeader); PX_UNUSED(pairs); PX_UNUSED(nbPairs);}

	void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) override;

	collectable_value findCollectable(const physx::PxShape*) const;

	auto findObstacle(const physx::PxShape* _shape) const->obstacle_value;

	auto findMissile(const physx::PxShape* _shape) const -> missile_value;

	int getNbGoodCollectables() const noexcept { return m_hyperShip->getNbGoodCollectables(); }
	int getNbBadCollectables() const noexcept { return m_hyperShip->getNbBadCollectables(); }

	auto destroyMissile(const physx::PxShape* _shape)-> missile_value;

	void eraseCollectable(collectable_value _collectable);
	void eraseObstacle(obstacle_value _obstacle);

	void eraseMissile(std::shared_ptr<Missile> _missile);

	void loadCollectables();
	void loadObstacles();
	void exchangeTrashToList();

	void savePositionShip();
	void loadBestScore();
	void saveScore();

};