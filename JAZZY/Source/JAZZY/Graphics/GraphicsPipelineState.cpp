#include <JAZZY/Graphics/GraphicsPipelineState.h>
#include <JAZZY/Graphics/ShaderBinary.h>
#include <JAZZY/Graphics/GraphicsPipelineLayout.h>

jazzy::GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& desc, const GraphicsResourceDesc& gDesc): GraphicsResource(gDesc)
{
	auto vs = desc.layout.getVSBinaryData();
	auto ps = desc.layout.getPSBinaryData();
	auto vsInputElements = desc.layout.getInputElementsData();

	DX3DGraphicsLogThrowOnFail
	(
		m_device.CreateInputLayout
		(
			static_cast<const D3D11_INPUT_ELEMENT_DESC*>(vsInputElements.data),
			static_cast<ui32>(vsInputElements.dataSize),
			vs.data,
			vs.dataSize,
			&m_layout
		),
		"CreateInputLayout failed."
	);

	DX3DGraphicsLogThrowOnFail
	(
		m_device.CreateVertexShader
		(
			vs.data,
			vs.dataSize,
			nullptr,
			&m_vs
		),
		"CreateVertexShader failed."
	);
	DX3DGraphicsLogThrowOnFail
	(
		m_device.CreatePixelShader
		(
			ps.data,
			ps.dataSize,
			nullptr,
			&m_ps
		),
		"CreatePixelShader failed."
	);
 }