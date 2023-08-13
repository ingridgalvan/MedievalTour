////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: PositionClass
////////////////////////////////////////////////////////////////////////////////
class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	//Enemy Delta
	void GetPositionDelta(float&, float&, float&);
	void SetPositionDelta(float, float, float);

	void GetRotationDelta(float&, float&, float&);
	void SetRotationDelta(float, float, float);

	//Enemy Torret
	void GetPositionTorret(float&, float&, float&);
	void SetPositionTorret(float, float, float);

	void GetRotationTorret(float&, float&, float&);
	void SetRotationTorret(float, float, float);

	//Enemy Boss
	void GetPositionEnemyBoss(float&, float&, float&);
	void SetPositionEnemyBoss(float, float, float);

	void GetRotationEnemyBoss(float&, float&, float&);
	void SetRotationEnemyBoss(float, float, float);

	//Enemy Boss Animation
	void GetPositionAnimationBoss(float& x, float& y, float& z, int coord);
	void ChangeAnimationBossControl(int op);
	bool GetAnimationBossControl(int op);

	//Car
	void GetPositionCar(float& x, float& y, float& z, int coord);
	void ChangeAnimationCarControl(int op);
	bool GetAnimationCarControl(int op);

	void GetCameraType(bool&);

	void SetFrameTime(float);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);
	void ChangeCameraType(bool);

	void ResetStructurePosition();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	float m_positionDeltaX, m_positionDeltaY, m_positionDeltaZ;
	float m_rotationDeltaX, m_rotationDeltaY, m_rotationDeltaZ;

	float m_positionTorretX, m_positionTorretY, m_positionTorretZ;
	float m_rotationTorretX, m_rotationTorretY, m_rotationTorretZ;

	float m_positionEnemyBossX, m_positionEnemyBossY, m_positionEnemyBossZ;
	float m_rotationEnemyBossX, m_rotationEnemyBossY, m_rotationEnemyBossZ;

	float m_positionCarX, m_positionCarY, m_positionCarZ;
	bool m_ctrlAnimCar[2];

	float m_ctrlAnimBossX, m_ctrlAnimBossY, m_ctrlAnimBossZ;
	bool m_ctrlAnimBossBool[2];

	float m_frameTime;

	float m_forwardSpeed, m_backwardSpeed;
	float m_upwardSpeed, m_downwardSpeed;
	float m_leftTurnSpeed, m_rightTurnSpeed;
	float m_lookUpSpeed, m_lookDownSpeed;
	bool m_cameraType;
};

#endif