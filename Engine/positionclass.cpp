////////////////////////////////////////////////////////////////////////////////
// Filename: positionclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "positionclass.h"


PositionClass::PositionClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	//Delta Enemy
	m_positionDeltaX = 0.0f;
	m_positionDeltaY = 0.0f;
	m_positionDeltaZ = 0.0f;
	m_rotationDeltaX = 0.0f;
	m_rotationDeltaY = 0.0f;
	m_rotationDeltaZ = 0.0f;

	//Torret Enemy
	m_positionTorretX = 0.0f;
	m_positionTorretY = 0.0f;
	m_positionTorretZ = 0.0f;
	m_rotationTorretX = 0.0f;
	m_rotationTorretY = 0.0f;
	m_rotationTorretZ = 0.0f;

	//Boss Enemy
	m_positionEnemyBossX = 0.0f;
	m_positionEnemyBossY = 0.0f;
	m_positionEnemyBossZ = 0.0f;
	m_rotationEnemyBossX = 0.0f;
	m_rotationEnemyBossY = 0.0f;
	m_rotationEnemyBossZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_frameTime = 0.0f;

	m_forwardSpeed   = 0.0f;
	m_backwardSpeed  = 0.0f;
	m_upwardSpeed    = 0.0f;
	m_downwardSpeed  = 0.0f;
	m_leftTurnSpeed  = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_lookUpSpeed    = 0.0f;
	m_lookDownSpeed  = 0.0f;

	m_positionCarX = 0.0f;
	m_positionCarY = 0.0f;
	m_positionCarZ = 0.0f;
	m_ctrlAnimCar[0] = false;
	m_ctrlAnimCar[1] = false;

	m_ctrlAnimBossX = 0.0f;
	m_ctrlAnimBossY = 0.0f;
	m_ctrlAnimBossZ = 0.0f;
	m_ctrlAnimBossBool[0] = false;
	m_ctrlAnimBossBool[1] = false;


	m_cameraType = true;
}


PositionClass::PositionClass(const PositionClass& other)
{
}


PositionClass::~PositionClass()
{
}


void PositionClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void PositionClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}

void PositionClass::GetCameraType(bool& type)
{
	type = m_cameraType;
	return;
}


void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


void PositionClass::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_forwardSpeed += m_frameTime * 0.001f;
		//m_forwardSpeed += m_frameTime * 100.0f;
		if(m_forwardSpeed > (m_frameTime * 0.01f))
		{
			m_forwardSpeed = m_frameTime * 0.01f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if(m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}


void PositionClass::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_backwardSpeed += m_frameTime * 0.001f;

		if(m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;
		
		if(m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

	return;
}


void PositionClass::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_upwardSpeed += m_frameTime * 0.003f;

		if(m_upwardSpeed > (m_frameTime * 0.03f))
		{
			m_upwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_upwardSpeed -= m_frameTime * 0.002f;

		if(m_upwardSpeed < 0.0f)
		{
			m_upwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY += m_upwardSpeed;

	return;
}


void PositionClass::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_downwardSpeed += m_frameTime * 0.003f;

		if(m_downwardSpeed > (m_frameTime * 0.03f))
		{
			m_downwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_downwardSpeed -= m_frameTime * 0.002f;

		if(m_downwardSpeed < 0.0f)
		{
			m_downwardSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionY -= m_downwardSpeed;

	return;
}


void PositionClass::TurnLeft(bool keydown)
{
	 /*Update the left turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if(m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime* 0.005f;

		if(m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	 Update the rotation.
	m_rotationY -= m_leftTurnSpeed;

	 Keep the rotation in the 0 to 360 range.
	if(m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}*/
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.003f;

		if (m_leftTurnSpeed > (m_frameTime * 0.03f))
		{
			m_leftTurnSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.002f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionX -= m_leftTurnSpeed;

	return;
}


void PositionClass::TurnRight(bool keydown)
{
	/*/ Update the right turn speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if(m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime* 0.005f;

		if(m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationY += m_rightTurnSpeed;

	// Keep the rotation in the 0 to 360 range.
	if(m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}*/

	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.003f;

		if (m_rightTurnSpeed > (m_frameTime * 0.03f))
		{
			m_rightTurnSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 0.002f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the height position.
	m_positionX += m_rightTurnSpeed;

	return;
}


void PositionClass::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if(m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime* 0.005f;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}


void PositionClass::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if(m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* 0.005f;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}

void PositionClass::ChangeCameraType(bool keydown)
{
	if (keydown)
	{
		m_cameraType = !m_cameraType;
	}

	return;
}

//EnemyProgramming
void PositionClass::SetPositionDelta(float x, float y, float z)
{
	m_positionDeltaX = x;
	m_positionDeltaY = y;
	m_positionDeltaZ = z;
	
	return;
}


void PositionClass::GetPositionDelta(float& x, float& y, float& z)
{
	x = m_positionDeltaX;
	y = m_positionDeltaY;
	z = m_positionDeltaZ;
	return;
}


void PositionClass::SetRotationDelta(float x, float y, float z)
{
	m_rotationDeltaX = x;
	m_rotationDeltaY = y;
	m_rotationDeltaZ = z;
	return;
}



void PositionClass::GetRotationDelta(float& x, float& y, float& z)
{
	x = m_rotationDeltaX;
	y = m_rotationDeltaY;
	z = m_rotationDeltaZ;
	return;
}

//TorretProgramming
void PositionClass::SetPositionTorret(float x, float y, float z)
{
	m_positionTorretX = x;
	m_positionTorretY = y;
	m_positionTorretZ = z;

	return;
}


void PositionClass::GetPositionTorret(float& x, float& y, float& z)
{
	x = m_positionTorretX;
	y = m_positionTorretY;
	z = m_positionTorretZ;
	return;
}


void PositionClass::SetRotationTorret(float x, float y, float z)
{
	m_rotationTorretX = x;
	m_rotationTorretY = y;
	m_rotationTorretZ = z;
	return;
}



void PositionClass::GetRotationTorret(float& x, float& y, float& z)
{
	x = m_rotationTorretX;
	y = m_rotationTorretY;
	z = m_rotationTorretZ;
	return;
}

//--Car move

void PositionClass::GetPositionCar(float& x, float& y, float& z, int coord)
{

	switch (coord)
	{
	case 0:
		m_positionCarX += m_frameTime * 0.001;
		break;
	case 1:
		m_positionCarY += m_frameTime * 0.001;
		break;
	case 2:
		m_positionCarZ += m_frameTime * 0.001;
		break;
	case 3:
		m_positionCarX -= m_frameTime * 0.001;
		break;
	case 4:
		m_positionCarY -= m_frameTime * 0.001;
		break;
	case 5: 
		m_positionCarZ -= m_frameTime * 0.001;
		break;
	default:
		break;
	}

	x = m_positionCarX;
	y = m_positionCarY;
	z = m_positionCarZ;
	return;
}

void PositionClass::ChangeAnimationCarControl(int op) {
	m_ctrlAnimCar[op] = !m_ctrlAnimCar[op];
	//return m_ctrlAnimCar[op];
}

bool PositionClass::GetAnimationCarControl(int op) {
	//m_ctrlAnimCar[op] = !m_ctrlAnimCar[op];
	return m_ctrlAnimCar[op];
}

//EnemyBoss Programming
void PositionClass::SetPositionEnemyBoss(float x, float y, float z)
{
	m_positionEnemyBossX = x;
	m_positionEnemyBossY = y;
	m_positionEnemyBossZ = z;
	return;
}


void PositionClass::GetPositionEnemyBoss(float& x, float& y, float& z)
{
	x = m_positionEnemyBossX;
	y = m_positionEnemyBossY;
	z = m_positionEnemyBossZ;
	return;
}


void PositionClass::SetRotationEnemyBoss(float x, float y, float z)
{
	m_rotationEnemyBossX = x;
	m_rotationEnemyBossY = y;
	m_rotationEnemyBossZ = z;
	return;
}



void PositionClass::GetRotationEnemyBoss(float& x, float& y, float& z)
{
	x = m_rotationEnemyBossX;
	y = m_rotationEnemyBossY;
	z = m_rotationEnemyBossZ;
	return;
}



//--Enemy Boss

void PositionClass::GetPositionAnimationBoss(float& x, float& y, float& z, int coord)
{

	switch (coord)
	{
	case 0:
		m_positionCarX += m_frameTime * 0.001;
		break;
	case 1:
		m_positionCarY += m_frameTime * 0.001;
		break;
	case 2:
		m_positionCarZ += m_frameTime * 0.001;
		break;
	case 3:
		m_positionCarX -= m_frameTime * 0.001;
		break;
	case 4:
		m_positionCarY -= m_frameTime * 0.001;
		break;
	case 5:
		m_positionCarZ -= m_frameTime * 0.001;
		break;
	default:
		break;
	}

	x = m_positionCarX;
	y = m_positionCarY;
	z = m_positionCarZ;
	return;
}

void PositionClass::ChangeAnimationBossControl(int op) {
	m_ctrlAnimBossBool[op] = !m_ctrlAnimBossBool[op];
	//return m_ctrlAnimCar[op];
}

bool PositionClass::GetAnimationBossControl(int op) {
	//m_ctrlAnimCar[op] = !m_ctrlAnimCar[op];
	return m_ctrlAnimBossBool[op];
}

void PositionClass::ResetStructurePosition() {
	m_positionX = 500.0f;
	m_positionY = 5.0f;
	m_positionZ = 0.0f;

	m_positionCarX = 0.0f;
	m_positionCarY = 0.0f;
	m_positionCarZ = 0.0f;
	m_ctrlAnimCar[0] = false;
	m_ctrlAnimCar[1] = false;

	m_ctrlAnimBossX = 0.0f;
	m_ctrlAnimBossY = 0.0f;
	m_ctrlAnimBossZ = 0.0f;
	m_ctrlAnimBossBool[0] = false;
	m_ctrlAnimBossBool[1] = false;
	m_cameraType = true;
}

