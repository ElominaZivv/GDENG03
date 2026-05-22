#pragma once
#include <JAZZY/Graphics/GraphicsResource.h>

namespace jazzy
{
	class ShaderBinary final: public GraphicsResource
	{
	public:
		ShaderBinary(const ShaderCompileDesc& desc, const GraphicsResourceDesc& gDesc);
		ShaderBinaryData getData() const noexcept;
		ShaderType getType() const noexcept;
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
		ShaderType m_type{};
	};
}

