#pragma once
#include <string>

namespace Engine{
	struct ShaderInfo
	{
		unsigned int type;
		const char* filename;
		unsigned int shaderId;
	};

	struct Shader
	{
		static unsigned int loadShaders(ShaderInfo* shaderInfo);


	private:
		static std::string readShader(const char* filename);
	};

}