#include "Framebuffer.hpp"  

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