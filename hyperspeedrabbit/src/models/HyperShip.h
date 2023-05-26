#pragma once
#include "Objet3DPhysique.h"
#include "CatmullRom.h"
#include <chrono>
#include "src/Converter.h"

class HyperShip : public Objet3DPhysique {
public:
	HyperShip(PM3D::CDispositifD3D11* _pDispositif,
		physx::PxPhysics* _physics,
		physx::PxScene* _scene,
		physx::PxMaterial* _material,
		Courbe3D _courbe,
		DirectX::XMFLOAT3 _position = { -43, -50, 257},
		DirectX::XMFLOAT4 _rotation = {0.f,0.f,0.f,1.f});

	virtual ~HyperShip() = default;
	void updateSpeed();
	void update(float _tempEcoule) override;
	float updateSlowDown();
	void reset()noexcept;
	void updateInerBooleans(bool& _lastTurn, int& _compteur, bool& _stopped);
	void calculateInertia();
	void moveInPlane();

	void rotate();

	physx::PxVec3 getAnchor() const { return m_anchor; }
	physx::PxVec3 getBaseY() const { return m_baseY; }
	physx::PxVec3 getBaseX() const { return m_baseX; }

	bool isArrived() const noexcept { return m_curve.empty(); }

	DirectX::XMFLOAT4 getRotationQuaternion() const {
		physx::PxQuat quat = m_actor->getGlobalPose().q;
		XMFLOAT4 xmfloat = caster<DirectX::XMFLOAT4>(quat);
		return xmfloat ;
	}

	void setQuaternionIndices();
	void updateQuaternions();
	float getSpeed() const noexcept { return m_coeffSpeed;  }
	float getCoeffSpeed() const noexcept { return m_coeffSpeed; }
	float getCoeffMaxSpeed() const noexcept { return m_maxSpeed; }
	float getCoeffMinSpeed() const noexcept { return m_minSpeed; }
	float getProgression() const noexcept;
	bool readyToShoot() const noexcept { return m_readyToshoot; }

	void setIsSlowedDown(bool status) noexcept;
	bool getIsSlowedDown() noexcept { return m_isSlowedDown; }
	void startTimerSlowDown() { previousTime = std::chrono::high_resolution_clock::now(); }

	void setIsMoving(bool _val) noexcept { m_isMoving = _val; }
	bool getIsMoving() noexcept { return m_isMoving; }

	void setT(double _t) noexcept { t = _t; }
	double getT() noexcept { return t; }

	void setNbPointsQuat(int _nb) { m_nbPointsQuat = _nb; }
	int getNbPointsQuat() { return m_nbPointsQuat; }

	void startLoadingMissile() noexcept;
	void loadingMissile() noexcept;

	int getNbGoodCollectables() const noexcept;
	int getNbBadCollectables() const noexcept;

	void addNbGoodCollectables(const int _nb = 1) noexcept;
	void addNbBadCollectables(const int _nb = 1) noexcept;

	void savePosition();

	std::vector<PointInfo> getSaveCurbe() const noexcept { return m_savedCurveGhost; }

	void setIsLocked(bool _val) { m_isLocked = _val; }
	bool getIsLocked() { return m_isLocked; }

private:

	physx::PxVec3 m_anchor;
	physx::PxVec3 m_baseX; // base X du plan (pas du vaisseau !!)
	physx::PxVec3 m_baseY; // base Y du plan (pas du vaisseau !!)

	float m_speedOnScreen = 0.9f;

	float m_hMovement = 0.f;
	float m_hMovementMin = -25.5f;
	float m_hMovementMax = 24.7f;
	
	float m_vMovement = 0.f;
	float m_vMovementMin = -12.4f;
	float m_vMovementMax = 16.0f;

	float m_coeffSpeed{ 33.651907995f };

	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point previousTime;

	bool m_isSlowedDown{ false };
	bool m_transitionSpeed{ false };

	float m_minSpeed{ 10.5f };
	float m_maxSpeed{ 200.0f };

	std::vector<PointInfo> m_curve;
	std::vector<PointInfo> m_savedCurve;
	std::vector<PointInfo> m_savedCurveNot;

	std::vector<PointInfo> m_savedCurveGhost;

	std::vector<int> m_indicesQuaternions;
	int m_currentIndex{};

	double t{};
	DirectX::XMVECTOR m_prevQuaternion;
	DirectX::XMVECTOR m_nextQuaternion;

	bool m_isMoving{ true };
	bool m_isLocked{ false };

	int m_nbPointsQuat;

	float m_xRotation{};
	float m_yRotation{};
	float m_zRotation{};

	bool upLT{ false };
	bool downLT{ false };
	bool leftLT{ false };
	bool rightLT{ false };

	bool stoppedL{ true };
	bool stoppedR{ true };
	bool stoppedU{ true };
	bool stoppedD{ true };

	int m_nbFramesInr = 12;
	float m_coeffInertia = 20.f;
	int m_compteurUp{}, m_compteurRight{}, m_compteurLeft{}, m_compteurDown{};
	physx::PxVec3 currentpos;

	// Missile
	bool m_readyToshoot = true;
	long long m_missileTimeLoading = 500;
	std::chrono::steady_clock::time_point m_missileTime;

	int m_nbGoodCollectables;
	int m_nbBadCollectables;
	float m_initCoeffSpeed;
};