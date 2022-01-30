#ifndef GLTEXTURE_H
#define GLTEXTURE_H
#pragma once
	
#include <glad/glad.h>
#include <string>

namespace EaseGL
{
	enum class TextureType
	{
		NONE = 0,
		TEXTURE2D,

		// Cubemap, 1d, 3d textures
	};
	
	class GLTexture 
	{
		private:
			GLuint m_TextureID;
			TextureType m_TextureType;

			int m_Width, m_Height, m_Channels;
			unsigned char* m_Pixels;

			std::string m_Filepath;

			GLenum GetGLTextureType() const;

			void GenTextures();
			void DeleteTextures();
		public:
			GLTexture() : m_TextureType(TextureType::TEXTURE2D), m_Pixels(nullptr), m_Width(0), m_Height(0), m_Channels(0), m_TextureID(0), m_Filepath("") {};
			GLTexture(GLuint textureID, TextureType textureType);

			void LoadTexture(const char* filepath);

			int Width() { return m_Width; }
			int Height() { return m_Height; }

			GLTexture(TextureType type);
			GLTexture(TextureType type, const char* filepath);
			~GLTexture();

			operator GLuint() { return m_TextureID; }

			void Bind(int slot = 0) const;
	};
} // namespace EaseGL

#endif