#include "GLBuffer.hpp"

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