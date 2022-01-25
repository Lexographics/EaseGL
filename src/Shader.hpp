#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <glad/glad.h>
#include "GLBuffer.hpp"
#include <unordered_map>
#include <glm/glm.hpp>
#include "GLTexture.hpp"

namespace EaseGL
{
	enum class ShaderType
	{
		NONE = 0,
		VERTEX,
		FRAGMENT,
		GEOMETRY,
	};
	class Shader  
	{
		private:
			GLuint m_VertexShaderID;
			GLuint m_FragmentShaderID;
			GLuint m_GeometryShaderID;

			GLuint m_ProgramID;

			std::unordered_map<std::string, GLint> m_UniformLocations;

			int m_MaxTextureSlots;
		public:

			Shader() = default;
			Shader(const char* shaderPath);
			~Shader();

			void LoadShader(const char* shaderPath);


			void Bind();

			GLuint GetUniformLocation(const std::string& name);



			void Uniform(const std::string& name, const glm::mat4& uniform);
			void Uniform(const std::string& name, const GLTexture& uniform, int slot);
			void Uniform(const std::string& name, int uniform);


			/**
			 * @brief 
			 * 
			 * @param uniformBuffer 
			 * @param bufferIndex between 0 and GL_MAX_UNIFORM_BUFFER_BINDINGS, similar to glActiveTexture
			 * @param uniformName
			 */
			void BindUBO(GLBuffer uniformBuffer, int bufferIndex, const char* uniformName);
	};
} // namespace EaseGL

#endif