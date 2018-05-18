#pragma once
/**
* Direct 3d 11 graphics.
*
* Mike Nielsen
* 2017
*/
#include <DirectXMath.h>
#include "direct3d11renderer.h"

// TODO: create global header file
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

namespace Direct3D11
{
class Direct3D11Graphics
{
public:
	/// constructor
	Direct3D11Graphics();
	/// destructor
	~Direct3D11Graphics();

	bool Create(int width, int height, HWND handle);
	/// release 
	void Destroy();
	/// render per frame
	bool RenderFrame();
	/// resize frame
	bool ResizeFrame(HWND hwnd);

	/// get frame height
	float GetFrameHight() const;

private:
	Direct3D11Renderer* renderer;
	Direct3D11Camera* camera;
	Direct3D11Model* model;
	Direct3D11Shader* basicShader;

	float frameWidth;
	float frameHeight;

};
} // namespace Direct3D11

