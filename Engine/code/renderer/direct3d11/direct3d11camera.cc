#include "stdneva.h"
#include "direct3d11camera.h"

using namespace DirectX;
using namespace Direct3D11;

/*----------------------------------------------
*/
Direct3D11Camera::Direct3D11Camera()
{
	this->position = { 0, 0, 0 };
	this->rotation = { 0, 0, 0 };
}

/*----------------------------------------------
*/
Direct3D11Camera::~Direct3D11Camera()
{

}

/*----------------------------------------------
*/
DirectX::XMMATRIX Direct3D11Camera::GetViewMatrix() const
{
	XMVECTOR up = { 0, 1, 0 };
	XMVECTOR lookAt = { 0, 0, 1 };

	float degreeInRad = 0.0174532925f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = XMVectorGetX(this->rotation) * degreeInRad;
	float yaw = XMVectorGetY(this->rotation) * degreeInRad;
	float roll = XMVectorGetZ(this->rotation)* degreeInRad;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(roll, pitch, yaw);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	XMVector3TransformCoord(lookAt, rotationMatrix);
	XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = this->position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	DirectX::XMMATRIX viewMatrix = XMMatrixLookAtLH(this->position, lookAt, up);

	return viewMatrix;
}