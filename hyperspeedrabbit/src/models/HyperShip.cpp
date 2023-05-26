#include "stdafx.h"
#include "HyperShip.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Courbe3D.h"
#include "MoteurWindows.h"
#include <src\Converter.h>

using namespace physx;

HyperShip::HyperShip(PM3D::CDispositifD3D11* _pDispositif, PxPhysics* _physics, PxScene* _scene, PxMaterial* _material, Courbe3D _courbe, DirectX::XMFLOAT3 _position, DirectX::XMFLOAT4)
	: Objet3DPhysique(_position, { 0.0f, -1.0f, 0.0f, 0.0f }, { 6.f, 6.f, 6.f }, 0.002f/*0.00002f*/)
{
	m_curve3D = _courbe;
	m_curve = m_curve3D.getCurve();

	m_nbPointsQuat = m_curve3D.getNbPoints();

	m_savedCurve = m_curve;
	m_savedCurveNot = m_curve;

	currentpos = caster<PxVec3>(_position);
	setQuaternionIndices();
	
	m_currentIndex++;
	
	std::reverse(m_savedCurveNot.begin(), m_savedCurveNot.end());

	updateQuaternions();
	// TO DO : MAKE SURE THERE IS AT LEAST TWO POINTS.
	m_position = caster<XMFLOAT3>(m_curve.back().getPosition());
	m_rotation = caster<XMFLOAT4>(m_curve.back().getRotation());
	m_curve.pop_back();
	m_nextPosition = m_curve.back().getPosition();
	//m_rotation = XMVECTORToXMFLOAT4(m_curve.back().getR);

	//     PHYSIQUE     //
	PxShape* shipBox = _physics->createShape(PxBoxGeometry(m_scale.x/2,m_scale.y/2,m_scale.z/2), *_material, true);
	PxQuat rotation = caster<PxQuat>(m_rotation).getNormalized();
	m_actor = PxCreateDynamic(*_physics, PxTransform(m_position.x, m_position.y, m_position.z, rotation), *shipBox, 10.0f);
	
	_scene->addActor(*m_actor);
	m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	//     GRAPHIQUE       //
	PM3D::CChargeurOBJ& chargeurObj = ResourceManager::get().getChargeur("assets/modeles/Ships/","PointyShip2.obj");
	m_meshs.emplace_back(std::make_unique<PM3D::CObjetMesh>(chargeurObj, _pDispositif, "assets/modeles/Ships/", "PointyShip2"));

	m_anchor = caster<PxVec3>(m_position);
	m_baseX = m_actor->getGlobalPose().q.getBasisVector2();
	m_baseY = m_actor->getGlobalPose().q.getBasisVector1();

	m_initCoeffSpeed = m_coeffSpeed;

	m_speed = 1.f;

	m_nbGoodCollectables = 0;
	m_nbBadCollectables = 0;

}

void HyperShip::updateSpeed() {

	PM3D::InputManager& inputManager = PM3D::InputManager::get();

	if (!getIsLocked())
	{
		if (inputManager.isPressed(DIK_SPACE) && !m_isSlowedDown)
			m_coeffSpeed *= 1.05f;
		else if (inputManager.isPressed(DIK_LSHIFT) && !m_isSlowedDown)
			m_coeffSpeed *= 0.95f;
	}
	//m_coeffSpeed = ((inputManager.isPressed(DIK_SPACE) && (!m_isSlowedDown)) ? m_coeffSpeed * 1.01f : m_coeffSpeed * 0.95f);
	m_coeffSpeed = (m_coeffSpeed <= m_minSpeed ? m_minSpeed : m_coeffSpeed);
	m_coeffSpeed = (m_coeffSpeed > m_maxSpeed ? m_maxSpeed : m_coeffSpeed);
}

void HyperShip::update([[maybe_unused]] float _tempEcoule) {
	
	//save the position & the rotation of the ship for the ghost
	savePosition();


	loadingMissile();

	if (getIsMoving())
	{
		m_anchor = m_actor->getGlobalPose().p - m_baseX * m_hMovement - m_baseY * m_vMovement;
		m_actor->setGlobalPose(PxTransform(m_anchor, m_actor->getGlobalPose().q));

		if (!getIsLocked())
			moveInPlane();

		if (!m_curve.empty())
		{
			/////////////// Etape deplacement
			PxVec3 pos = m_actor->getGlobalPose().p;
			//m_actor->setGlobalPose(PxTransform(0, 50, 0));//m_actor->getGlobalPose().p;
			PxVec3 savedPos = pos;
			PxVec3 currentDir = m_actor->getGlobalPose().q.getBasisVector0();
			PxVec3 dirTarget = m_nextPosition - pos;
			dirTarget = dirTarget.getNormalized();

			updateSpeed();	
		
		pos = m_actor->getGlobalPose().p;

			////////////////////// Etape update de la courbe

			// norme restante
			float reste = (0.01*m_speed * dirTarget * m_coeffSpeed).magnitude();
			float normCurrent;
			PxVec3 currentPos;
			currentPos = m_actor->getGlobalPose().p;

			if (reste < 0.01f)
			{
				m_curve.pop_back();
				if (!m_curve.empty())
				{
					m_nextPosition = m_curve.back().getPosition();
					m_currentIndex++;
				}
			}
			else {
				while (reste > 0)
				{
					normCurrent = (m_nextPosition - savedPos).magnitude();

					if (reste >= normCurrent) {
						m_actor->setGlobalPose(PxTransform(m_actor->getGlobalPose().p + (m_nextPosition - savedPos), m_actor->getGlobalPose().q));
						currentPos = m_actor->getGlobalPose().p;
						savedPos = m_nextPosition;
						m_curve.pop_back();
						if (!m_curve.empty())
						{
							m_nextPosition = m_curve.back().getPosition();
							m_currentIndex++;
						}
						else
							break;
						reste -= normCurrent;
					}
					else {
						m_actor->setGlobalPose(PxTransform(m_actor->getGlobalPose().p + (m_nextPosition - savedPos).getNormalized() * reste, m_actor->getGlobalPose().q));
						currentPos = m_actor->getGlobalPose().p;
						break;
					}

				}
			}
			currentPos = m_actor->getGlobalPose().p;
			updateQuaternions();
			// S'il reste de la normetotale � prendre
			//// Calculer la norme de la portion du circuit sur laquelle on se trouve
			//// Si cette norme est sup�rieure � reste de normetotale
			//////// Se d�placer de reste normetotale dans la direction norme portion
			//////// Se placer dans la portion suivante
			//// Sinon
			//////// Se d�placer de norme portion

			/////////////////// Etape rotation

			if (m_curve.size() > 1 && m_currentIndex > 0) {

				PxVec3 const _prevPosition = m_savedCurveNot.at(m_currentIndex - 1).getPosition();
				float const _prevT = m_savedCurveNot.at(m_currentIndex - 1).getTime();
				float const _nextT = m_savedCurveNot.at(m_currentIndex).getTime();

				dirTarget = m_nextPosition - m_actor->getGlobalPose().p;
				dirTarget = dirTarget.getNormalized();

				double const distAV = (m_actor->getGlobalPose().p - _prevPosition).magnitude();
				double const distVB = (m_nextPosition - m_actor->getGlobalPose().p).magnitude();
				double const distAB = (m_nextPosition - _prevPosition).magnitude();

				double const t = (distAV / distAB) * (_nextT - _prevT) + _prevT;
				setT(t);

				XMVECTOR const q1 = DirectX::XMQuaternionNormalize(m_prevQuaternion);
				XMVECTOR const q2 = DirectX::XMQuaternionNormalize(m_nextQuaternion);

				PxQuat const q = caster<PxQuat>(DirectX::XMQuaternionSlerp(q1, q2, t)).getNormalized();
				//PxQuat(acos(currentDir.dot(dirTarget)) / 30.f, currentDir.cross(dirTarget).getNormalized());
				m_actor->setGlobalPose(PxTransform(m_actor->getGlobalPose().p, q));

				m_baseX = m_actor->getGlobalPose().q.getBasisVector2();
				m_baseY = m_actor->getGlobalPose().q.getBasisVector1();
			}

		}

		m_actor->setGlobalPose(PxTransform(m_actor->getGlobalPose().p + m_vMovement * m_baseY, m_actor->getGlobalPose().q));
		m_actor->setGlobalPose(PxTransform(m_actor->getGlobalPose().p + m_hMovement * m_baseX, m_actor->getGlobalPose().q));
	}
	else {

		rotate();
		PxQuat temp_quat = m_actor->getGlobalPose().q;

		float factor = sin(m_xRotation/100 / 2.0);

		PxQuat x_quat = PxQuat{ factor,0,0, static_cast<float>(cos(m_xRotation / 2.0)) }.getNormalized();
		factor = sin(m_yRotation/100 / 2.0);
		PxQuat y_quat = PxQuat{ 0,factor,0,  static_cast<float>(cos(m_yRotation / 2.0)) }.getNormalized();
		factor = sin(m_zRotation/100 / 2.0);
		PxQuat z_quat = PxQuat{ 0,0,factor, static_cast<float>(cos(m_zRotation / 2.0)) }.getNormalized();

		temp_quat = (temp_quat * x_quat * y_quat * z_quat).getNormalized();
	
		m_actor->setGlobalPose(PxTransform(m_actor->getGlobalPose().p, temp_quat));

		m_xRotation = 0;
		m_yRotation = 0;
		m_zRotation = 0;

		m_baseX = m_actor->getGlobalPose().q.getBasisVector2();
		m_baseY = m_actor->getGlobalPose().q.getBasisVector1();
	}
	currentpos = m_actor->getGlobalPose().p;
}

float HyperShip::updateSlowDown() {
	currentTime = std::chrono::high_resolution_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count() >= 3000) {
		setIsSlowedDown(false);
		return 0.f;
	}
	else
		return (3000.f - std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count())/3000.f;
}

void HyperShip::startLoadingMissile() noexcept
{
	m_readyToshoot = false;
	m_missileTime = std::chrono::high_resolution_clock::now();
}

void HyperShip::loadingMissile() noexcept
{
	auto timeLoading = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::high_resolution_clock::now() - m_missileTime).count();

	if (std::move(timeLoading) >= m_missileTimeLoading)
	{
		m_readyToshoot = true;
		PM3D::CMoteurWindows::get().setCursorTargetAvailable();
	}
}

void HyperShip::reset() noexcept {
	m_curve = m_savedCurve;
	m_position = caster<XMFLOAT3>(m_curve.back().getPosition());
	m_rotation = caster<XMFLOAT4>(m_curve.back().getRotation());
	m_curve.pop_back();
	m_nextPosition = m_curve.back().getPosition();
	m_currentIndex = 1;

	m_actor->setGlobalPose(PxTransform(m_position.x, m_position.y, m_position.z, caster<PxQuat>(m_rotation)));

	m_actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_anchor = caster<PxVec3>(m_position);
	m_baseX = m_actor->getGlobalPose().q.getBasisVector2();
	m_baseY = m_actor->getGlobalPose().q.getBasisVector1();

	m_coeffSpeed = m_initCoeffSpeed;
	m_nbGoodCollectables = 0;
	m_nbBadCollectables = 0;
	m_hMovement = 0.f;
	m_vMovement = 0.f;
	m_coeffSpeed = 1.0f;
	m_savedCurveGhost.clear();
	m_isSlowedDown = false;
}

void HyperShip::updateInerBooleans(bool& _lastTurn, int& _compteur, bool& _stopped) {
	if (_lastTurn)
	{
		_compteur = 0;
		_stopped = false;
	}
	else if (_compteur == m_nbFramesInr)
	{
		_compteur = 0;
		_stopped = true;
	}
	else if (!_stopped) _compteur++;

}

void HyperShip::calculateInertia() {

	updateInerBooleans(upLT, m_compteurUp, stoppedU);
	updateInerBooleans(downLT, m_compteurDown, stoppedD);
	updateInerBooleans(leftLT, m_compteurLeft, stoppedL);
	updateInerBooleans(rightLT, m_compteurRight, stoppedR);

	if (m_compteurUp != 0)
		m_vMovement += (m_nbFramesInr - m_compteurUp) / m_coeffInertia;
	if (m_compteurDown != 0)
		m_vMovement -= (m_nbFramesInr - m_compteurDown) / m_coeffInertia;
	if (m_compteurLeft != 0)
		m_hMovement += (m_nbFramesInr - m_compteurLeft) / m_coeffInertia;
	if (m_compteurRight != 0)
		m_hMovement -= (m_nbFramesInr - m_compteurRight) / m_coeffInertia;

	m_hMovement = min(m_hMovement, m_hMovementMax);
	m_hMovement = max(m_hMovement, m_hMovementMin);
	m_vMovement = min(m_vMovement, m_vMovementMax);
	m_vMovement = max(m_vMovement, m_vMovementMin);
}

void HyperShip::moveInPlane()
{
	if (m_isSlowedDown)
		updateSlowDown();

	PM3D::InputManager& inputManager = PM3D::InputManager::get();

	// Up
	if ((inputManager.isPressed(DIK_W) && m_vMovement <= m_vMovementMax))
	{
		m_vMovement+= m_speedOnScreen;
		upLT = true;
		stoppedU = false;
	}
	// Down
	if ((inputManager.isPressed(DIK_S) && m_vMovement >= m_vMovementMin))
	{
		m_vMovement-= m_speedOnScreen;
		downLT = true;
		stoppedD = false;
	}
	// Left
	if ((inputManager.isPressed(DIK_A) && m_hMovement <= m_hMovementMax))
	{
		m_hMovement+= m_speedOnScreen;
		leftLT = true;
		stoppedL = false;
	}

	// Right
	if ((inputManager.isPressed(DIK_D) && m_hMovement >= m_hMovementMin))
	{
		m_hMovement-= m_speedOnScreen;
		rightLT = true;
		stoppedR = false;
	}

	calculateInertia();
	leftLT = rightLT = upLT = downLT = false;
}

void HyperShip::rotate()
{

	PM3D::InputManager& inputManager = PM3D::InputManager::get();

	// Up
	if (inputManager.isPressed(DIK_W))
		m_zRotation++;

	// Down
	if (inputManager.isPressed(DIK_S))
		m_zRotation--;

	// Left
	if (inputManager.isPressed(DIK_A))
		m_yRotation--;

	// Right
	if (inputManager.isPressed(DIK_D))
		m_yRotation++;

	// Right
	if (inputManager.isPressed(DIK_Q))
		m_xRotation++;

	// Right
	if (inputManager.isPressed(DIK_E))
		m_xRotation--;
}

void HyperShip::setQuaternionIndices()
{
	for (size_t i = 0; i < m_savedCurveNot.size(); i++)
	{
		if (m_savedCurveNot[i].hasRotation())
			m_indicesQuaternions.push_back(i);
	}
}

void HyperShip::updateQuaternions()
{
	m_prevQuaternion = m_savedCurveNot[0].getRotation();
	int max = 0;
	int min = m_savedCurveNot.size() - 1;
	m_nextQuaternion = m_savedCurveNot.back().getRotation();

	for (size_t i = 0; i < m_savedCurveNot.size(); i++)
	{
		if (m_savedCurveNot[i].hasRotation()) {
			if (i <= m_currentIndex && i > max) {
				m_prevQuaternion = m_savedCurveNot[i].getRotation();
				max = i;
			}
			if (i > m_currentIndex && i < min) {
				m_nextQuaternion = m_savedCurveNot[i].getRotation();
				min = i;
			}
		}
	}
}

float HyperShip::getProgression() const noexcept
{
	return static_cast<float>(m_currentIndex) / m_savedCurve.size();
}

void HyperShip::setIsSlowedDown(bool status) noexcept
{
	if (m_isSlowedDown!=status && status)
		m_coeffSpeed = min(m_maxSpeed, max(m_minSpeed, m_coeffSpeed / 2));
	m_isSlowedDown = status;
}

int HyperShip::getNbGoodCollectables() const noexcept
{
	return m_nbGoodCollectables;
}

int HyperShip::getNbBadCollectables() const noexcept
{
	return m_nbBadCollectables;
}

void HyperShip::addNbGoodCollectables(const int _nb) noexcept
{
	m_nbGoodCollectables += _nb;
}

void HyperShip::addNbBadCollectables(const int _nb) noexcept
{
	m_nbBadCollectables += _nb;
}

void HyperShip::savePosition() {
	m_savedCurveGhost.emplace_back(
		PointInfo(physx::PxVec3{ getPosition().x,getPosition().y,getPosition().z }, -1.f ,{ getRotation().x , getRotation().y, getRotation().z, getRotation().w}, false)
	);
}