#pragma once
#include "stdafx.h"
#include <vector>
#include <DirectXMath.h>
#include <PxVec3.h>
#include "Courbe3D.h"
#include <string>

class LevelCreator
{
public:

	enum EObjects {
		eHyperShip = 1 << 0,
		eObstacle = 1 << 1,
		eMissile = 1 << 2,
		eCollectable = 1 << 3,
		eDestructible = 1 << 4
	};

	struct ObjectInfo {
		const char* m_assetFileDirectory;
		const char* m_assetName;
		DirectX::XMFLOAT3 m_objectPosition;
		DirectX::XMFLOAT4 m_objectRotation;
		DirectX::XMFLOAT3 m_objectScale;
		const std::string m_material_name;
		physx::PxVec3 m_axisRotation;

		ObjectInfo(const char* _assetFileDirectory,
			const char* _assetName,
			const std::string _materialname,
			DirectX::XMFLOAT3 _objectPosition = DirectX::XMFLOAT3{},
			DirectX::XMFLOAT4 _objectRotation = DirectX::XMFLOAT4{ 0.f,0.f,0.f,1.f },
			DirectX::XMFLOAT3 _objectScale = { 1.f,1.f,1.f },
			physx::PxVec3 _axisRotation = { 0.f, 3.f, 0.f })
			: m_assetFileDirectory { _assetFileDirectory },
			m_assetName{ _assetName },
			m_objectPosition{ _objectPosition },
			m_objectRotation{ _objectRotation },
			m_objectScale{ _objectScale },
			m_axisRotation{ _axisRotation },
			m_material_name{ _materialname }{}

	};

	struct LightInfo {
		const DirectX::XMVECTOR m_position;
		const DirectX::XMVECTOR m_valA;
		const DirectX::XMVECTOR m_valD;
		const DirectX::XMVECTOR m_valS;
		const float m_r1;
		const float m_r2;

		LightInfo(const DirectX::XMVECTOR& _position,
			const DirectX::XMVECTOR _valA = { .4f, .4f, .4f, 1.0f },
			const DirectX::XMVECTOR _valD = { 0.2f,0.2f,0.2f,1.f },
			const DirectX::XMVECTOR _valS = { 0.4f,0.4f, 0.4f,1.f },
			const float _r1 = std::numeric_limits<float>::infinity(),
			const float _r2 = std::numeric_limits<float>::infinity())
			: m_position{ _position },
			m_valA{ _valA },
			m_valD{ _valD },
			m_valS{ _valS },
			m_r1{ _r1 },
			m_r2{ _r2 }{}
	};

private:
	DirectX::XMFLOAT3 m_shipPosition;
	DirectX::XMFLOAT4 m_shipRotation;

	const char* m_heightmapFilename;
	DirectX::XMFLOAT3 m_heightmapScale;
	DirectX::XMFLOAT3 m_heightmapPosition;

	const char* m_skyboxModelDirectory;
	const char* m_skyboxModelFile;

	std::vector<ObjectInfo> m_objectDecInfoList;
	std::vector<ObjectInfo> m_obstacleInfoList;
	std::vector<ObjectInfo> m_collectableList;
	std::vector<ObjectInfo> m_antiCollectableList;
	std::vector<ObjectInfo> m_destructibleList;

	std::vector<LightInfo> m_lightInfoList;

	std::vector<physx::PxVec3> m_3dcurvePoints;
	std::vector<DirectX::XMVECTOR> m_3dcurveQuats;
	Courbe3D m_3dCurve;
	bool m_pointsDisplay = false;
	bool m_curveDisplay = false;

	unsigned int m_patounesTotales;
	unsigned int m_destructiblesTotales;
	unsigned int m_patounesGot = 0;

	DirectX::XMFLOAT4 m_idQuat = { 0.f,0.f,0.f,1.f };

	DirectX::XMFLOAT4 const m_baseRotation = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT3 const m_patouneScale = { 5.f, 5.f, 5.f };

public:

	enum Levels {
		Level_1,
		Level_2
	};

private:
	Levels m_level;

public:
	const char* const m_curveDisplayModelDirectory = "assets/modeles/Obstacles/Ball/";
	const char* const m_curveDisplayModelFile = "ball";

	const char* const m_patouneModelDirectory = "assets/modeles/Collectables/rabbitcoin/";
	const char* const m_patouneModelFile = "rabbitcoin";
	const char* const m_patouneTexture = "ball_material";
	const char* const m_antiPatouneTexture = "anti_material";

	const char* const m_obstacleModelDirectory = "assets/modeles/Obstacles/voxel/";
	const char* const m_obstacleModelFile = "voxel";
	const char* const m_obstacleTextureDirt = "dirt_material";
	const char* const m_obstacleTextureSpace = "obstacle_material"; 
	const char* const m_obstacleTextureDestructible = "destructible_material";


	LevelCreator() = default;
	LevelCreator(Levels level);

	DirectX::XMFLOAT3 const getShipPosition() noexcept { return m_shipPosition; }
	DirectX::XMFLOAT4 const getShipRotation() noexcept { return m_shipRotation; }
	const char* const getHeightmapFilename() noexcept { return m_heightmapFilename; }
	DirectX::XMFLOAT3 const getHeightmapScale() noexcept  { return m_heightmapScale; }
	DirectX::XMFLOAT3 const getHeightmapPosition() noexcept  { return m_heightmapPosition; }
	std::vector<ObjectInfo> const getObjectDecInfoList() { return m_objectDecInfoList; }
	std::vector<ObjectInfo> const getObstacleInfoList() { return m_obstacleInfoList; }
	std::vector<ObjectInfo> const getCollectableList() { return m_collectableList; }
	std::vector<ObjectInfo> const getAntiCollectableList() { return m_antiCollectableList; }
	std::vector<ObjectInfo> const getDestructibleList() { return m_destructibleList; }
	std::vector<LightInfo> const getLightInfoList() { return m_lightInfoList; }
	const char* const getSkyboxModelDirectory() noexcept { return m_skyboxModelDirectory; }
	const char* const getSkyboxModelFile() noexcept { return m_skyboxModelFile; }
	std::vector<physx::PxVec3> const get3dCurvePoints() { return m_3dcurvePoints; }
	Courbe3D const get3dCurve() noexcept { return m_3dCurve; }
	bool const isPointsDisplay() noexcept { return m_pointsDisplay; }
	bool const isCurveDisplay() noexcept { return m_curveDisplay; }
	unsigned int const getTotalPatounes() noexcept { return m_patounesTotales; }
	unsigned int const getCollectedPatounes() noexcept { return m_patounesGot; }

private:
	void initCourbe();
	void initLights();
	void initObtacles();
	void initCollectibles();

};

