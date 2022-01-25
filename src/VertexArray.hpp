#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#pragma once

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