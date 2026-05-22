#pragma once
#include<JAZZY/Core/Common.h>
#include<JAZZY/Core/Base.h>
#include<d3d11.h>
#include<wrl.h>

// I didnt quite understand Pardcode, he wanted to put the Rendersystem in this so that its functions are not exposed to the rest of the engine
// Which is why the header is in the source folder instead of the include folder

namespace jazzy
{
	class RenderSystem final: public Base
	{
	public:
		explicit RenderSystem(const RenderSystemDesc& desc);
		virtual ~RenderSystem() override;
	private:
		// Beautiful smart pointers keep tracks of the number of references to the pointer
		// If the number of references to the pointer == 0, it automatically destroys the things <3
		Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice{};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext{};
	};
}
