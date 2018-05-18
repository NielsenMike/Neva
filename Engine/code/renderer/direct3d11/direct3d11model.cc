#include "stdneva.h"
#include "direct3d11model.h"

using namespace DirectX;
using namespace Direct3D11;

/*----------------------------------------------
*/
Direct3D11Model::Direct3D11Model() :
	vertexBuffer(0),
	indexBuffer(0)
{
}

Direct3D11Model::Direct3D11Model(const Direct3D11Model& other)
{
	this->vertexBuffer = other.vertexBuffer;
	this->indexBuffer = other.indexBuffer;
}

/*----------------------------------------------
*/
Direct3D11Model::~Direct3D11Model()
{
	this->Destroy();
}


/*----------------------------------------------
*/
bool Direct3D11Model::Create(ID3D11Device* device)
{
	HRESULT result;
	unsigned long* indices;
	VertexType* vertices;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	
	ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_VIEWPORT));
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_VIEWPORT));

	// Vertex data
	this->vertexCount = 3;
	vertices = new VertexType[this->vertexCount];
	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * this->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	this->indexCount = 3;
	indices = new unsigned long[this->indexCount];
	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.
	// description of static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

/*----------------------------------------------
*/
void Direct3D11Model::Destroy()
{
	if (this->vertexBuffer != nullptr)
	{
		this->vertexBuffer->Release();
		this->vertexCount = 0;
	}
	if (this->indexBuffer != nullptr)
	{
		this->indexBuffer->Release();
		this->indexCount = 0;
	}
}

/*----------------------------------------------
*/
void Direct3D11Model::Render(ID3D11DeviceContext* context)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;


	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*----------------------------------------------
*/
int Direct3D11Model::GetIndexCount() const
{
	return this->indexCount;
}

/*----------------------------------------------
*/
int Direct3D11Model::GetVertexCount() const
{
	return this->indexCount;
}



