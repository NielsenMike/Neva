#include "stdneva.h"
#include "direct3d11shader.h"

using namespace DirectX;
using namespace Direct3D11;

/*----------------------------------------------
*/
Direct3D11Shader::Direct3D11Shader() :
	vertexShader(0),
	pixelShader(0),
	layout(0),
	constantBuffer(0)
{
}

/*----------------------------------------------
*/
Direct3D11Shader::~Direct3D11Shader()
{
	this->Destroy();
}


/*----------------------------------------------
	L"C:/Projects/Neva/Engine/shader/basiccolor.hlsl"
*/
bool Direct3D11Shader::Create(ID3D11Device* device, HWND hWnd, wchar_t const* filePath)
{
	HRESULT result;
	ID3DBlob* vertexShaderBuffer = 0;
	ID3DBlob* pixelShaderBuffer = 0;
	ID3DBlob* errorMessage = 0;

	D3D11_BUFFER_DESC constantBufferDesc;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = D3DCompileFromFile(filePath, 0, 0, "VShader", "vs_4_0", D3DCOMPILE_DEBUG, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			this->OutputShaderErrorMessage(errorMessage, hWnd, filePath);
		}
		else
		{
			MessageBoxExW(hWnd, filePath, L"Missing Shader File", MB_OK, 0);
		}
		return false;
	}
		
	result = D3DCompileFromFile(filePath, 0, 0, "PShader", "ps_4_0", D3DCOMPILE_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			this->OutputShaderErrorMessage(errorMessage, hWnd, filePath);
		}
		else
		{
			MessageBoxExW(hWnd, filePath, L"Missing Shader File", MB_OK, 0);
		}
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &this->vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &this->layout);
	if (FAILED(result))
	{
		return false;
	}


	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	
	pixelShaderBuffer->Release();
	pixelShader = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = sizeof(WorldViewProjectionMatrix);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;


	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&constantBufferDesc, NULL, &this->constantBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

/*----------------------------------------------
*/
void Direct3D11Shader::Destroy()
{
	if (this->constantBuffer)
	{
		constantBuffer->Release();
		constantBuffer = 0;
	}
	if (this->layout)
	{
		this->layout->Release();
		this->layout = 0;
	}
	if (this->pixelShader)
	{
		this->pixelShader->Release();
		this->pixelShader = 0;
	}
	if (this->vertexShader)
	{
		this->vertexShader->Release();
		this->vertexShader = 0;
	}
	return;
}

/*----------------------------------------------
*/
bool Direct3D11Shader::Render(ID3D11DeviceContext* context, int indexCount, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{
	bool result = false;

	// Set the shader parameters that it will use for rendering.
	result = this->SetShaderParameters(context, world, view, projection);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	this->RenderShader(context, indexCount);

	return true;
}

/*----------------------------------------------
*/
void Direct3D11Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, wchar_t const* filePath)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBoxExW(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", filePath, MB_OK, 0);

	return;
}

/*----------------------------------------------
*/
bool Direct3D11Shader::SetShaderParameters(ID3D11DeviceContext* context, XMMATRIX world,
	XMMATRIX view, XMMATRIX projection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	WorldViewProjectionMatrix* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	XMMatrixTranspose(world);
	XMMatrixTranspose(view);
	XMMatrixTranspose(projection);

	// Lock the constant buffer so it can be written to.
	result = context->Map(this->constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (WorldViewProjectionMatrix*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;


	// Unlock the constant buffer.
	context->Unmap(this->constantBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	context->VSSetConstantBuffers(bufferNumber, 1, &this->constantBuffer);

	return true;
}

/*----------------------------------------------
*/
bool Direct3D11Shader::RenderShader(ID3D11DeviceContext* context, int indexCount)
{
	// Set the vertex input layout.
	context->IASetInputLayout(this->layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	context->VSSetShader(this->vertexShader, NULL, 0);
	context->PSSetShader(this->pixelShader, NULL, 0);

	// Render the triangle.
	context->DrawIndexed(indexCount, 0, 0);

	return true;
}


