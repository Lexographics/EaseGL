#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#pragma once

#include <glad/glad.h>
#include "Texture.hpp"
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