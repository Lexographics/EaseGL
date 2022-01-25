/*-- File: src/_single_header_start.cpp start --*/
/**
 * EaseGL
 * @author Lexographics
 * @brief A Small OpenGL Rendering library
 * @version 0.1
 * @date 2022-01-16
 * 
 * @copyright Copyright (c) 2022 Lexographics
 */

/**
 * define EASEGL_IMPLEMENTATION in one .cpp file
 * 
 * 
 * in some_file.cpp
 * #define EASEGL_IMPLEMENTATION
 * #include <EaseGL.hpp>
 * 
 * in any other file(cpp, hpp, ...)
 * #include <EaseGL.hpp>
 * 
 */

/**
 * EaseGL::VertexArray
 * 
 * EaseGL::GLBuffer buffer = VertexBuffer::New();
 * EaseGL::GLBuffer buffer = IndexBuffer::New();
 * 
 * EaseGL::GLTexture texture = Texture2D::New();
 * 
 * EaseGL::Shader
 */



/*-- File: src/_single_header_start.cpp end --*/
/*-- File: src/Buffer.cpp start --*/
/*-- #include "src/Buffer.hpp" start --*/
#ifndef BUFFER_H
#define BUFFER_H

/*-- #include "src/GLBuffer.hpp" start --*/
#ifndef GLBUFFER_H
#define GLBUFFER_H

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
/*-- #include "src/GLBuffer.hpp" end --*/
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
/*-- #include "src/Buffer.hpp" end --*/
#ifdef EASEGL_IMPLEMENTATION

namespace EaseGL
{
   
} // namespace EaseGL


#endif
/*-- File: src/Buffer.cpp end --*/
/*-- File: src/Framebuffer.cpp start --*/
/*-- #include "src/Framebuffer.hpp" start --*/
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
/*-- #include "src/Texture.hpp" start --*/
#ifndef TEXTURE_H
#define TEXTURE_H
	
#include <glad/glad.h>
/*-- #include "src/GLTexture.hpp" start --*/
#ifndef GLTEXTURE_H
#define GLTEXTURE_H
	
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
			GLTexture() : m_TextureType(TextureType::TEXTURE2D) {};
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
/*-- #include "src/GLTexture.hpp" end --*/

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
/*-- #include "src/Texture.hpp" end --*/
#include <vector>

namespace EaseGL
{
	enum class FramebufferAttachmentFormat
	{
		RGB,
		RGBA,
		RGBA8,
		RED_INTEGER,

		DEPTH24STENCIL8,
	};

	struct FrameBufferCreateInfo
	{
		int width = 1280;
		int height = 720;
		int samples = 1;

		std::vector<FramebufferAttachmentFormat> attachments;
		FrameBufferCreateInfo()
			: attachments({ FramebufferAttachmentFormat::RGBA8,
								 FramebufferAttachmentFormat::DEPTH24STENCIL8 })
		{
		}
	};

	class Framebuffer  
	{
		private:
			GLuint m_BufferID;
			
			struct Attachment
			{
				GLuint id;
				FramebufferAttachmentFormat format;

				Attachment()
					: id(0), format(FramebufferAttachmentFormat::DEPTH24STENCIL8)
				{
				}

				Attachment(GLuint _id, FramebufferAttachmentFormat _format)
					: id(id), format(_format)
				{
				}
			};
			std::vector<Attachment> m_ColorAttachments; // Texture2D
			Attachment m_DepthAttachment;// RenderBufferObject
		public:
			Framebuffer();
			~Framebuffer();

			// Create/Recreate framebuffer
			void RecreateBuffer(const FrameBufferCreateInfo& createInfo);

			void Bind();
			void Unbind();

			operator GLuint() { return m_BufferID; }

			GLuint GetColorAttachment(int at = 0) const { return m_ColorAttachments[at].id; }
	};
} // namespace EaseGL
#endif
/*-- #include "src/Framebuffer.hpp" end --*/

#ifdef EASEGL_IMPLEMENTATION
#include <glad/glad.h>
#include <iostream>

namespace EaseGL
{
   Framebuffer::Framebuffer()
      : m_BufferID(0)
   {
      
   }
      
   Framebuffer::~Framebuffer()
   {
      
   }
   
   bool IsColorAttachment(FramebufferAttachmentFormat format)
   {
      return format == FramebufferAttachmentFormat::RGBA8 
         || format == FramebufferAttachmentFormat::RGB
         || format == FramebufferAttachmentFormat::RGBA
         || format == FramebufferAttachmentFormat::RED_INTEGER;
   }

   bool IsDepthAttachment(FramebufferAttachmentFormat format)
   {
      return format == FramebufferAttachmentFormat::DEPTH24STENCIL8;
   }

   GLenum GetAttachmentFormat(FramebufferAttachmentFormat format)
   {
      return format == FramebufferAttachmentFormat::RGBA8 ? GL_RGBA8
           : format == FramebufferAttachmentFormat::RGB ? GL_RGB
           : format == FramebufferAttachmentFormat::RGBA ? GL_RGBA
           : format == FramebufferAttachmentFormat::DEPTH24STENCIL8 ? GL_RGBA
           : format == FramebufferAttachmentFormat::RED_INTEGER ? GL_RED_INTEGER
           : GL_NONE;
   }
   GLenum GetAttachmentInternalFormat(FramebufferAttachmentFormat format)
   {
      return format == FramebufferAttachmentFormat::RGBA8 ? GL_RGBA8
           : format == FramebufferAttachmentFormat::RGB ? GL_RGB
           : format == FramebufferAttachmentFormat::RGBA ? GL_RGBA
           : format == FramebufferAttachmentFormat::DEPTH24STENCIL8 ? GL_RGBA
           : format == FramebufferAttachmentFormat::RED_INTEGER ? GL_R32I
           : GL_NONE;
   }

   void Framebuffer::RecreateBuffer(const FrameBufferCreateInfo& createInfo)
   {
      if(m_BufferID != 0)
      {
         glDeleteFramebuffers(1, &m_BufferID);
         m_BufferID = 0;
      }

      glGenFramebuffers(1, &m_BufferID);
      glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);

      bool hasDepthAttachment = false;
      int colorAttachmentCount = 0;
      for(const FramebufferAttachmentFormat& attachment : createInfo.attachments)
      {
         if(IsDepthAttachment(attachment))
            hasDepthAttachment = true;
         else if(IsColorAttachment(attachment))
            m_ColorAttachments.emplace_back(0, attachment);
      }
      
      for(int i=0; i<m_ColorAttachments.size(); i++)
         glGenTextures(1, &m_ColorAttachments[i].id);

      if(createInfo.samples > 1)
         std::cout << "SAMPLES DOESNT WORK" << std::endl;

      for(int i = 0; i < m_ColorAttachments.size(); i++)
      {
         glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i].id);
         glTexImage2D(GL_TEXTURE_2D, 0, // level
               GetAttachmentInternalFormat(m_ColorAttachments[i].format), // internal format
               createInfo.width, createInfo.height,
               0, // border
               GetAttachmentFormat(m_ColorAttachments[i].format), // format
               GL_UNSIGNED_BYTE, // type
               NULL); // pixels
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         if(IsColorAttachment(createInfo.attachments[i]))
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_ColorAttachments[i].id, 0);
      }

      if(hasDepthAttachment)
      {
         glGenRenderbuffers(1, &m_DepthAttachment.id);
         glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment.id);
         glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, createInfo.width, createInfo.height);
         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment.id);
      }

      GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if(framebufferStatus != GL_FRAMEBUFFER_COMPLETE)
      {
         std::cout << "Framebuffer Error: " << framebufferStatus << std::endl;
      }
   }


   void Framebuffer::Bind()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
   }

   void Framebuffer::Unbind()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

} // namespace EaseGL
#endif
/*-- File: src/Framebuffer.cpp end --*/
/*-- File: src/GLBuffer.cpp start --*/
/*-- #include "src/GLBuffer.hpp" start --*/
/*-- #include "src/GLBuffer.hpp" end --*/

#ifdef EASEGL_IMPLEMENTATION
#include <glad/glad.h>

#include <cassert>

namespace EaseGL
{
   GLBuffer::GLBuffer(GLBufferType bufferType)
      : m_BufferType(bufferType), m_BufferID(0)
   {
      RecreateBuffer();
   }

   GLBuffer::GLBuffer() 
      : m_BufferType(GLBufferType::NONE), m_BufferID(0)
   {
   }
      
   GLBuffer::~GLBuffer()
   {
      if(m_BufferID != 0)
         glDeleteBuffers(1, &m_BufferID);
   }

   void GLBuffer::RecreateBuffer() 
   {
      if(m_BufferID != 0) {
         glDeleteBuffers(1, &m_BufferID);
      }

      glGenBuffers(1, &m_BufferID);
      glBindBuffer(GetGLBufferType(), m_BufferID);
   }

   void GLBuffer::BufferData(const void* data, uint32_t size, GLBufferUsage usage) const
   {
      glBindBuffer(GetGLBufferType(), m_BufferID);
      glBufferData(GetGLBufferType(), size, data, GetGLBufferUsage(usage));
   }

   void GLBuffer::BufferSubData(const void* data, GLintptr offset, GLsizeiptr size) const
   {
      glBindBuffer(GetGLBufferType(), m_BufferID);
      glBufferSubData(GetGLBufferType(), offset, size, data);
   }

   void* GLBuffer::MapBuffer() 
   {
      void* pointer = glMapBuffer(GetGLBufferType(), GL_READ_WRITE); 
      return pointer;
   }

   void GLBuffer::UnmapBuffer() 
   {
      glUnmapBuffer(GetGLBufferType());
   }


   void GLBuffer::Bind()  const
   {
      glBindBuffer(GetGLBufferType(), m_BufferID);
   }

   GLenum GLBuffer::GetGLBufferType() const
   {
      return m_BufferType == GLBufferType::ARRAY_BUFFER ? GL_ARRAY_BUFFER
         : m_BufferType == GLBufferType::ELEMENT_ARRAY_BUFFER ? GL_ELEMENT_ARRAY_BUFFER
         : m_BufferType == GLBufferType::UNIFORM_BUFFER ? GL_UNIFORM_BUFFER
         : GL_NONE; 
   }

   GLenum GLBuffer::GetGLBufferUsage(GLBufferUsage usage) const 
   {
      return usage == GLBufferUsage::STATIC_DRAW ? GL_STATIC_DRAW
         : usage == GLBufferUsage::DYNAMIC_DRAW ? GL_DYNAMIC_DRAW
         : usage == GLBufferUsage::STREAM_DRAW ? GL_STREAM_DRAW
         : GL_NONE; 
   }

} // namespace EaseGL
#endif
/*-- File: src/GLBuffer.cpp end --*/
/*-- File: src/GLTexture.cpp start --*/
/*-- #include "src/GLTexture.hpp" start --*/
/*-- #include "src/GLTexture.hpp" end --*/

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
      GenTextures();
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
/*-- File: src/GLTexture.cpp end --*/
/*-- File: src/Shader.cpp start --*/
/*-- #include "src/Shader.hpp" start --*/
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
/*-- #include "src/GLBuffer.hpp" start --*/
/*-- #include "src/GLBuffer.hpp" end --*/
#include <unordered_map>
#include <glm/glm.hpp>
/*-- #include "src/GLTexture.hpp" start --*/
/*-- #include "src/GLTexture.hpp" end --*/

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
/*-- #include "src/Shader.hpp" end --*/
#ifdef EASEGL_IMPLEMENTATION

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

/*-- #include "src/GLBuffer.hpp" start --*/
/*-- #include "src/GLBuffer.hpp" end --*/

namespace EaseGL
{
   void GetShaderError(GLuint shader, const char* filename, const char* type)
   {
      int  success;
      char infoLog[512];
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if(!success)
      {
         glGetShaderInfoLog(shader, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << '\n' << filename << std::endl;
         throw;
      }
   }
   void GetProgramError(GLuint shader, const char* filename)
   {
      int  success;
      char infoLog[512];
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if(!success)
      {
         glGetProgramInfoLog(shader, 512, NULL, infoLog);
         std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << '\n' << filename << std::endl;
         throw;
      }
   }

   Shader::Shader(const char* shaderPath)
      : m_ProgramID(0)
   {
      LoadShader(shaderPath);
   }

   void Shader::LoadShader(const char* shaderPath)
   {
      glGetIntegerv(GL_MAX_TEXTURE_UNITS, &m_MaxTextureSlots);

      ShaderType currentType = ShaderType::NONE;
      std::ifstream ifstream;
      ifstream.open(shaderPath);
      if(!ifstream)
      {
         std::cout << shaderPath;
         throw std::runtime_error("Failed to load file");
      }

      std::string vertexShaderStr = "";
      std::string fragmentShaderStr = "";
      std::string geometryShaderStr = "";

      std::string line;
      while (std::getline(ifstream, line))
      {
         if(strncmp(line.c_str(), "#shader vertex", strlen("#shader vertex")) == 0)
         {
            currentType = ShaderType::VERTEX;
         }
         else if(strncmp(line.c_str(), "#shader fragment", strlen("#shader fragment")) == 0)
         {
            currentType = ShaderType::FRAGMENT;
         }
         else if(strncmp(line.c_str(), "#shader geometry", strlen("#shader geometry")) == 0)
         {
            currentType = ShaderType::GEOMETRY;
         }
         else
         {
            // 'line' is shader code
            if(currentType == ShaderType::VERTEX)
            {
               vertexShaderStr += line + '\n';
            }
            else if(currentType == ShaderType::FRAGMENT)
            {
               fragmentShaderStr += line + '\n';
            }
            else if(currentType == ShaderType::GEOMETRY)
            {
               geometryShaderStr += line + '\n';
            }
         }
         
      }
      ifstream.close();


      if(vertexShaderStr != "")
      {
         m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
         const char* vertexShaderSrc = vertexShaderStr.c_str();
         glShaderSource(m_VertexShaderID, 1, &vertexShaderSrc, NULL);
         glCompileShader(m_VertexShaderID);
         GetShaderError(m_VertexShaderID, shaderPath, "VERTEX");
      }

      if(fragmentShaderStr != "")
      {
         m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
         const char* fragmentShaderSrc = fragmentShaderStr.c_str();
         glShaderSource(m_FragmentShaderID, 1, &fragmentShaderSrc, NULL);
         glCompileShader(m_FragmentShaderID);
         GetShaderError(m_FragmentShaderID, shaderPath, "FRAGMENT");
      }

      if(geometryShaderStr != "")
      {
         m_GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
         const char* geometryShaderSrc = geometryShaderStr.c_str();
         glShaderSource(m_GeometryShaderID, 1, &geometryShaderSrc, NULL);
         glCompileShader(m_GeometryShaderID);
         GetShaderError(m_GeometryShaderID, shaderPath, "GEOMETRY");
      }


      
      m_ProgramID = glCreateProgram();

      if(vertexShaderStr != "")
         glAttachShader(m_ProgramID, m_VertexShaderID);
      if(fragmentShaderStr != "")
         glAttachShader(m_ProgramID, m_FragmentShaderID);
      if(geometryShaderStr != "")
         glAttachShader(m_ProgramID, m_GeometryShaderID);
      glLinkProgram(m_ProgramID);

      GetProgramError(m_ProgramID, shaderPath);


      if(m_VertexShaderID != 0)
         glDeleteShader(m_VertexShaderID);
      if(m_FragmentShaderID != 0)
         glDeleteShader(m_FragmentShaderID);
      if(m_GeometryShaderID != 0)
         glDeleteShader(m_GeometryShaderID);
   }
      
   Shader::~Shader()
   {
      if(m_ProgramID != 0)
         glDeleteProgram(m_ProgramID);
   }

   void Shader::Bind() 
   {
      glUseProgram(m_ProgramID);
   }

   GLuint Shader::GetUniformLocation(const std::string& name) 
   {
      for(const auto& uniform : m_UniformLocations)
      {
         if(uniform.first == name)
            return uniform.second;
      }

      GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
      if(loc == -1)
      {
         std::cout << "ERROR: Uniform " << name << " not found!\n";
      }
      m_UniformLocations[name] = loc;
      return loc;
   }

   void Shader::BindUBO(GLBuffer uniformBuffer, int bufferIndex, const char* uniformName) 
   {
      std::cout << "DONT USE UNIFORM BUFFERS YET!" << std::endl;
      
      glBindBufferBase(GL_UNIFORM_BUFFER, bufferIndex, uniformBuffer.m_BufferID);

      int uniformIndex = glGetUniformBlockIndex(m_ProgramID, uniformName);
      glUniformBlockBinding(m_ProgramID, uniformIndex, bufferIndex);
   }



   void Shader::Uniform(const std::string& name, const glm::mat4& uniform)
   {
      glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &uniform[0][0]);
   }

   void Shader::Uniform(const std::string& name, const GLTexture& uniform, int slot) 
   {
      if(slot >= m_MaxTextureSlots)
      {
         std::cout << "TEXTURE SLOT " << slot << " NOT EXISTS! MAX: " << m_MaxTextureSlots - 1 << std::endl;
      }
      uniform.Bind(slot);
      Uniform(name, slot);
   }

   void Shader::Uniform(const std::string& name, int uniform) 
   {
      glUniform1i(GetUniformLocation(name), uniform);
   }
} // namespace EaseGL

#endif
/*-- File: src/Shader.cpp end --*/
/*-- File: src/Texture.cpp start --*/
/*-- #include "src/Texture.hpp" start --*/
/*-- #include "src/Texture.hpp" end --*/
#ifdef EASEGL_IMPLEMENTATION

namespace EaseGL
{
   
} // namespace EaseGL

#endif
/*-- File: src/Texture.cpp end --*/
/*-- File: src/VertexArray.cpp start --*/
/*-- #include "src/VertexArray.hpp" start --*/
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <glad/glad.h>
#include <vector>

namespace EaseGL
{
	enum class AttributeType
	{
		NONE = 0,
		FLOAT,
		VEC2,
		VEC3,
	};

	class VertexArray  
	{
		private:

			GLuint m_VertexArrayID;
		public:
			VertexArray();
			// creating a vertex array automatically binds it
			static VertexArray New();


			~VertexArray();

			void Bind() const;
			static void Unbind();


			void ResetAttributes();
			void SetAttribute(uint32_t slot, AttributeType type);
			void UploadAttributes();
		private:
			// Atrributes
			struct Attribute
			{
				AttributeType type;
				uint32_t slot;

				bool operator<(const Attribute& other)
				{
					return slot < other.slot;
				}

				Attribute(uint32_t _slot, AttributeType _type)
					: slot(_slot), type(_type)
				{
				}
			};
			uint32_t m_AttributeSizeInBytes = 0;

			std::vector<Attribute> m_UsedAttributes;
	};
} // namespace EaseGL

#endif
/*-- #include "src/VertexArray.hpp" end --*/

#ifdef EASEGL_IMPLEMENTATION

#include <glad/glad.h>
#include <algorithm>
#include <iostream>

namespace EaseGL
{
   VertexArray::VertexArray()
      : m_VertexArrayID(0)
   {
      glGenVertexArrays(1, &m_VertexArrayID);
      glBindVertexArray(m_VertexArrayID);
   }

   // static
   VertexArray VertexArray::New() 
   {
      return VertexArray();
   }
      
   VertexArray::~VertexArray()
   {
      if(m_VertexArrayID != 0)
         glDeleteVertexArrays(1, &m_VertexArrayID);
   }

   void VertexArray::Bind() const
   {
      glBindVertexArray(m_VertexArrayID);
   }

   //static
   void VertexArray::Unbind()
   {
      glBindVertexArray(0);
   }


   uint32_t GetGLAttributeSize(AttributeType type)
   {
      return type == AttributeType::FLOAT ? sizeof(GLfloat)
         : type == AttributeType::VEC2  ? sizeof(GLfloat) * 2
         : type == AttributeType::VEC3  ? sizeof(GLfloat) * 3
         : 0; 
   }
   uint32_t GetGLAttributeElementCount(AttributeType type)
   {
      return type == AttributeType::FLOAT ? 1
         : type == AttributeType::VEC2  ? 2
         : type == AttributeType::VEC3  ? 3
         : 0; 
   }
   GLenum GetGLAttributeType(AttributeType type)
   {
      return type == AttributeType::FLOAT ? GL_FLOAT
         : type == AttributeType::VEC2  ? GL_FLOAT
         : type == AttributeType::VEC3  ? GL_FLOAT
         : GL_NONE; 
   }

   void VertexArray::ResetAttributes()
   {
      Bind();

      for (const auto& attribute : m_UsedAttributes) {
         glDisableVertexAttribArray(attribute.slot);
      }
      m_UsedAttributes.clear();
   }

   void VertexArray::SetAttribute(uint32_t slot, AttributeType type) 
   {
      m_UsedAttributes.emplace_back(slot, type);
      m_AttributeSizeInBytes += GetGLAttributeSize(type);
   }

   void VertexArray::UploadAttributes()
   {
      Bind();
      std::sort(m_UsedAttributes.begin(), m_UsedAttributes.end());
      
      uint64_t usedBytes = 0;
      for(const auto& attribute : m_UsedAttributes)
      {
         glVertexAttribPointer(
            attribute.slot,                               // slot
            GetGLAttributeElementCount(attribute.type),  // size (element count)
            GetGLAttributeType(attribute.type),         // type
            GL_FALSE,                                  // normalized
            m_AttributeSizeInBytes,                   // stride
            (const void*)usedBytes                   // pointer
            );
         glEnableVertexAttribArray(attribute.slot);

         /*std::cout
         << "Stride: " << m_AttributeSizeInBytes

         << '\n';
   */
         usedBytes += GetGLAttributeSize(attribute.type);
      }
   }
} // namespace EaseGL


#endif
/*-- File: src/VertexArray.cpp end --*/
