#include "stdneva.h"
#include "direct3d11renderer.h"

using namespace DirectX;
using namespace Direct3D11;

/*----------------------------------------------
*/
Direct3D11Renderer::Direct3D11Renderer()
{
	this->swapchain = 0;
	this->device = 0;
	this->devcontext = 0;
	this->renderTarget = 0;
	this->depthStencilBuffer = 0;
	this->depthStencilState = 0;
	this->depthStencilView = 0;
	this->rasterState = 0;

	this->flags = 0;
	this->vsync = true;
	this->fullscreen = false;
	this->videoCardMemory = 0;
	this->videoCardDescription = (char*)calloc(128, sizeof(char));

	this->projectionMatrix = DirectX::XMMATRIX();
	this->worldMatrix = DirectX::XMMATRIX();
	this->orthoMatrix = DirectX::XMMATRIX();

	this->frameWidth = 0;
	this->frameHeight = 0;
	this->nearPlane = 0;
	this->depthPlane = 0;
}

/*----------------------------------------------
*/
bool Direct3D11Renderer::Setup(float frameWidth, float frameHeight, bool vsync, 
	HWND hWnd, bool fullscreen, float nearPlane, float depthPlane)
{
	HRESULT result = S_OK;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D* backBufferPtr = 0;
	D3D11_VIEWPORT viewport;

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	
	float fieldOfView, screenAspect;

	// Store the vsync setting.
	this->vsync = vsync;

	// setup adapter display mode
	unsigned int numerator, denominator = 0;
	if (!this->SetupAdapter(frameWidth, frameHeight, numerator, denominator))
	{
		return false;
	}

	// set swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc = this->GetSwapChainDescription(hWnd, fullscreen, numerator, denominator);
	D3D11_TEXTURE2D_DESC depthBufferDesc = this->GetDepthBufferDescription(frameWidth, frameHeight);
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = this->GetDepthStencilDescription();
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = this->GetDepthStencilViewDescription();
	D3D11_RASTERIZER_DESC rasterizerDesc = this->GetRasterizerDescription();

	// create a device, device context and swap chain using the information in the scd struct
	result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&this->swapchain,
		&this->device,
		NULL,
		&this->devcontext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the texture for the depth buffer using the filled out description.
	result = this->device->CreateTexture2D(&depthBufferDesc, NULL, &this->depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// use the back buffer address to create the render target
	// Create the render target view with the back buffer pointer.
	result = this->device->CreateRenderTargetView(backBufferPtr, NULL, &this->renderTarget);
	if (FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	// Create the depth stencil state.
	result = this->device->CreateDepthStencilState(&depthStencilDesc, &this->depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	this->devcontext->OMSetDepthStencilState(this->depthStencilState, 1);

	// Create the depth stencil view.
	result = this->device->CreateDepthStencilView(this->depthStencilBuffer, &depthStencilViewDesc, &this->depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	this->devcontext->OMSetRenderTargets(1, &this->renderTarget, this->depthStencilView);

	// Create the rasterizer state from the description we just filled out.
	result = this->device->CreateRasterizerState(&rasterizerDesc, &this->rasterState);
	if (FAILED(result))
	{
		return false;
	}


	// Now set the rasterizer state.
	this->devcontext->RSSetState(this->rasterState);

	// Setup the viewport for rendering.
	viewport.Width = frameWidth;
	viewport.Height = frameHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	this->devcontext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)g_XMPi[0] / 4.0f;
	screenAspect = frameWidth / frameHeight;

	// Create the projection matrix for 3D rendering.
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, nearPlane, depthPlane);

	this->worldMatrix = XMMatrixIdentity();

	this->orthoMatrix = XMMatrixOrthographicLH(frameWidth, frameHeight, nearPlane, depthPlane);

	return true;
}

/*----------------------------------------------
*/
void Direct3D11Renderer::Destroy()
{
	// Before destroy set to windowed mode or when you release the swap chain it will throw an exception.
	if (this->swapchain != nullptr)
	{
		this->swapchain->SetFullscreenState(false, NULL);
	}

	if (this->device != nullptr)
	{
		this->device->Release();
		this->device = 0;
	}

	if (this->swapchain != nullptr)
	{
		this->swapchain->Release();;
		this->swapchain = 0;
	}

	if (this->devcontext != nullptr)
	{
		this->devcontext->Release();;
		this->devcontext = 0;
	}

	if (this->renderTarget != nullptr)
	{
		this->renderTarget->Release();;
		this->renderTarget = 0;
	}

	if (this->depthStencilBuffer != nullptr)
	{
		this->depthStencilBuffer->Release();;
		this->depthStencilBuffer = 0;
	}

	if (this->depthStencilState != nullptr)
	{
		this->depthStencilState->Release();;
		this->depthStencilState = 0;
	}

	if (this->depthStencilView != nullptr)
	{
		this->depthStencilView->Release();;
		this->depthStencilView = 0;
	}

	if (this->rasterState != nullptr)
	{
		this->rasterState->Release();;
		this->rasterState = 0;
	}

	if (this->videoCardDescription != nullptr)
	{
		delete[] this->videoCardDescription;
		this->videoCardDescription = 0;
	}

	printf("Direct3D11::Direct3D11Renderer::Destroy - destroyed!");

	return;
}

/*----------------------------------------------
*/
bool Direct3D11::Direct3D11Renderer::Resize(int screenWidth, int screenHeight)
{
	this->devcontext->OMSetRenderTargets(0, 0, 0);

	//release all outstanding references to the swap chains buffers
	this->renderTarget->Release();

	HRESULT result;

	// Preserve the existing buffer count and format.
	// Automatically choose the width and height to match the client rect for HWNDs.
	result = this->swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(result))
	{
		return false;
	}
	// Get buffer and create a render-target-view.
	ID3D11Texture2D* pBuffer;
	result = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = this->device->CreateRenderTargetView(pBuffer, NULL, &this->renderTarget);
	if (FAILED(result))
	{
		return false;
	}
	pBuffer->Release();

	this->devcontext->OMSetRenderTargets(1, &this->renderTarget, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = screenWidth;
	vp.Height = screenHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->devcontext->RSSetViewports(1, &vp);

	return true;
}

/*----------------------------------------------
*/
void Direct3D11Renderer::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//// Clear the back buffer.
	this->devcontext->ClearRenderTargetView(this->renderTarget, color);
	//
	//// Clear the depth buffer.
	this->devcontext->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

/*----------------------------------------------
*/
void Direct3D11Renderer::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (this->vsync)
	{
		// Lock to screen refresh rate.
		this->swapchain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		this->swapchain->Present(0, 0);
	}

	return;
}

/*----------------------------------------------
*/
bool Direct3D11Renderer::SetupAdapter(int screenWidth, int screenHeight, unsigned int& outNumerator, unsigned int& outDenominator)
{
	HRESULT result;

	DXGI_ADAPTER_DESC adapterDesc;
	ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));

	unsigned int numModes = 0;
	unsigned int stringLength = 0;

	IDXGIFactory* factory = 0;
	IDXGIAdapter* adapter = 0;
	IDXGIOutput* adapterOutput = 0;

	DXGI_MODE_DESC* displayModeList = 0;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				outNumerator = displayModeList[i].RefreshRate.Numerator;
				outDenominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	delete displayModeList;

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	this->videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	int error = wcstombs_s(&stringLength, this->videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	return true;
}

/*----------------------------------------------
*/
DXGI_SWAP_CHAIN_DESC Direct3D11Renderer::GetSwapChainDescription(HWND hWnd, bool fullscreen, unsigned int numerator, unsigned int denominator) const
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Set the refresh rate of the back buffer.
	if (this->vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// fill the swap chain description struct
	swapChainDesc.BufferCount = 1;										// one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// use 32-bit color
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// how swap chain is to be used
	swapChainDesc.OutputWindow = hWnd;									// the window to be used
	swapChainDesc.SampleDesc.Count = 1;									// how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;								// multisample quality level
	swapChainDesc.Windowed = !fullscreen;								// set fullscreen mode      														
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// windowed/full-screen mode
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				// Discard the back buffer contents after presenting.

	return swapChainDesc;
}

/*----------------------------------------------
*/
D3D11_TEXTURE2D_DESC Direct3D11Renderer::GetDepthBufferDescription(unsigned int screenWidth, unsigned int screenHeight) const
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	return depthBufferDesc;
}

/*----------------------------------------------
*/
D3D11_DEPTH_STENCIL_DESC Direct3D11Renderer::GetDepthStencilDescription() const
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	return depthStencilDesc;
}

/*----------------------------------------------
*/
D3D11_DEPTH_STENCIL_VIEW_DESC Direct3D11Renderer::GetDepthStencilViewDescription() const
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	return depthStencilViewDesc;
}

D3D11_RASTERIZER_DESC Direct3D11Renderer::GetRasterizerDescription() const
{
	D3D11_RASTERIZER_DESC rasterDesc;

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	return rasterDesc;
}

/*----------------------------------------------
*/
Direct3D11Renderer::~Direct3D11Renderer()
{
	this->Destroy();
}


