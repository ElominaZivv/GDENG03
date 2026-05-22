#pragma once
#include <JAZZY/Core/Common.h>
namespace jazzy
{
	namespace GraphicsUtils
	{
		inline const char* GetShaderModelTarget(ShaderType type)
		{
			switch(type)
			{
			case ShaderType::VertexShader: return "vs_5_0";
			case ShaderType::PixelShader: return "ps_5_0";
			default: return "";
			}
		}
	}
}