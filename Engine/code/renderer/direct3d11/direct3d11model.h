/**
* Direct 3d 11 model.
*
* Mike Nielsen
* 2017
*/

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace Direct3D11
{

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	class Direct3D11Model
	{
	public:
		/// constructor
		Direct3D11Model();
		/// copy constructor
		Direct3D11Model(const Direct3D11Model& other);
		/// destructor
		~Direct3D11Model();

		/// create vertex/index buffer 
		bool Create(ID3D11Device* device);
		/// release buffers
		void Destroy();
		/// set index/vertex buffer
		void Render(ID3D11DeviceContext* context);
		/// get index count
		int GetIndexCount() const;
		/// get vertex count
		int GetVertexCount() const;

	private:
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;

		int vertexCount;
		int indexCount;
	};

} // namespace direct3D11
