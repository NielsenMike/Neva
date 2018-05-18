#pragma once
/**
* Direct 3d 11 camera.
*
* Mike Nielsen
* 2017
*/
#include <DirectXMath.h>

namespace Direct3D11
{

class Direct3D11Camera
{
public:
	/// constructor
	Direct3D11Camera();
	/// destructor
	~Direct3D11Camera();

	/// set position
	void SetPosition(float x, float y, float z);
	/// get position
	DirectX::XMVECTOR GetPosition() const;
	/// set rotation
	void SetRotation(float x, float y, float z);
	/// get rotation
	const DirectX::XMVECTOR GetRotation() const;
	/// get view matrix
	DirectX::XMMATRIX GetViewMatrix() const;

private:
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
};

/*----------------------------------------------
*/
inline void Direct3D11Camera::SetPosition(float x, float y, float z)
{
	this->position = { x, y, z };
}

/*----------------------------------------------
*/
inline DirectX::XMVECTOR Direct3D11Camera::GetPosition() const
{
	return this->position;
}

/*----------------------------------------------
*/
inline void Direct3D11Camera::SetRotation(float x, float y, float z)
{
	this->rotation = { x, y, z, 1.0f };
}

/*----------------------------------------------
*/
inline const DirectX::XMVECTOR Direct3D11Camera::GetRotation() const
{
	return this->rotation;
}

} // namespace Direct3D11