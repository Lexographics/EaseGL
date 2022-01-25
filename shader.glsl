#shader vertex

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aColor;

out vec2 texCoords;
out vec3 color;

uniform mat4 u_Model;

void main()
{
   gl_Position = u_Model * vec4(aPos, 1.0);
   texCoords = aTexCoords;
   color = aColor;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 texCoords;
in vec3 color;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
      
      FragColor = mix( mix(texture(tex1, texCoords), texture(tex2, texCoords), 0.5 ), vec4(color, 1.0), 0.5);

      if(FragColor.a <= 0.1)
         discard;
}