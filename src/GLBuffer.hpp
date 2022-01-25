#ifndef GLBUFFER_H
#define GLBUFFER_H
#pragma once

#include <glad/glad.h>

namespace EaseGL
{
	enum class GLBufferType
	{
		NONE = 0,
		ARRAY_BUFFER,
		ELEMENT_ARRAY_BUFFER,
		UNIFORM_BUFFER,
	};

	enum class GLBufferUsage
	{
		NONE = 0,
		STATIC_DRAW,
		DYNAMIC_DRAW,
		STREAM_DRAW,
	};

	class GLBuffer  
	{
		private:
			GLBufferType m_BufferType;

			GLenum GetGLBufferType() const;
			GLenum GetGLBufferUsage(GLBufferUsage usage) const;

			GLuint m_BufferID;

			friend class Shader;
		public:
			GLBuffer();

			GLBuffer(GLBufferType bufferType);
			~GLBuffer();

			/**
			 * @brief Call when buffer objects needs to be created/recreated
			 */
			void RecreateBuffer();

			void BufferData(const void* data, uint32_t size, GLBufferUsage usage) const;
			void BufferSubData(const void* data, GLintptr offset, GLsizeiptr size) const;

			void* MapBuffer();
			void UnmapBuffer();
			void BindBufferBase(GLuint bufferIndex);

			void Bind() const;
	};
} // namespace EaseGL
#endif