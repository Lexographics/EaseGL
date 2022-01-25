#ifndef BUFFER_H
#define BUFFER_H
#pragma once

#include "GLBuffer.hpp"
#include <glad/glad.h>
#include <iostream>

namespace EaseGL
{
	struct IndexBuffer
	{
		public:
			static GLBuffer New()
			{
				return GLBuffer(GLBufferType::ELEMENT_ARRAY_BUFFER);
			}
			static void Unbind()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

		private:
			IndexBuffer() {}
		public:
			~IndexBuffer() {}
	};

	struct VertexBuffer
	{
		public:
			static GLBuffer New()
			{
				return GLBuffer(GLBufferType::ARRAY_BUFFER);
			}
			static void Unbind()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

		private:
			VertexBuffer() {}
		public:
			~VertexBuffer() {}
	};

	struct UniformBuffer // look at Shader Storage Buffer Object
	{
		public:
			static GLBuffer New()
			{
				std::cout << "DONT USE UNIFORM BUFFERS YET!" << std::endl;
				return GLBuffer(GLBufferType::UNIFORM_BUFFER);
			}
			static void Unbind()
			{
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
			}

		private:
			UniformBuffer() {}
		public:
			~UniformBuffer() {}
	};
} // namespace EaseGL

#endif