#include "GLTexture.hpp"

#ifdef EASEGL_IMPLEMENTATION


#ifdef STB_IMAGE_IMPLEMENTATION
   #undef STB_IMAGE_IMPLEMENTATION
   #include <stb/stb_image.h>
   #define STB_IMAGE_IMPLEMENTATION
#else
   #include <stb/stb_image.h>
#endif

#include <iostream>

namespace EaseGL
{
   GLTexture::~GLTexture()
   {
   }

   void GLTexture::LoadTexture(const char* filepath) 
   {
      if(m_Pixels != nullptr)
         stbi_image_free(m_Pixels);
      
      int w,h,n,ok;
      ok = stbi_info(filepath, &w, &h, &n);
      if(ok == 1)
         std::cout << "Texture loading " << filepath << " is ok [w:" << w << ", h:" << h << ", n:" << n << "]" << std::endl;

      m_Pixels = stbi_load(filepath, &m_Width, &m_Height, &m_Channels, 0);
      
      if(m_Pixels == NULL)
         std::cout << "ERROR on loading Texture " << filepath << std::endl;
      
      GenTextures();
   }

   GLTexture::GLTexture(TextureType type) 
      : m_TextureType(type), m_Pixels(nullptr), m_Width(0), m_Height(0), m_Channels(0), m_TextureID(0), m_Filepath("")
   {
   }

   GLTexture::GLTexture(TextureType type, const char* filepath)
      : m_TextureType(type), m_Filepath(filepath), m_Pixels(nullptr), m_Width(0), m_Height(0), m_Channels(0), m_TextureID(0)
   {
      LoadTexture(filepath);
   }

   GLTexture::GLTexture(GLuint textureID, TextureType textureType)
      : m_TextureID(textureID), m_TextureType(textureType)
   {
   }
   

   GLenum GLTexture::GetGLTextureType() const
   {
      return m_TextureType == TextureType::TEXTURE2D ? GL_TEXTURE_2D
         : GL_NONE;
   }

   void GLTexture::GenTextures()
   {
      DeleteTextures();

      glGenTextures(1, &m_TextureID);
      Bind();

      glTexParameteri(GetGLTextureType(), GL_TEXTURE_WRAP_S, GL_REPEAT);	
      glTexParameteri(GetGLTextureType(), GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GetGLTextureType(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GetGLTextureType(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      
      int oldUnpackAlignment = 0;
      glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldUnpackAlignment);
      bool changeAlignment = false; // to prevent unnecessary GPU calls
      // if m_Channels == 3 -> pixels should be aligned by 1, else(1, 2, 4) it can be aligned to m_Channels itself
      if(m_Channels == 3 && oldUnpackAlignment != 1)
      {
         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
         changeAlignment = true;
      }
      else if(oldUnpackAlignment != m_Channels)
      {
         glPixelStorei(GL_UNPACK_ALIGNMENT, m_Channels);
         changeAlignment = true;
      }

      glTexImage2D(GetGLTextureType(), 0,
         GL_RGBA8, // store in GPU as
         m_Width, m_Height,
         0, // border
         m_Channels == 4 ? GL_RGBA : m_Channels == 3 ? GL_RGB : m_Channels == 2 ? GL_RG : m_Channels == 1 ? GL_RED : GL_NONE, // which format does our image is, (can be found from 'm_Channels')
         GL_UNSIGNED_BYTE,
         m_Pixels);
      glGenerateMipmap(GetGLTextureType());

      if(changeAlignment)
      {
         glPixelStorei(GL_UNPACK_ALIGNMENT, oldUnpackAlignment);
      }

   }

   void GLTexture::DeleteTextures()
   {
      if(m_TextureID != 0)
         glDeleteTextures(1, &m_TextureID);

      //if(m_Pixels != nullptr)
         //stbi_image_free(m_Pixels);
   }

   void GLTexture::Bind(int slot /* = 0*/) const
   {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GetGLTextureType(), m_TextureID);
   }
} // namespace EaseGL

#endif