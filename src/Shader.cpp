#include "Shader.hpp" 
#ifdef EASEGL_IMPLEMENTATION

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

#include "GLBuffer.hpp"

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