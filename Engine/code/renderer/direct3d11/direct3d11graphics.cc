#include "stdneva.h"
#include "direct3d11graphics.h"

using namespace Direct3D11;
using namespace DirectX;

/*----------------------------------------------
*/
Direct3D11Graphics::Direct3D11Graphics()
{
	this->renderer = 0;
	this->camera = 0;
	this->model = 0;
	this->basicShader = 0;
}

/*----------------------------------------------
*/
Direct3D11Graphics::~Direct3D11Graphics()
{
	this->Destroy();
}

/*----------------------------------------------
*/
bool Direct3D11Graphics::Create(int screenWidth, int screenHeight, HWND hwnd)
{




	// Create renderer.
	this->renderer = new Direct3D11Renderer();
	if (!this->renderer)
	{
		return false;
	}

	// Initialize renderer.
	if (!this->renderer->Create(screenWidth, screenHeight, true, hwnd, false, 1000.0f, 0.1f))
	{
		MessageBoxA(hwnd, "Failed to initialize Direct3D11Renderer", "Error", MB_OK);
		return false;
	}

	// create camera.
	this->camera = new Direct3D11Camera();
	if (!this->camera)
	{
		return false;
	}
	this->camera->SetPosition(0.0f, 0.0f, -50.0f);

	// create model
	this->model = new Direct3D11Model();
	if (!this->model)
	{
		return false;
	}
	if (!this->model->Create(this->renderer->GetDevice())) 
	{
		MessageBoxA(hwnd, "Failed to initialize Direct3D11Model", "Error", MB_OK);
		return false;
	}

	// create shader.
	this->basicShader = new Direct3D11Shader();
	if (!this->basicShader)
	{
		return false;
	}

	if (!this->basicShader->Create(this->renderer->GetDevice(), hwnd, L"C:/Projects/Neva/Engine/shader/basiccolor.hlsl"))
	{
		MessageBoxA(hwnd, "Failed to initialize Direct3D11Shader", "Error", MB_OK);
		return false;
	}


	return true;
}

/*----------------------------------------------
*/
void Direct3D11Graphics::Destroy()
{
	if (this->renderer != nullptr)
	{
		delete this->renderer;
		this->renderer = 0;
	}

	if (this->camera != nullptr)
	{
		delete this->camera;
		this->camera = 0;
	}

	if (this->model != nullptr)
	{
		delete this->model;
		this->model = 0;
	}

	if (this->basicShader != nullptr)
	{
		delete this->basicShader;
		this->basicShader = 0;
	}

	printf("Direct3D11::Direct3D11Graphics::Destroy - destroyed!");
}

/*----------------------------------------------
*/
bool Direct3D11Graphics::RenderFrame()
{

	// Clear the buffers to begin the scene.
	this->renderer->BeginScene(0.0f, 0.0f, 0.8f, 1.0f);

	// Generate the view matrix based on the camera's position.
	this->camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	XMMATRIX viewMatrix = this->camera->GetViewMatrix();
	XMMATRIX worldMatrix = this->renderer->GetWorldMatrix();
	XMMATRIX projectionMatrix = this->renderer->GetProjectionMatrix();


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->model->Render(this->renderer->GetDeviceContext());

	// Render the model using the basic shader.
	if (!this->basicShader->Render(this->renderer->GetDeviceContext(), this->model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}

	return true;
}

/*----------------------------------------------
*/
bool Direct3D11Graphics::ResizeFrame(HWND hwnd)
{
	RECT rect;
	if (GetWindowRect(hwnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		return this->renderer->Resize(width, height);
	}

	return false;
}