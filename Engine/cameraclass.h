////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include "d3dclass.h"
#include <d3dx10math.h>
#include "textclass.h"
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass(TextClass* &text, D3DClass* &d3d);
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	void Follow(D3DXVECTOR3 followTarget, D3DXVECTOR3 targetForwardVector, float deltaTime);

	TextClass* m_Text;
	D3DClass* m_D3D;

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXVECTOR3 carPos;
	D3DXVECTOR3	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 up = D3DXVECTOR3 (0.0f, 1.0f, 0.0f);
	bool firstFrame = true;
	float distance = 23.0f; //Distance to keep from car
	float height = -5.0f; //Height from ground
	float springConstant = 400.0f;
	float dampConstant = 2.0f * sqrt(springConstant);
};

#endif