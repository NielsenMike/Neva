#pragma once
/**
* Direct 3d 11 renderer.
*
* Mike Nielsen
* 2017
*/
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#include <D3D11.h>
#include <DXGI.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "direct3d11camera.h"
#include "direct3d11model.h"
#include "direct3d11shader.h"

namespace Direct3D11
{

class Direct3D11Renderer
{
public:
	/// constructor
	Direct3D11Renderer();
	/// destructor
	~Direct3D11Renderer();
	
	/// create swapchain, device and device context
	bool Setup(float frameWidth, float frameHeight, bool vsync, HWND hWnd, bool fullscreen, float nearPlane, float depthPlane);
	/// release
	void Destroy();
	/// release and resize
	bool Resize(int screenWidth, int screenHeight);
	/// Clear the buffers to begin the scene.
	void BeginScene(float red, float green, float blue, float alpha);
	/// Present the rendered scene to the screen.
	void EndScene();

	/// get device
	ID3D11Device* GetDevice();
	/// get device context
	ID3D11DeviceContext* GetDeviceContext();
	/// get projection matrix
	DirectX::XMMATRIX GetProjectionMatrix() const;
	/// get world matrix
	DirectX::XMMATRIX GetWorldMatrix() const;
	/// get orthographic matrix
	DirectX::XMMATRIX GetOrthoMatrix() const;

	/// get frame near
	float GetNearPlane() const;
	/// set frame near
	void SetNearPlane(float near);

	/// get frame depth
	float GetDepthPlane() const;
	/// set  frame depth
	void SetDepthPlane(float depth);

	/// get fullscreen mode
	bool GetFullScreen() const;
	/// set fullscreen mode
	void SetFullScreen(bool fullscreen);

	/// get frame width for renderer
	float GetFrameWidth() const;
	/// set frame width for renderer
	void SetFrameWidth(float height);

	/// get frame width for renderer
	float GetFrameHeight() const;
	/// set frame width for renderer
	void SetFrameHeight(float width);

	/// get vertical synchronization
	bool GetVsync() const;
	/// set vertical synchronization
	void SetVsync(bool vysnc);


private:
	/// setup adapter description
	bool SetupAdapter(int screenWidth, int screenHeight, unsigned int& outNumerator, unsigned int& outDenominator);
	/// get swap chain description
	DXGI_SWAP_CHAIN_DESC GetSwapChainDescription(HWND hWnd, bool fullscreen, unsigned int numerator, unsigned int denominator) const;
	/// get depth buffer description
	D3D11_TEXTURE2D_DESC GetDepthBufferDescription(unsigned int screenWidth, unsigned int screenHeight) const;
	/// get depth stencil description
	D3D11_DEPTH_STENCIL_DESC GetDepthStencilDescription() const;
	/// get depth stencil view description
	D3D11_DEPTH_STENCIL_VIEW_DESC GetDepthStencilViewDescription() const;
	/// get rasterizer description
	D3D11_RASTERIZER_DESC GetRasterizerDescription() const;

	unsigned int flags;
	int videoCardMemory;
	char* videoCardDescription;

	ID3D11Device *device;   
	IDXGISwapChain* swapchain;
	ID3D11DeviceContext* devcontext;
	ID3D11RenderTargetView* renderTarget;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RasterizerState* rasterState;

	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;

	// include camera!!!

	bool vsync;
	bool fullscreen;

	float frameWidth;
	float frameHeight;

	float nearPlane;
	float depthPlane;
};

/*----------------------------------------------
*/
inline ID3D11Device* Direct3D11Renderer::GetDevice()
{
	return this->device;
}

/*----------------------------------------------
*/
inline ID3D11DeviceContext* Direct3D11Renderer::GetDeviceContext()
{
	return this->devcontext;
}

/*----------------------------------------------
*/
inline DirectX::XMMATRIX Direct3D11Renderer::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

/*----------------------------------------------
*/
inline DirectX::XMMATRIX Direct3D11Renderer::GetWorldMatrix() const
{
	return this->worldMatrix;
}

/*----------------------------------------------
*/
inline DirectX::XMMATRIX Direct3D11Renderer::GetOrthoMatrix() const
{
	return this->orthoMatrix;
}

/*----------------------------------------------
*/
inline float Direct3D11Renderer::GetNearPlane() const
{
	return this->nearPlane;
}

/*----------------------------------------------
*/
inline void Direct3D11Renderer::SetNearPlane(float nearplane)
{
	this->nearPlane = nearplane;
}

/*----------------------------------------------
*/
inline float Direct3D11Renderer::GetDepthPlane() const
{
	return this->depthPlane;
}

/*----------------------------------------------
*/
inline void Direct3D11Renderer::SetDepthPlane(float depth)
{
	this->depthPlane = depth;
}

/*----------------------------------------------
*/
inline bool Direct3D11Renderer::GetFullScreen() const
{
	return this->fullscreen;
}

/*----------------------------------------------
*/
inline void Direct3D11Renderer::SetFullScreen(bool _fullscreen)
{
	this->fullscreen = _fullscreen;
}

/*----------------------------------------------
*/
inline float Direct3D11Renderer::GetFrameWidth() const
{
	return this->frameWidth;
}

/*----------------------------------------------
*/
inline void Direct3D11Renderer::SetFrameWidth(float width)
{
	this->frameWidth = width;
}

/*----------------------------------------------
*/
inline float Direct3D11Renderer::GetFrameHeight() const
{
	return this->frameHeight;
}

/*----------------------------------------------
*/
inline void Direct3D11Renderer::SetFrameHeight(float height)
{
	this->frameHeight = height;
}

/*----------------------------------------------
*/
inline bool Direct3D11Renderer::GetVsync() const
{
	return this->vsync;
}

/*----------------------------------------------
*/
inline void Direct3D11Renderer::SetVsync(bool vysnc)
{
	this->vsync = vysnc;
}

} // namespace Direct3D11
