#include <JAZZY/Graphics/ShaderBinary.h>
#include <JAZZY/Graphics/GraphicsUtil.h>
#include <d3dcompiler.h>
jazzy::ShaderBinary::ShaderBinary(const ShaderCompileDesc& desc, const GraphicsResourceDesc& gDesc): 
	GraphicsResource(gDesc), m_type(desc.shaderType)
{
	if (!desc.shaderSourceName) DX3DLogThrowInvalidArg("No shader source name provided.");
	if (!desc.shaderSourceCode) DX3DLogThrowInvalidArg("No shader source code provided.");
	if (!desc.shaderSourceCodeSize) DX3DLogThrowInvalidArg("No shader source code size provided.");
	if (!desc.shaderEntryPoint) DX3DLogThrowInvalidArg("No shader entry point provided.");

	UINT compileFlags{};

#ifndef _DEBUG
		compileFlags != D3DCOMPILE_DEBUG;
#endif

	Microsoft::WRL::ComPtr<ID3DBlob>errorBlob{};
	DX3DGraphicsCheckShaderCompile
	(
		D3DCompile
		(
			desc.shaderSourceCode,
			desc.shaderSourceCodeSize,
			desc.shaderSourceName,
			nullptr,
			nullptr,
			desc.shaderEntryPoint,
			jazzy::GraphicsUtils::GetShaderModelTarget(desc.shaderType),
			compileFlags,
			0,
			&m_blob,
			&errorBlob
		),
		errorBlob.Get()
	);
}

jazzy::ShaderBinaryData jazzy::ShaderBinary::getData() const noexcept
{
	return
	{
		m_blob->GetBufferPointer(),
		m_blob->GetBufferSize()
	};
}

jazzy::ShaderType jazzy::ShaderBinary::getType() const noexcept
{
	return m_type;
}
