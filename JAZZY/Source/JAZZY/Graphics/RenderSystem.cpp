#include <JAZZY/Graphics/RenderSystem.h>

jazzy::RenderSystem::RenderSystem(const RenderSystemDesc& desc): Base(desc.base)
{
	D3D_FEATURE_LEVEL featureLevel{};
	UINT createDeviceFlags{};

#ifndef _DEBUG	// Helps get error and warning messages during runtime
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	auto hr = D3D11CreateDevice
	(
		NULL,						// XGI Adaptor represents graphics card GPU installed in the system
		D3D_DRIVER_TYPE_HARDWARE,	// if XGI adaptor is null, and this is hardware, it will pick the primary GPU
		NULL,						// Handle to a dll a custom software rastorizer but we arent using in this case
		createDeviceFlags,
		NULL,						// A pointer to an array of feature level, if NULL, the system will pick the best one
		0,							// Size of the previous array, since NULL, 0
		D3D11_SDK_VERSION,			// Microsoft safety check to prevent mismatch of versions and to make sure the versions match the direct3d runtime  
		
		// OUTPUT STUFF BELOW
		&m_d3dDevice,				// Expects a double pointer, this is where all the resources like buffer are
		&featureLevel,
		&m_d3dContext				// Expects a double pointer, this is where all the resources like buffer are
	);

	if (FAILED(hr))
	{
		getLogger().log(Logger::LogLevel::Error, "Direct3D initialization failed.");
		throw std::runtime_error("Direct3D initialization failed.");
	}
}

jazzy::RenderSystem::~RenderSystem()
{
}
