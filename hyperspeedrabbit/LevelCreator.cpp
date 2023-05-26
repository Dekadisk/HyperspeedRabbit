#include "stdafx.h"
#include "LevelCreator.h"
using namespace physx;

using namespace DirectX;

LevelCreator::LevelCreator(Levels level)
{
	m_level = level;

	m_objectDecInfoList.clear();
	m_obstacleInfoList.clear();

	switch (level) {
	case Level_1:

		//Skybox
		m_skyboxModelDirectory = "assets/modeles/Skybox/";
		m_skyboxModelFile = "skybox.obj";

		//Ship
		m_shipPosition = XMFLOAT3{ 176.f, -249.f, -963.f };
		m_shipRotation = XMFLOAT4{ 0.f, 0.f, 0.f, 0.f };

		//Heightmap
		m_heightmapFilename = "assets/bitmap/Terrain32p.bmp";
		m_heightmapScale = XMFLOAT3{ 10.f * 6.25f,-2.5f,10.f * 6.25f };
		m_heightmapPosition = XMFLOAT3{ 150.f, 0.f, 12.f };

		//ObjectsDec
		m_objectDecInfoList.push_back(ObjectInfo("assets/modeles/Decorations/island/", "island", "island", XMFLOAT3{ 150.f, -2150.f, 12.f }, XMFLOAT4{ 0.f, 0.f, 0.f, 0.f }, { 10000.f, 10000.f, 10000.f }));
		//m_objectDecInfoList.push_back(ObjectInfo("assets/modeles/Decorations/earth/", "earth","snow_material", XMFLOAT3{ 9500.f, 100000.f, 100000.f }) );
		//m_objectDecInfoList.push_back(ObjectInfo("assets/modeles/Decorations/earth/", "earth","snow_material", XMFLOAT3{ 3000.f, -1000.f, -2000.f }) );
		//"assets/modeles/Decorations/pipeball/", "pipeball", XMFLOAT3{ 337.f, -4.f, -305.f }, XMFLOAT4{2.f, 1.5f,0.f,1.f}

	
		m_objectDecInfoList.push_back(ObjectInfo("assets/modeles/Decorations/AsteroidTunnel/", "AsteroidTunnel", "meteor", XMFLOAT3{ 230.f, 1500.f, 1000.f }, XMFLOAT4{ -1.f, 0.f, 0.f, 1.f }, { 40.f, 40.f, 40.f }));

		float part1centerX = -170.f;
		float part1centerY = -250.f;

		float* centerX = &part1centerX;
		float* centerY = &part1centerY;

		float rightX = *centerX + 10.f;
		float leftX = *centerX - 20.f;
		float upY = *centerY + 15.f;
		float downY = *centerY - 20.f;

		initObtacles();
		initCollectibles();
		initCourbe();
		initLights();
		break;
	}
}

void LevelCreator::initObtacles()
{
	switch (m_level) {
	case Level_1:


		float part1centerX = -170.f;
		float part1centerY = -260.f;

		float rightX = part1centerX + 10.f;
		float leftX = part1centerX - 20.f;
		float upY = part1centerY + 15.f;
		float downY = part1centerY - 20.f;

		//Part 1 premier tiers

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ rightX, part1centerY, -900.f }, m_baseRotation, { 25.f, 55.f, 10.f }));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ leftX + 5.f, part1centerY + 7.f, -690.f }, m_baseRotation, { 35.f, 35.f, 10.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ part1centerX, downY, -690.f }, m_baseRotation, { 66.f, 20.f, 10.f }));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ rightX, part1centerY, -550.f }, m_baseRotation, { 30.f, 55.f, 10.f }));

		//Part 1 deuxieme tiers
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ -50.f, part1centerY -10.f, -200.f }, XMFLOAT4{ 0.f, .3f, 0.f, 1.f }, { 22.f, 45.f, 30.f }));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ 0.f, part1centerY - 10.f, -100.f }, XMFLOAT4{ 0.f, .3f, 0.f, 1.f }, { 15.f, 45.f, 30.f }));

		part1centerX = 230.f;
		rightX = part1centerX + 10.f;
		leftX = part1centerX - 20.f;

		//Part 1 troisieme tiers
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ 255.f, part1centerY - 10.f, 250.f }, XMFLOAT4{ 0.f, .3f, 0.f, 1.f }, { 15.f, 45.f, 30.f }));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ leftX, part1centerY - 10.f , 350.f }, m_baseRotation, { 20.f, 30.f, 10.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ part1centerX , upY, 380.f }, m_baseRotation, { 60.f, 20.f, 100.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ rightX +10.f , part1centerY - 15.f, 420.f }, XMFLOAT4{ 0.f, .3f, -.3f, 1.f }, { 20.f, 60.f, 10.f }));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDirt,
			XMFLOAT3{ leftX +10.f, part1centerY - 20.f, 580.f }, XMFLOAT4{ 0.f, .3f, .3f, 1.f }, { 20.f, 60.f, 10.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ part1centerX, part1centerY , 800.f }, m_baseRotation, { 10.f, 10.f, 150.f }));

		//Part 2
		float part2centerX = 230.f;
		float part2centerZ = 1000.f;

		rightX = part2centerX + 10.f;
		leftX = part2centerX - 10.f;
		float upZ = part2centerZ - 10.f;
		float downZ = part2centerZ + 10.f;

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ part2centerX, 200.f, part2centerZ }, m_baseRotation, { 10.f, 100.f, 10.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ rightX + 5.f, 200.f, part2centerZ }, m_baseRotation, { 20.f, 10.f, 30.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ part2centerX, 175.f, upZ - 5.f }, m_baseRotation, { 10.f, 10.f, 20.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ leftX, 400.f, upZ + 10.f }, m_baseRotation, { 10.f, 100.f, 50.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ rightX, 350.f, upZ  }, m_baseRotation, { 30.f, 20.f, 10.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ rightX, 400.f, downZ }, m_baseRotation, { 30.f, 20.f, 10.f }));

		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ leftX, 650.f, upZ }, m_baseRotation, { 10.f, 10.f, 10.f }));
		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ part2centerX, 675.f, downZ }, m_baseRotation, { 10.f, 10.f, 10.f }));
		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ rightX + 5.f, 700.f, upZ }, m_baseRotation, { 10.f, 10.f, 10.f }));
		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ part2centerX, 725.f, part2centerZ - 5.f}, m_baseRotation, { 10.f, 10.f, 10.f }));
		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ rightX, 750.f, downZ }, m_baseRotation, { 10.f, 10.f, 10.f }));
		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ part2centerX, 775.f, upZ }, m_baseRotation, { 10.f, 10.f, 10.f }));
		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ leftX - 5.f, 800.f, downZ }, m_baseRotation, { 10.f, 10.f, 10.f }));

		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ part2centerX, 950.f, part2centerZ }, m_baseRotation, { 100.f, 10.f, 100.f }));

		//Part 3
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ part2centerX, 1100.f, part2centerZ }, XMFLOAT4{ 0.f, .25f, 0.f, 1.f }, { 20.f, 10.f, 70.f }));
		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ part2centerX, 1150.f, part2centerZ }, XMFLOAT4{ 0.f, -.4f, 0.f, 1.f }, { 20.f, 10.f, 70.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ part2centerX, 1250.f, upZ }, m_baseRotation, { 90.f, 10.f, 30.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ leftX, 1325.f, downZ }, XMFLOAT4{ 0.f, -.4f, 0.f, 1.f }, { 80.f, 10.f, 50.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ rightX, 1500.f, upZ - 5.f }, XMFLOAT4{ 0.f, -.4f, 0.f, 1.f }, { 80.f, 200.f, 30.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ leftX, 1425.f, downZ - 5.f }, XMFLOAT4{ 0.f, .4f, 0.f, 1.f }, { 80.f, 10.f, 10.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ leftX, 1500.f, downZ + 12.f }, m_baseRotation, { 80.f, 10.f, 20.f }));

		m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
			XMFLOAT3{ leftX, 1500.f, downZ + 12.f }, m_baseRotation, { 80.f, 10.f, 20.f }));

		float angle = (XM_2PI) / 8;

		for (int i = 0; i < 8; i++) {
			m_obstacleInfoList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureSpace,
				XMFLOAT3(part2centerX + cos(angle * i) * 25, 1700.f + (40.f * i), part2centerZ + sin(angle * i) * 25), m_baseRotation, { 40.f, 60.f, 40.f }));
		}

		m_destructibleList.push_back(ObjectInfo(m_obstacleModelDirectory, m_obstacleModelFile, m_obstacleTextureDestructible,
			XMFLOAT3{ part2centerX, 2000.f, part2centerZ }, m_baseRotation, { 100.f, 10.f, 100.f }));
		break;
	}
}

void LevelCreator::initCollectibles()
{
	switch (m_level) {
	case Level_1:

		float part1centerX = -170.f;
		float part1centerY = -260.f;


		float rightX = part1centerX + 10.f;
		float leftX = part1centerX - 20.f;
		float upY = part1centerY + 10.f;
		float downY = part1centerY - 10.f;

		
		//Patounes
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX, part1centerY, -875.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX, part1centerY, -900.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX, part1centerY, -925.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, downY, -770.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, downY, -795.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, downY , -745.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, upY, -665.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, upY, -690.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, upY, -715.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX, part1centerY, -575.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX, part1centerY, -550.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX, part1centerY, -525.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//Part 1 deuxieme tiers
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ -70.f, upY, -180.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ 25.f, downY, -100.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ 65.f, part1centerY, -45.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ 90.f, part1centerY, -10.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ 120.f, part1centerY, 30.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		part1centerX = 230.f;
		rightX = part1centerX + 10.f;
		leftX = part1centerX - 10.f;

		//Part 1 troisieme tiers
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ 225.f, part1centerY, 250.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ 225.f, part1centerY, 275.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ 225.f, part1centerY, 225.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, downY, 325.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, downY, 375.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, downY, 350.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX , downY, 440.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX , downY, 465.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX , downY, 415.f }, m_baseRotation, m_patouneScale));
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, upY -10.f, 580.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, upY - 10.f, 605.f }, m_baseRotation, m_patouneScale));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, upY - 10.f, 555.f }, m_baseRotation, m_patouneScale));

		
		//Part 2
		float angle1 = (XM_2PI) / 20;
		float angle2 = (XM_2PI) / 10;

		for (int j = 1; j < 3; j++) {

			for (int i = 0; i < 10; i++) {
				m_antiCollectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_antiPatouneTexture,
					XMFLOAT3(part1centerX + cos(angle2 * i) * 15, part1centerY + sin(angle2 * i) * 15, 750.f + 50.f * j), m_baseRotation, m_patouneScale));
			}
			for (int i = 0; i < 20; i++) {
				m_antiCollectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_antiPatouneTexture,
					XMFLOAT3(part1centerX + cos(angle1 * i) * 25, part1centerY + sin(angle1 * i) * 25, 750.f + 50.f * j), m_baseRotation, m_patouneScale));
			}

		}

		float part2centerX = 230.f;
		float part2centerZ = 1000.f;

		rightX = part2centerX + 10.f;
		leftX = part2centerX - 10.f;
		float upZ = part2centerZ - 10.f;
		float downZ = part2centerZ + 10.f;

		XMFLOAT4 upRotation{ -1.f,0.f,0.f,1.f };
		PxVec3 axisRotation{ 0.f, 0.f, 3.f };

		float angle = (XM_2PI) / 8;

		for (int i = 0; i < 8; i++) {
			m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
				XMFLOAT3(part2centerX + cos(angle * i) * 15, 100.f + (25.f*i), part2centerZ + sin(angle * i) * 15), upRotation, m_patouneScale, axisRotation));
		}

		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, 325.f, downZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, 350.f, downZ }, upRotation, m_patouneScale, axisRotation));

		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, 400.f, upZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX, 420.f, upZ }, upRotation, m_patouneScale, axisRotation));

		//Part 3
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX - 15.f, 1100.f, part2centerZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX - 15.f, 1125.f, part2centerZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX - 15.f, 1075.f, part2centerZ }, upRotation, m_patouneScale, axisRotation));

		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX + 15.f, 1250.f, downZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX , 1225.f, downZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ leftX - 15.f, 1200.f, downZ }, upRotation, m_patouneScale, axisRotation));

		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX + 10.f, 1325.f, upZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX + 10.f, 1300.f, upZ }, upRotation, m_patouneScale, axisRotation));
		m_collectableList.push_back(ObjectInfo(m_patouneModelDirectory, m_patouneModelFile, m_patouneTexture,
			XMFLOAT3{ rightX + 10.f, 1350.f, upZ }, upRotation, m_patouneScale, axisRotation));

		break;
	}
}

void LevelCreator::initCourbe() {

	constexpr float heightPart1 = -260.f;

	m_3dcurvePoints.push_back(PxVec3{ -175.f, -240, -1000.f }); //Premier point non atteint
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.1242f, -0.69f, -0.12f, 0.69f }); //Premier quat

	m_3dcurvePoints.push_back(PxVec3{ -175.f, -240, -1000.f }); //Premier point non atteint
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.1242f, -0.69f, -0.12f, 0.69f }); //Premier quat

	m_3dcurvePoints.push_back(PxVec3{ -175.f, heightPart1, -800.f }); //Start
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.f, -0.7f, 0.f, 0.7f });

	m_3dcurvePoints.push_back(PxVec3{ -175.f, heightPart1, -500.f }); //Ligne droite
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.f, -0.7f, 0.f, 0.7f });

	m_3dcurvePoints.push_back(PxVec3{ -175.f, heightPart1, -360.f }); //Virage
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.f, -0.45f, 0.f, 0.9f });

	m_3dcurvePoints.push_back(PxVec3{ 0.f, heightPart1, -100.f }); //Mid bridge
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.f, -0.45f, 0.f, 0.9f });


	m_3dcurvePoints.push_back(PxVec3{ 145.f, heightPart1, 100.f }); //Avant virage
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.f, -0.45f, 0.f, 0.9f });

	m_3dcurvePoints.push_back(PxVec3{ 230.f, heightPart1, 230.f }); //Virage
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.f, -0.7f, 0.f, 0.7f });

	m_3dcurvePoints.push_back(PxVec3{ 230.f, heightPart1, 1000.f }); //Fin part 1
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ 0.f, -0.7f, 0.f, 0.7f });

	m_3dcurvePoints.push_back(PxVec3{ 230.f, 0.f, 1000.f }); //Debut part 2
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ -0.5f, -0.5f, 0.5f, 0.5f });

	m_3dcurvePoints.push_back(PxVec3{ 230.f, 1000.f, 1000.f }); //Entree asteroide
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ -0.5f, -0.5f, 0.5f, 0.5f });

	m_3dcurvePoints.push_back(PxVec3{ 230.f, 2000.f, 1000.f }); //Sortie asteroide
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ -0.5f, -0.5f, 0.5f, 0.5f });

	m_3dcurvePoints.push_back(PxVec3{ 230.f, 2200.f, 1000.f }); //Dernier point non atteint
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ -0.5f, -0.5f, 0.5f, 0.5f });

	m_3dcurvePoints.push_back(PxVec3{ 230.f, 2200.f, 1000.f }); //Dernier point non atteint
	m_3dcurveQuats.push_back(DirectX::XMVECTOR{ -0.5f, -0.5f, 0.5f, 0.5f });

	m_3dCurve = Courbe3D(m_3dcurvePoints, m_3dcurveQuats);
}
void LevelCreator::initLights() {

	switch (m_level) {
	case Level_1:
		////planet light
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 3000.f, -1000.f, -2000.f ,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.1f, 0.0f, 0.0f,1.f }, { 1.f, 0.f, 0.f,1.f }, 1000.f, 3000.f));
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ -2000.f, -100.f, -2000.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.95f, 0.6f, 0.2f,1.f }, { .95f, 0.6f, 0.2f,1.f }, 2000.0f, 4000.0f));

		////in the tunnel

	float range1, range2;
	range1 = 100.0f;
	range2 = 250.0f;

	//m_lightInfoList.push_back(LevelCreator::LightInfo({ -1500.f, -2000.f, 2000.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.8f, 0.2f,1.f }, { .6f, 0.6f, 0.4f,1.f }, 100.0f, 700.0f));
	//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -3500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.7f, 0.2f, 0.6f,1.f }, { .8f, 0.4f, 0.6f,1.f }, range1, range2));
	m_lightInfoList.push_back(LevelCreator::LightInfo({ 230.f, 1500.f, 1000.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 1.0f, 0.6f, 0.0f,1.f }, {1.0f, 0.6f, 0.0f,1.f }, range1, range2));
	m_lightInfoList.push_back(LevelCreator::LightInfo({ -175.f, -253.f, -800.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 0.0f, 0.0f, 0.4f,1.f }, {0.0f, 0.0f, 0.4f,1.f }, 20.0f, 400.0f));
	m_lightInfoList.push_back(LevelCreator::LightInfo({ 270.f, 750.f, 1010.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, {0.8f, 0.0f, 0.0f,1.f }, 20.0f, 400.0f));
	//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -7500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.2f, 0.2f,1.f }, { .9f, 0.4f, 0.4f,1.f }, range1, range2));
	//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -9500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 1.f, 0.2f, 1.f,1.f }, { 1.f, 0.4f, 1.f,1.f }, range1, range2));
	//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -11500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.2f, 0.2f, 1.f,1.f }, { .4f, 0.4f, 0.8f,1.f }, range1, range2));
	//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -13500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 1.f, 1.f, 0.2f,1.f }, { 1.f, 1.f, 0.4f,1.f }, range1, range2));

		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, 120.f, -2000.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.8f, 0.2f,1.f }, { .6f, 0.6f, 0.4f,1.f }, 100.0f, 700.0f));
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -3500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.7f, 0.2f, 0.6f,1.f }, { .8f, 0.4f, 0.6f,1.f }, 100.0f, 700.0f));
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -4500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.2f, 0.7f, 0.8f,1.f }, { .4f, 0.7f, 0.8f,1.f }, 100.f, 700.f));
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -5500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.2f, 0.2f,1.f }, { .9f, 0.4f, 0.4f,1.f }, 100.f, 700.f));
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -6500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 1.f, 0.2f, 1.f,1.f }, { 1.f, 0.4f, 1.f,1.f }, 100.f, 700.f));
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -7500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 0.2f, 0.2f, 1.f,1.f }, { .4f, 0.4f, 0.8f,1.f }, 100.f, 700.f));
		//m_lightInfoList.push_back(LevelCreator::LightInfo({ 2240.f, -1300.f, -8500.f,1.f }, { 0.f, 0.f, 0.f,1.f }, { 1.f, 1.f, 0.2f,1.f }, { 1.f, 1.f, 0.4f,1.f }, 100.f, 700.f));
		
		//Tunnel
		m_lightInfoList.push_back(LevelCreator::LightInfo({ 230.f, 950.f, 1000.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, 20.0f, 400.0f));
		m_lightInfoList.push_back(LevelCreator::LightInfo({ 230.f, 1100.f, 1000.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, 20.0f, 400.0f));
		m_lightInfoList.push_back(LevelCreator::LightInfo({ 230.f, 1400.f, 1000.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, 20.0f, 400.0f));
		m_lightInfoList.push_back(LevelCreator::LightInfo({ 230.f, 1700.f, 1000.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, 20.0f, 400.0f));
		m_lightInfoList.push_back(LevelCreator::LightInfo({ 230.f, 1950.f, 1000.f ,0.f }, { 0.f, 0.f, 0.f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, { 0.8f, 0.0f, 0.0f,1.f }, 20.0f, 400.0f));
		break;
	}
}
