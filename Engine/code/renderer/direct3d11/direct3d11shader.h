#pragma once
/**
* Direct 3d 11 shader.
*
* Mike Nielsen
* 2017
*/

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <fstream>

namespace Direct3D11
{
	struct WorldViewProjectionMatrix
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};


	class Direct3D11Shader
	{
	public:
		/// constructor
		Direct3D11Shader();
		/// destructor
		~Direct3D11Shader();

		/// create shader, and compile shader file
		bool Create(ID3D11Device* device, HWND hWnd, wchar_t const* filePath);
		/// realse shader and constant buffer
		void Destroy();
		/// Set shader parameters and render shader
		bool Render(ID3D11DeviceContext* context, int, DirectX::XMMATRIX world,
			DirectX::XMMATRIX view, DirectX::XMMATRIX projection);

	private:
		/// output shader error messages
		void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, wchar_t const* filePath);
		/// set shader parameters
		bool SetShaderParameters(ID3D11DeviceContext* context, DirectX::XMMATRIX world,
			DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
		/// render shader
		bool RenderShader(ID3D11DeviceContext* context, int indexCount);

		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout* layout;
		ID3D11Buffer* constantBuffer;
	};

} // namespace direct3D11