#include "stdafx.h"
#include <algorithm>
#include "Scene.h"
#include "PlaneteTerrain.h"
#include "Obstacle.h"
#include "ObjDecGeneric.h"
#include "Objet2D.h"
#include "Ball.h"
#include "Material.h"
#include "MaterialManager.h"
#include "LevelCreator.h"
#include <FreeCamera.h>
#include "ThirdPersonCamera.h"
#include "SpotCamera.h"
#include <InputManager.h>
#include <FirstPersonCamera.h>
#include "Skybox.h"
#include "MoteurWindows.h"
#include "Collectable.h"
#include <src\Converter.h>
#include "PointInfo.h"


physx::PxFilterFlags sceneFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	constantBlockSize;
	constantBlock;

	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eMODIFY_CONTACTS;

	return physx::PxFilterFlag::eDEFAULT;
}

Scene::Scene(PM3D::CDispositifD3D11* _pDispositif, XMMATRIX* _proj, XMMATRIX* _view, XMMATRIX* _viewProj) 
	:m_ingame{ false }, m_pause{ false }, m_endgame{false}, m_loading{ true }, m_loadingState{ ELoadingState::NOT_STARTED },
	m_setting{ false }, m_help{ false }, m_credit{ false } , m_pDispositif{ _pDispositif }, m_matProj{ _proj }, m_matView{ _view },
	m_matViewProj{ _viewProj }, m_level{ LevelCreator(LevelCreator::Level_1) }, m_soundManager{ std::make_unique <SoundManager>(FindWindowA(nullptr, "PetitMoteur3D")) }, m_shadowEffect { std::make_unique<PM3D::ShadowEffect>(_pDispositif) }
{
	preInitUI();
	m_shadowEffect = std::make_unique<PM3D::ShadowEffect>(_pDispositif);
	m_pPanneau = std::make_unique<PM3D::PanneauPE>(_pDispositif);
}

void Scene::init()
{
}

bool Scene::initPhysics()
{
	using namespace physx;
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);

	m_pvd = PxCreatePvd(*m_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), true, m_pvd);

	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_dispatcher;
	sceneDesc.filterShader = sceneFilterShader;
	sceneDesc.simulationEventCallback = this;
	sceneDesc.contactModifyCallback = this;
	m_scenePhysic = m_physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_scenePhysic->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	m_material = m_physics->createMaterial(0.5f, 0.5f, 0.6f);
	return true;
}

void Scene::initShip()
{
	m_hyperShip = std::make_shared<HyperShip>(m_pDispositif, m_physics, m_scenePhysic, m_material, m_level.get3dCurve(), m_level.getShipPosition(), m_level.getShipRotation());
	setupFiltering(m_hyperShip->getActor(), LevelCreator::EObjects::eHyperShip, LevelCreator::EObjects::eObstacle | LevelCreator::EObjects::eMissile | LevelCreator::EObjects::eCollectable | LevelCreator::EObjects::eDestructible);
	m_hyperShip->anime(0.0f);
	m_objectList.emplace_back(m_hyperShip);

	m_hyperShip->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial("bunny"));
	//m_hyperShip->addMaterial(m_material_fisrt);

	m_ghostShip = std::make_unique<GhostShip>(m_pDispositif);
	m_ghostShip->anime(0.0f);
	m_objectList.emplace_back(m_ghostShip);
	m_ghostShip->loadCurve("src/ghostrecord.txt");
	m_ghostShip->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial("ghost"));
	loadBestScore();
	
}

void Scene::initMateriaux() {
	PM3D::MaterialManager& m_materiau_manager = PM3D::MaterialManager::getMaterialManager();
	std::shared_ptr < PM3D::Material > m_material_fisrt = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/MyTexture5.dds", "tunnel_material");
	m_materiau_manager.addMaterial(m_material_fisrt);
	std::shared_ptr < PM3D::Material > m_material_island = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/island.dds", "island");
	m_materiau_manager.addMaterial(m_material_island);
	std::shared_ptr < PM3D::Material > m_material_snow = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/Snow.dds", "snow_material");
	m_materiau_manager.addMaterial(m_material_snow);

	std::shared_ptr < PM3D::Material > m_material_mat = std::make_shared<PM3D::Material>(m_pDispositif, "", "mat_material_no_texture", XMFLOAT4{ 0.3f,0.3f ,0.3f ,1.0f }, XMFLOAT4{ 0.4f,0.4f ,0.4f , 1.0f }, XMFLOAT4{ 0.8f,0.8f ,0.8f , 1.0f });
	m_materiau_manager.addMaterial(m_material_mat);

	std::shared_ptr < PM3D::Material > m_material_ball = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/TextureOrange.dds", "ball_material", XMFLOAT4{ 0.4f,0.4f ,0.f ,1.0f }, XMFLOAT4{ 0.6f,0.6f ,0.0f , 1.0f }, XMFLOAT4{ 1.0f,1.0f ,0.0f , 1.0f });
	m_materiau_manager.addMaterial(m_material_ball);

	std::shared_ptr < PM3D::Material > m_material_anti = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/TextureRed.dds", "anti_material", XMFLOAT4{ 0.4f,0.0f ,0.0f ,1.0f }, XMFLOAT4{ 0.8f,0.0f ,0.0f , 1.0f }, XMFLOAT4{ 0.8f,0.0f ,0.0f , 1.0f });
	m_materiau_manager.addMaterial(m_material_anti);

	std::shared_ptr < PM3D::Material > m_material_destructible = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/neon/cube_neon.dds", "destructible_material", XMFLOAT4{ 0.8f,0.0f ,0.0f ,1.0f }, XMFLOAT4{ 0.8f,0.0f ,0.0f , 1.0f }, XMFLOAT4{ 0.8f,0.0f ,0.4f , 1.0f });
	m_materiau_manager.addMaterial(m_material_destructible);

	std::shared_ptr < PM3D::Material > m_material_obstacle = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/neon/cube_neon_blue.dds", "obstacle_material", XMFLOAT4{ 0.4f,0.0f ,0.8f ,1.0f }, XMFLOAT4{ 0.4f,0.0f ,0.8f , 1.0f }, XMFLOAT4{ 0.4f,0.0f ,0.8f , 1.0f });
	m_materiau_manager.addMaterial(m_material_obstacle);

	std::shared_ptr < PM3D::Material > m_material_bunny = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/shipbunny.dds", "bunny", XMFLOAT4{ 0.4f,0.4f ,0.4f ,1.0f }, XMFLOAT4{ 0.4f,0.4f ,0.4f , 1.0f }, XMFLOAT4{ 0.4f,0.4f ,0.4f , 1.0f });
	m_materiau_manager.addMaterial(m_material_bunny);

	std::shared_ptr < PM3D::Material > m_material_meteor = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/meteor.dds", "meteor", XMFLOAT4{ 0.4f,0.4f ,0.4f ,1.0f }, XMFLOAT4{ 0.4f,0.4f ,0.4f , 1.0f }, XMFLOAT4{ 0.4f,0.4f ,0.4f , 1.0f });
	m_materiau_manager.addMaterial(m_material_meteor);

	std::shared_ptr < PM3D::Material > m_material_ghost = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/shipbunnyghost.dds", "ghost", XMFLOAT4{ 0.4f,0.4f ,0.4f ,1.0f }, XMFLOAT4{ 0.4f,0.4f ,0.4f , 1.0f }, XMFLOAT4{ 0.0f,0.0f ,0.0f , 1.0f });
	m_materiau_manager.addMaterial(m_material_ghost);
	std::shared_ptr < PM3D::Material > m_material_terrain = std::make_shared<PM3D::Material>(m_pDispositif, "assets/textures/DirtTexture.dds", "dirt_material");
	m_materiau_manager.addMaterial(m_material_terrain);

}

void Scene::initField()
{
	m_objectList.emplace_back(std::make_shared<PlaneteTerrain>(m_level.getHeightmapFilename(), m_level.getHeightmapScale(), m_pDispositif, m_level.getHeightmapPosition()));
	m_objectList.emplace_back(std::make_shared<Skybox>(m_pDispositif, m_hyperShip, m_level.getSkyboxModelDirectory(), m_level.getSkyboxModelFile()));

	
	m_objectList.emplace_back(std::make_shared<PlaneteTerrain>(m_level.getHeightmapFilename(), m_level.getHeightmapScale(), m_pDispositif, m_level.getHeightmapPosition()));

	for (LevelCreator::ObjectInfo info : m_level.getObjectDecInfoList()) {
		std::shared_ptr< ObjDecGeneric> obj = std::make_shared<ObjDecGeneric>(m_pDispositif, info.m_assetFileDirectory, info.m_assetName, info.m_objectPosition, info.m_objectRotation, info.m_objectScale);
		obj->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial(info.m_material_name));
		m_objectList.emplace_back(std::move(obj));
	}

	for (LevelCreator::ObjectInfo info : m_level.getCollectableList()) {
			std::shared_ptr<Collectable> collObj2 = std::make_shared<Collectable>(m_pDispositif, m_physics, m_scenePhysic, m_material, info.m_assetFileDirectory, info.m_assetName, info.m_objectPosition, info.m_objectRotation, info.m_objectScale, info.m_axisRotation);
			collObj2->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial(info.m_material_name));
			setupFiltering(collObj2->getActor(),
				LevelCreator::EObjects::eCollectable,
				LevelCreator::EObjects::eObstacle | LevelCreator::EObjects::eHyperShip | LevelCreator::EObjects::eMissile | LevelCreator::EObjects::eCollectable | LevelCreator::EObjects::eDestructible);
			m_collectableList.emplace_back(std::move(collObj2));
	}
	for (LevelCreator::ObjectInfo info : m_level.getAntiCollectableList()) {
			std::shared_ptr<Collectable> collObj2 = std::make_shared<Collectable>(m_pDispositif,
				m_physics, m_scenePhysic, m_material,
				info.m_assetFileDirectory, info.m_assetName, info.m_objectPosition, info.m_objectRotation, info.m_objectScale, info.m_axisRotation,1.0f,
				false);

			collObj2->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial(info.m_material_name));
			setupFiltering(collObj2->getActor(),
				LevelCreator::EObjects::eCollectable,
				LevelCreator::EObjects::eObstacle | LevelCreator::EObjects::eHyperShip | LevelCreator::EObjects::eMissile | LevelCreator::EObjects::eCollectable | LevelCreator::EObjects::eDestructible);
			m_collectableList.emplace_back(std::move(collObj2));
	}

}

void Scene::initObstacles()
{
	if (m_level.isCurveDisplay()) {
		for (int i = 0; i < m_hyperShip->getCurve().getCurve().size(); i += 1)
		{
			auto point = m_hyperShip->getCurve().getCurve()[i];
			if (i % (m_hyperShip->getNbPointsQuat() + 1) == 0)
			{
				XMFLOAT4 rot = caster<XMFLOAT4>(point.getRotation());
				std::shared_ptr< ObjDecGeneric> obj = std::make_shared<ObjDecGeneric>(m_pDispositif, "assets/modeles/Ships/", "PointyShip", XMFLOAT3{ point.getPosition().x, point.getPosition().y, point.getPosition().z }, rot, XMFLOAT3{ 5.f, 5.f, 5.f });
				obj->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial("mat_material_no_texture"));
				m_objectList.emplace_back(std::move(obj));
			}
			else
			{
				std::shared_ptr< ObjDecGeneric> obj = std::make_shared<ObjDecGeneric>(m_pDispositif, m_level.m_curveDisplayModelDirectory, m_level.m_curveDisplayModelFile, XMFLOAT3{ point.getPosition().x, point.getPosition().y, point.getPosition().z });
				obj->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial("mat_material_no_texture"));
				m_objectList.emplace_back(std::move(obj));
			}
			//setupFiltering(std::dynamic_pointer_cast<Obstacle>(m_objectList.back())->getActor(), 1 << 2, 1 << 0 | 1 << 2);
			//std::shared_ptr< ObjDecGeneric> obj = std::make_shared<ObjDecGeneric>(m_pDispositif, m_level.m_curveDisplayModelDirectory, m_level.m_curveDisplayModelFile, XMFLOAT3{ point.getPosition().x, point.getPosition().y, point.getPosition().z });
			//obj->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial("mat_material_no_texture"));
			//m_objectList.emplace_back(std::move(obj));
		}
	} 
	else if (m_level.isPointsDisplay()) {
		for (auto point : m_level.get3dCurve().getPoints()) {
			std::shared_ptr< ObjDecGeneric> obj = std::make_shared<ObjDecGeneric>(m_pDispositif, m_level.m_curveDisplayModelDirectory, m_level.m_curveDisplayModelFile, XMFLOAT3{ point.x, point.y, point.z });
			obj->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial("snow_material"));
			m_objectList.emplace_back(std::move(obj));
		}

	}


	for (LevelCreator::ObjectInfo info : m_level.getObstacleInfoList()) {
		std::shared_ptr<Obstacle> collObj2 = std::make_shared<Obstacle>(m_pDispositif,
			m_physics, m_scenePhysic, m_material,
			info.m_assetFileDirectory, info.m_assetName,
			info.m_objectPosition, info.m_objectRotation, info.m_objectScale, 1.0f, false);

		collObj2->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial(info.m_material_name));
		setupFiltering(collObj2->getActor(),
			LevelCreator::EObjects::eObstacle,
			LevelCreator::EObjects::eObstacle | LevelCreator::EObjects::eHyperShip | LevelCreator::EObjects::eMissile | LevelCreator::EObjects::eCollectable | LevelCreator::EObjects::eDestructible);
		m_obstacleList.emplace_back(std::move(collObj2));
	}
	for (LevelCreator::ObjectInfo info : m_level.getDestructibleList()) {
		std::shared_ptr<Obstacle> collObj2 = std::make_shared<Obstacle>(m_pDispositif,
			m_physics, m_scenePhysic, m_material,
			info.m_assetFileDirectory, info.m_assetName,
			info.m_objectPosition, info.m_objectRotation, info.m_objectScale);

		collObj2->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial(info.m_material_name));
		setupFiltering(collObj2->getActor(),
			LevelCreator::EObjects::eDestructible,
			LevelCreator::EObjects::eObstacle | LevelCreator::EObjects::eHyperShip | LevelCreator::EObjects::eMissile | LevelCreator::EObjects::eCollectable | LevelCreator::EObjects::eDestructible);
		m_obstacleList.emplace_back(std::move(collObj2));
	}
}

void Scene::initCameras(XMMATRIX* _view, XMMATRIX* _proj, XMMATRIX* _viewProj)
{
	m_cameraList.emplace_back(std::make_shared<PM3D::ThirdPersonCamera>(XMVectorSet(0.0f, 100.0f, -20.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _view, _proj, _viewProj, m_hyperShip));

	m_cameraList.emplace_back(std::make_shared<PM3D::FirstPersonCamera>(XMVectorSet(0.0f, 100.0f, -20.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _view, _proj, _viewProj, m_hyperShip));

	m_cameraList.emplace_back(std::make_shared<PM3D::FreeCamera>(XMVectorSet(0.0f, 10.0f, -20.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), _view, _proj, _viewProj));

	m_cameraList.emplace_back(std::make_shared<PM3D::SpotCamera>(XMVectorSet(241.f, 152.f, 314.f, 1.0f),
		XMVectorSet(-0.186199173f, -0.532819867f, -0.825489521f, 1.f),
		XMVectorSet(0.f, 1.f, 0.f, 1.f), _view, _proj, _viewProj));

	m_cameraList[m_currentCameraIndex]->update(0.f);
}

void Scene::initLights() {
	//ambiant light

	addLight({ 10000.f, 10000.f, 0.f,1.f }, { 0.6f,0.6f, 0.6f,1.f });

	for (LevelCreator::LightInfo info : m_level.getLightInfoList()) {
		addLight(info.m_position, info.m_valA, info.m_valD, info.m_valS, info.m_r1, info.m_r2);
	}
}

void Scene::preInitUI()
{
	m_ui = std::make_unique<UI>(m_pDispositif, this);
}

void Scene::initUI()
{
	m_ui->init();
	goToMainMenu();
	m_cameraList[m_currentCameraIndex]->update(0.f);
}

void Scene::adjustLights() {
	while (PM3D::CMoteurWindows::get().getLights().size() < MAX_LIGHTS)
		addLight({ 100000.f, 100000.f, 0.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.f, 0.f, 0.f,1.f });
}

void Scene::addLight(const XMVECTOR &_position, const XMVECTOR &_valA, const XMVECTOR &_valD, const XMVECTOR &_valS, const float _r1, const float _r2) {
	_position; _valA; _valD; _valS; _r2; _r1;
	PM3D::CMoteurWindows::get().addLight(std::make_shared<PM3D::CLumierePoint>(PM3D::CLumierePoint(_position, _valA, _valD, _valS, _r1, _r2)));
}

Scene::~Scene()
{
	cleanupPhysics();
}

void Scene::cleanupPhysics()
{
	PX_RELEASE(m_scenePhysic);
	PX_RELEASE(m_dispatcher);
	PX_RELEASE(m_physics);
	if (m_pvd)
	{
		physx::PxPvdTransport* transport = m_pvd->getTransport();
		m_pvd->release(); m_pvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_foundation);
}

void Scene::readShortcuts()
{
	PM3D::InputManager& inputManager = PM3D::InputManager::get();

	if (inputManager.isPressed(DIK_T))
	{
		if (!m_waitForSavePosition)
			savePosition();
	}
	else
		m_waitForSavePosition = false;

	if (inputManager.isPressed(DIK_R))
		restart();

	if (inputManager.isPressed(DIK_RETURN))
		launchLevel();
	if (inputManager.isPressed(DIK_U))
	{
		if (!m_waitForQuaternion)
			toggleQuaternion();
	}
	else
		m_waitForQuaternion = false;
	if (inputManager.getMouseState().rgbButtons[0] & 0x80 && !m_hyperShip->getIsLocked())
	{
		if (!m_waitForFire)
			shoot();
	}
	else
		m_waitForFire = false;

	if (inputManager.isPressed(DIK_ESCAPE))
	{
		if (!m_waitForPause)
			togglePause();
	}
	else
		m_waitForPause = false;

	if (inputManager.isPressed(DIK_J))
	{
		if (!m_waitForSaveGhost)
			saveScore();
	}
	else
		m_waitForSaveGhost = false;

}

bool Scene::checkTimerFinished()
{
	long long a = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_startTime).count();
	return ( a >= m_introDuration);
}

void Scene::shoot()
{
	m_waitForFire = true;

	if (m_hyperShip->readyToShoot())
	{
		XMFLOAT3 rayDirection = mouseToDir();
		XMVECTOR farPoint = rayCast(caster<physx::PxVec3>(m_cameraList[m_currentCameraIndex]->getPosition()) + caster<physx::PxVec3>(rayDirection) * 45.f, caster<physx::PxVec3>(rayDirection));
		XMFLOAT3 newDir = caster<DirectX::XMFLOAT3>(XMVector3Normalize( std::move(farPoint) - caster<DirectX::XMVECTOR>(m_hyperShip->getPosition())));

		std::shared_ptr<Missile> missile = std::make_shared<Missile>(m_pDispositif, m_physics, m_scenePhysic, m_material, m_hyperShip->getPosition(), newDir);
		setupFiltering(missile->getActor(), LevelCreator::EObjects::eMissile, LevelCreator::eCollectable | LevelCreator::EObjects::eObstacle | LevelCreator::EObjects::eHyperShip | LevelCreator::EObjects::eMissile | LevelCreator::EObjects::eDestructible);
		missile->anime(0.0f);
		missile->addMaterial(PM3D::MaterialManager::getMaterialManager().getMaterial("mat_material_no_texture"));
		m_missileList.emplace_back(std::move(missile));

		m_hyperShip->startLoadingMissile();
		PM3D::CMoteurWindows::get().setCursorTargetUnavailable();
		m_soundManager->playSoundShoot();
	}
}

DirectX::XMFLOAT3 Scene::mouseToDir()
{
	DirectX::XMFLOAT2 posMouse = PM3D::InputManager::get().getMousePosition(); // Screen-coordinates
	DirectX::XMVECTOR mouseNear = DirectX::XMVectorSet(posMouse.x + 2.f, posMouse.y - 8.f, 0.0f, 0.0f);
	DirectX::XMVECTOR mouseFar = DirectX::XMVectorSet(posMouse.x + 2.f, posMouse.y - 8.f, 1.0f, 0.0f);
	DirectX::XMVECTOR unprojectedNear = DirectX::XMVector3Unproject(std::move(mouseNear), 0, 0, 1920, 1080, 0.0f, 1.f,
		*(m_cameraList[m_currentCameraIndex]->getMatProj()), *(m_cameraList[m_currentCameraIndex]->getMatView()), DirectX::XMMatrixIdentity());
	DirectX::XMVECTOR unprojectedFar = DirectX::XMVector3Unproject(std::move(mouseFar), 0, 0, 1920, 1080, 0.0f, 1.f,
		*(m_cameraList[m_currentCameraIndex]->getMatProj()), *(m_cameraList[m_currentCameraIndex]->getMatView()), DirectX::XMMatrixIdentity());
	DirectX::XMVECTOR result = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(std::move(unprojectedFar), std::move(unprojectedNear)));
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, std::move(result));
	return direction;
}

DirectX::XMVECTOR Scene::rayCast(physx::PxVec3 _position, physx::PxVec3 _direction, float _distance)
{
	// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/SceneQueries.html
	// Raycast against all static & dynamic objects (no filtering)
	// The main result from this call is the closest hit, stored in the 'hit.block' structure
	using namespace physx;
	
	PxRaycastBuffer hit;
	if (m_scenePhysic->raycast(_position, _direction, _distance, hit))
		return caster<DirectX::XMVECTOR>(hit.block.position);
	else
		return caster<DirectX::XMVECTOR>(_position + _direction * _distance);
}

void Scene::lowerMusic()
{
	m_soundManager->lowerMusic();
}

void Scene::higherMusic()
{
	m_soundManager->higherMusic();
}

void Scene::lowerSound()
{
	m_soundManager->lowerSound();
}

void Scene::higherSound()
{
	m_soundManager->higherSound();
}

std::wstring Scene::getVolumeMusicString() const
{
	return m_soundManager->getVolumeMusicString();
}

std::wstring Scene::getVolumeSoundString() const
{
	return m_soundManager->getVolumeSoundString();
}

void Scene::setAllModeToFalse() noexcept
{
	m_ingame = false;
	m_pause = false;
	m_setting = false;
	m_loading = false;
	m_endgame = false;
	m_timer = false;
	m_help = false;
	m_credit = false;
}

void Scene::goToMainMenu()
{
	setAllModeToFalse();

	m_ui->updateDisplayedItems();
	PM3D::CMoteurWindows::get().setCursorArrow();
	m_soundManager->stopMusicInGame();
	m_soundManager->playMusicMenu();
}

void Scene::goToTimerScreen()
{
	setAllModeToFalse();
	m_ingame = true;
	m_timer = true;

	m_hyperShip->setIsLocked(true);
	m_ui->updateDisplayedItems();
	m_soundManager->stopMusicMenu();
	m_soundManager->playMusicInGame();
}

void Scene::goToEndGameMenu()
{
	setAllModeToFalse();
	m_endgame = true;

	m_ui->updateDisplayedItems();
	PM3D::CMoteurWindows::get().setCursorArrow();
	saveScore();
	m_soundManager->stopMusicInGame();
	m_soundManager->playMusicVictory();
}

void Scene::goToSettingsMenu()
{
	setAllModeToFalse();
	m_setting = true;

	m_ui->updateDisplayedItems();
	PM3D::CMoteurWindows::get().setCursorArrow();
}

void Scene::goToHelpMenu()
{
	setAllModeToFalse();
	m_help = true;

	m_ui->updateDisplayedItems();
	PM3D::CMoteurWindows::get().setCursorArrow();
}

void Scene::goToCreditMenu()
{
	setAllModeToFalse();
	m_credit = true;

	m_ui->updateDisplayedItems();
	PM3D::CMoteurWindows::get().setCursorArrow();
}

void Scene::launchLevel()
{
	setAllModeToFalse();
	m_ingame = true;

	m_ui->updateDisplayedItems();

	m_hyperShip->setIsLocked(false);
	PM3D::CMoteurWindows::get().setCursorTargetAvailable();
	m_previousTime = std::chrono::high_resolution_clock::now();
}

void Scene::togglePause()
{
	m_pause = !m_pause;
	m_waitForPause = true;
	m_ui->updateDisplayedItems();
	if (m_pause)
		PM3D::CMoteurWindows::get().setCursorArrow();
	else if (m_hyperShip->readyToShoot())
		PM3D::CMoteurWindows::get().setCursorTargetAvailable();
	else
		PM3D::CMoteurWindows::get().setCursorTargetUnavailable();
}

void Scene::toggleQuaternion()
{
	m_waitForQuaternion = true;
	m_hyperShip->setIsMoving(!(m_hyperShip->getIsMoving()));
}

void Scene::restart()
{
	loadCollectables();
	loadObstacles();
	m_missileList.clear();
	m_hyperShip->reset();
	m_ghostShip->loadCurve("src/ghostrecord.txt");
	m_elapsedTime = 0;
	//launchLevel();
	startTimer();
	goToTimerScreen();
}

void Scene::quit()
{
	PM3D::CMoteurWindows::get().quit();
}

void Scene::updateElapsedTime(const std::chrono::steady_clock::time_point &_newTime) noexcept
{	m_elapsedTime += std::chrono::duration_cast<std::chrono::milliseconds>(_newTime - m_previousTime).count();
}

bool Scene::anime(float _tempEcoule)
{
	switch (m_gameState)
	{
	case ELoadingState::loadPhysics:
		initPhysics();
		m_ui->updateLoadingScreen(10.f);
		break;
	case ELoadingState::loadMateriaux:
		initMateriaux();
		m_ui->updateLoadingScreen(50.f);
		break;
	case ELoadingState::loadLights:
		initLights();
		m_ui->updateLoadingScreen(90.f);
		break;
	case ELoadingState::loadField:
		initField();
		m_ui->updateLoadingScreen(95.f);
		break;
	default:
		animeGame();
		break;
	}
	return true;
}

	if (m_ingame && !m_timer)
		readShortcuts();

	auto newTime = std::chrono::high_resolution_clock::now();
	if (isTimer())
	{
		if (checkTimerFinished()) {
			launchLevel();
		}
		else {
			updateCurrentTime();
			m_ui->updateTimer((m_currentTime-m_startTime).count());
		}

	}
	if (isInGame() && !isPaused()) // Update the game
	{
		// Check if the game is over
		if (m_hyperShip->isArrived())
			goToEndGameMenu();
		else
		{
			updateElapsedTime(newTime);
			if (m_ingame && !m_timer)
				swapCamera();
			clearFarMissiles();

			// Animation - changement de position rotation scale HF++
			for (auto& object : m_objectList)
				object->anime(_tempEcoule);

			for (auto& missile : m_missileList)
				missile->anime(_tempEcoule);

			for (auto& collectable : m_collectableList)
				collectable->anime(_tempEcoule);

			for (auto& obstacle : m_obstacleList)
				obstacle->anime(_tempEcoule);

			// Physic - Update des positions physique lié à l'objet
			m_scenePhysic->simulate(1.0f / 60.0f);
			m_scenePhysic->fetchResults(true);

			clearExplodedMissiles();

			// Update les positions Graphique des meshs
			for (auto& object : m_objectList)
				object->update(_tempEcoule);

			for (auto& missile : m_missileList)
				missile->update(_tempEcoule);

			for (auto& obstacle : m_obstacleList)
				obstacle->update(_tempEcoule);

			for (auto& collectable : m_collectableList)
				collectable->update(_tempEcoule);
			// Camera
			m_cameraList[m_currentCameraIndex]->update(_tempEcoule);

			//update postEffect
			updatePostEffect();
		}
	}

	// UI
	m_ui->update();

	m_previousTime = std::move(newTime);
	return true;
}

void Scene::updatePostEffect()
{
	float percent = (m_hyperShip->getCoeffSpeed() - m_hyperShip->getCoeffMinSpeed()) / (m_hyperShip->getCoeffMaxSpeed() - m_hyperShip->getCoeffMinSpeed());
	
	if (m_hyperShip->getIsSlowedDown()) {
		m_pPanneau->setBlurPercent(percent );
		m_pPanneau->setRedPercent(m_hyperShip->updateSlowDown());
		m_pPanneau->setTechniqueIndice(2);
	}
	else {
		m_pPanneau->setBlurPercent(percent);
		m_pPanneau->setTechniqueIndice(1);
	}
	m_cameraList[m_currentCameraIndex]->getMatProj();

	float m_champDeVision = XM_PI / 4 + XM_PI / 6 * percent; 
	float m_ratioDAspect = static_cast<float>(m_pDispositif->GetLargeur()) / static_cast<float>(m_pDispositif->GetHauteur());
	float m_planRapproche = 2.0f;
	float m_planEloigne = 800000.0f;

	updateMatProj(m_champDeVision, m_ratioDAspect, m_planRapproche, m_planEloigne);
}

void Scene::updateMatProj(float _champDeVision, float _ratioDAspect, float _planRapproche, float _planEloigne)
{
	*m_matProj = DirectX::XMMatrixPerspectiveFovLH(_champDeVision, _ratioDAspect, _planRapproche, _planEloigne);
	*m_matViewProj = *m_matView * *m_matProj;
}

void Scene::swapCamera()
{
	PM3D::InputManager& inputManager = PM3D::InputManager::get();

	// Verifier l'etat de la key SwapMode
	if (inputManager.isPressed(DIK_TAB))
	{
		if (!m_waitForSwapCamera) {
			m_currentCameraIndex = (m_currentCameraIndex + 1) % m_cameraList.size();
			m_waitForSwapCamera = true;
		}
	}
	else 
	{
		m_waitForSwapCamera = false;
	}
}

void Scene::clearFarMissiles()
{
	auto it = std::partition(m_missileList.begin(), m_missileList.end(), [](std::shared_ptr<Missile> missile) noexcept {return !missile->isFar(); } );

	m_missileList.erase(it, m_missileList.end());
}

void Scene::clearExplodedMissiles()
{
	auto it = std::partition(m_missileList.begin(), m_missileList.end(), [](std::shared_ptr<Missile> missile) noexcept {return !missile->getIsBlownUp(); });

	m_missileList.erase(it, m_missileList.end());
}

bool Scene::render()
{
	if (m_loadingState != ELoadingState::COMPLETED) {
		m_ui->draw();
		return true;
	}

	m_pPanneau->DebutPostEffect();

	//enable the ShadowMap
	m_shadowEffect->beginShadowEffect();

		//Draw Every object in the ShadowMap Texture
		for (auto& object : m_objectList)
			object->DrawShadowMap(m_shadowEffect->GetMatricVPLight());

		for (auto& missile : m_missileList)
			missile->Draw(m_shadowEffect->GetShadowMapView(), m_shadowEffect->GetMatricVPLight(), m_cameraList[m_currentCameraIndex]->getPosition());

		for (auto& collectable : m_collectableList)
			collectable->Draw(m_shadowEffect->GetShadowMapView(), m_shadowEffect->GetMatricVPLight(), m_cameraList[m_currentCameraIndex]->getPosition());

		for (auto& obstacle : m_obstacleList)
			obstacle->Draw(m_shadowEffect->GetShadowMapView(), m_shadowEffect->GetMatricVPLight(), m_cameraList[m_currentCameraIndex]->getPosition());

	//disable the ShadowMap
	m_shadowEffect->endShadowEffect();

	//draw every object with the ShadowMap
	for (auto& object : m_objectList)
		object->Draw(m_shadowEffect->GetShadowMapView(), m_shadowEffect->GetMatricVPLight(), m_cameraList[m_currentCameraIndex]->getPosition());

	for (auto& missile : m_missileList)
		missile->Draw(m_shadowEffect->GetShadowMapView(), m_shadowEffect->GetMatricVPLight(), m_cameraList[m_currentCameraIndex]->getPosition());

	for (auto& collectable : m_collectableList)
		collectable->Draw(m_shadowEffect->GetShadowMapView(), m_shadowEffect->GetMatricVPLight(), m_cameraList[m_currentCameraIndex]->getPosition());
	for (auto& obstacle : m_obstacleList)
		obstacle->Draw(m_shadowEffect->GetShadowMapView(), m_shadowEffect->GetMatricVPLight(), m_cameraList[m_currentCameraIndex]->getPosition());

	m_pPanneau->FinPostEffect();
	
	m_pPanneau->Draw();
	m_ui->draw();
	

	return true;
}

void Scene::setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask)
{
	physx::PxFilterData filterData;
	filterData.word0 = filterGroup;
	filterData.word1 = filterMask;

	const physx::PxU32 numShapes = actor->getNbShapes();
	physx::PxShape** shapes = (physx::PxShape**)malloc(sizeof(physx::PxShape*) * numShapes);
	actor->getShapes(shapes, numShapes);
	for (physx::PxU32 i = 0; i < numShapes; i++)
	{
		physx::PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	free(shapes);
}

void Scene::onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count)
{
	for (physx::PxU32 i = 0; i < count; i++)
	{
		physx::PxContactModifyPair& pair = pairs[i];

		if (pair.actor[0] != NULL && pair.actor[1] != NULL) {

			if (pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eMissile
				&& pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eCollectable)
			{
				destroyMissile(pair.shape[0]);
				m_soundManager->playSoundCoinIsShot();
			}
			else if (pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eMissile
					&& pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eCollectable)
			{
				destroyMissile(pair.shape[1]);
				m_soundManager->playSoundCoinIsShot();
			}
			else if (((pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eHyperShip
				|| pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eHyperShip))
				&& ((pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eObstacle
					|| pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eObstacle)))
			{
				m_hyperShip->setIsSlowedDown(true);
				m_hyperShip->startTimerSlowDown();
				m_hyperShip->getActor()->setAngularVelocity({ 0.f,0.f,0.f });
				m_hyperShip->getActor()->setLinearVelocity(caster<physx::PxVec3>(m_hyperShip->getDirection()));

				if (!m_collisionObstacle)
				{
					m_soundManager->playSoundObstacleExplodes();
					m_collisionObstacle = true;
				}
			}
			else {
				m_collisionObstacle = false;

				for (physx::PxU32 j = 0; j < pair.contacts.size(); j++) {
					pair.contacts.ignore(j);
				}
			}
			
			

			if (pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eHyperShip 
				&& pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eCollectable) 
			{
				//same thing than before
				if (auto collectable = findCollectable(pair.shape[0]); collectable) {
					if (collectable->isBonus())
					{
						m_hyperShip->addNbGoodCollectables();
						m_soundManager->playSoundCoinGet();
					}
					else
					{
						m_hyperShip->addNbBadCollectables();
						m_soundManager->playSoundBadCoinGet();
					}

					collectable->getActor()->setGlobalPose(m_graveyard);
					m_collectableTrash.emplace_back(collectable);
					eraseCollectable(collectable);
				}
			}

			if (pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eHyperShip
				&& pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eCollectable)
			{
				//find the coin
				if (auto collectable = findCollectable(pair.shape[1]); collectable) {
					if (collectable->isBonus())
					{
						m_hyperShip->addNbGoodCollectables();
						m_soundManager->playSoundCoinGet();
					}
					else
					{
						m_hyperShip->addNbBadCollectables();
						m_soundManager->playSoundBadCoinGet();
					}

					collectable->getActor()->setGlobalPose(m_graveyard);
					m_collectableTrash.emplace_back(collectable);
					eraseCollectable(collectable);
				}
			}

			if (pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eMissile
				&& (pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eDestructible
					|| pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eObstacle))
			{
				destroyMissile(pair.shape[0]);//->setIsBlownUp(true);
				//find the destructible
				if (auto obstacle = findObstacle(pair.shape[1]); obstacle && obstacle->getIsDestructible()) {

					//"erase" the obstacle
					obstacle->getActor()->setGlobalPose(m_graveyard);
					m_obstacleTrash.emplace_back(obstacle);
					eraseObstacle(obstacle);
				}
				m_soundManager->playSoundObstacleExplodes();
			}
			if ((pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eDestructible
				|| pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eObstacle)
				&& pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eMissile)
			{
				destroyMissile(pair.shape[1]);
				//find the destructible
				if (auto obstacle = findObstacle(pair.shape[1]); obstacle && obstacle->getIsDestructible()) {

					//"erase" the obstacle
					obstacle->getActor()->setGlobalPose(m_graveyard);
					m_obstacleTrash.emplace_back(obstacle);
					eraseObstacle(obstacle);
				}
				m_soundManager->playSoundObstacleExplodes();
			}
			if (pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eHyperShip
				&& (pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eDestructible))
			{
				m_hyperShip->setIsSlowedDown(true);
				m_hyperShip->startTimerSlowDown();
				//find the destructible
				if (auto destructible = findObstacle(pair.shape[0]); destructible) {

					//"erase" the obstacle
					destructible->getActor()->setGlobalPose(m_graveyard);
					m_obstacleTrash.emplace_back(destructible);
					eraseObstacle(destructible);
				}
				m_soundManager->playSoundShipCollideObstacle();
			}
			if ((pair.shape[1]->getSimulationFilterData().word0 == LevelCreator::eDestructible
				&& pair.shape[0]->getSimulationFilterData().word0 == LevelCreator::eHyperShip))
			{
				//find the destructible
				m_hyperShip->setIsSlowedDown(true);
				m_hyperShip->startTimerSlowDown();
				if (auto destructible = findObstacle(pair.shape[1]); destructible) {

					//"erase" the obstacle
					destructible->getActor()->setGlobalPose(m_graveyard);
					m_obstacleTrash.emplace_back(destructible);
					eraseObstacle(destructible);
				}
				m_soundManager->playSoundShipCollideObstacle();
			}
		}
	}
}

auto Scene::findCollectable(const physx::PxShape* _shape) const -> collectable_value {
	auto it = find_if(m_collectableList.begin(), m_collectableList.end(), [&](const collectable_value coll)
	{
		return (coll->getShape() == _shape);
	});
	if(it != m_collectableList.end())
		return *it;
	return nullptr;

}

auto Scene::findObstacle(const physx::PxShape* _shape) const -> obstacle_value {
	auto it = find_if(m_obstacleList.begin(), m_obstacleList.end(), [&](const obstacle_value coll)
		{
			return (coll->getShape() == _shape);
		});
	if (it != m_obstacleList.end())
		return *it;
	return nullptr;
}

auto Scene::destroyMissile(const physx::PxShape* _shape) -> missile_value {
	auto it = find_if(m_missileList.begin(), m_missileList.end(), [&](const missile_value coll)
		{
			return (coll->getShape() == _shape);
		});
	if (it != m_missileList.end())
	{
		m_scenePhysic->removeActor(*it->get()->getActor());
		m_missileList.erase(it);
	}
	return nullptr;

}

auto Scene::findMissile(const physx::PxShape* _shape) const -> missile_value {
	auto it = find_if(m_missileList.begin(), m_missileList.end(), [&](const missile_value coll)
		{
			return (coll->getShape() == _shape);
		});
	if (it != m_missileList.end())
		return (*it);
	return nullptr;

}

void Scene::eraseCollectable(collectable_value _collectable) {
	auto it = find_if(m_collectableList.begin(), m_collectableList.end(), [&](const collectable_value coll)
	{
		return (coll == _collectable);
	});

	m_collectableList.erase(it);
}

void Scene::eraseObstacle(obstacle_value _obstacle) {
	auto it = find_if(m_obstacleList.begin(), m_obstacleList.end(), [&](const obstacle_value obs)
		{
			return (obs == _obstacle);
		});

	m_obstacleList.erase(it);
}

void Scene::eraseMissile(std::shared_ptr<Missile> _missile) {

	if (_missile == nullptr)
		return;
	auto it = find_if(m_missileList.begin(), m_missileList.end(), [&](const std::shared_ptr<Missile> obs)
		{
			return (obs == _missile);
		});

	m_missileList.erase(it);
}

void Scene::loadCollectables() {
	
	//put the trash in the good one 
	//std::move(m_collectableTrash.begin(), m_collectableTrash.end(), m_collectableList.begin());
	exchangeTrashToList();

	//restartColatballe
	std::for_each(m_collectableList.begin(), m_collectableList.end(), [&](collectable_value& _coll) {
		_coll->reset();
	});
}

void Scene::loadObstacles() {

	//put the trash in the good one 
	//std::move(m_collectableTrash.begin(), m_collectableTrash.end(), m_collectableList.begin());
	exchangeTrashToList();

	//restartColatballe
	std::for_each(m_obstacleList.begin(), m_obstacleList.end(), [&](obstacle_value& _obs) {
		_obs->reset();
		});
}

void Scene::exchangeTrashToList() {
	while ((m_collectableTrash.size() != 0)) {
		m_collectableList.emplace_back(std::move(m_collectableTrash.back()));
		m_collectableTrash.pop_back();
	}
	while ((m_obstacleTrash.size() != 0)) {
		m_obstacleList.emplace_back(std::move(m_obstacleTrash.back()));
		m_obstacleTrash.pop_back();
	}
}

#include <fstream>
void Scene::savePosition()
{
	//m_collectableList.push_back(ObjectInfo("assets/modeles/Collectables/rabbitcoin/", "rabbitcoin", "ball_material", XMFLOAT3{-1895.3, - 1700, 1853.1}));

	XMFLOAT3 pos = m_hyperShip->getPosition();
	std::ofstream file{ "src/positions.txt", std::ios_base::app };
	file << "m_collectableList.push_back(ObjectInfo(\"assets/modeles/Collectables/rabbitcoin/\", \"rabbitcoin\", \"ball_material\", XMFLOAT3{" <<
		pos.x << ", " <<
		pos.y << ", " <<
		pos.z << "}));" << std::endl;
	m_waitForSavePosition = true;
}

void Scene::savePositionShip() {
	std::ofstream file{ "src/ghostrecord.txt", std::ofstream::out};
	std::vector<PointInfo> v = m_hyperShip->getSaveCurbe();

	std::copy(begin(v), end(v), std::ostream_iterator<PointInfo>{file, "\n"});
	file.close();
}

void Scene::loadBestScore() {
	std::ifstream is{ "src/highscore.txt" };
	is >> m_bestScore;
	is.close();
}

void Scene::saveScore() {
	long long minutes = getElapsedTime() / 60'000; // 60'000 because ellapsedTime is in ms
	long long seconds = (getElapsedTime() - minutes * 60'000) / 1'000;

	m_resultShipScore = 10'000.f - (seconds * 100.f) + 1000.f * m_hyperShip->getNbGoodCollectables() - 500 * m_hyperShip->getNbBadCollectables();

	if (isNewRecord()) {
		m_bestScore = m_resultShipScore;
		std::ofstream file{ "src/highscore.txt" };
		file << m_resultShipScore;
		file.close();
		savePositionShip();
	}
}