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
   //FragColor = mix(texture(texmex1, texCoord), texture(texmex2, texCoord), 0.6);

   //Replaces the transparent bit of the 2nd image with the 1st image, fixing the darkening caused by using the mix() function.
   if(texture(texmex2, texCoord).w < 0.1)
   {
        FragColor = texture(texmex1, texCoord);
   }
   else
   {
        FragColor = texture(texmex2, texCoord);
        FragColor = FragColor + vec4(gloColor, 0, gloColor/2, 1.0);
   }
}