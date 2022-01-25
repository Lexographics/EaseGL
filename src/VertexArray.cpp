#include "VertexArray.hpp"  

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