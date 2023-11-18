#version 330 core

out vec4 FragColor;
in vec3 vertexColor; //the input from the above vertexShader, matching name and type
in vec2 texCoord;
uniform float gloColor; //global uniform color

uniform sampler2D texmex1;
uniform sampler2D texmex2;

void main()
{
   //FragColor = vec4(vertexColor, 1.0);
   //FragColor = vec4(vertexColor.x - gloColor, vertexColor.y + 1-gloColor, vertexColor.z - gloColor, 1.0); //cool new technique makes a wild oscillating color rectangle
   
   //mixes two textures in the same area, the third argument determines the mix
   //0.2 returns 80% the first color, 20% the second.
   FragColor = mix(texture(texmex1, texCoord), texture(texmex2, texCoord), 0.6);
}