#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once
	
#include <glad/glad.h>
#include "GLTexture.hpp"

namespace EaseGL
{
	struct Texture2D
	{
		public:
			static GLTexture New(const char* filepath)
			{
				return GLTexture(TextureType::TEXTURE2D, filepath);
			}
			static GLTexture New()
			{
				return GLTexture(TextureType::TEXTURE2D);
			}

			static void Unbind()
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

		private:
			Texture2D();
		public:
			~Texture2D();
	};
} // namespace EaseGL
#endif