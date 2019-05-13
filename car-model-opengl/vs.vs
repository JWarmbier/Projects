#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec4 aBitangent;
layout (location = 5) in vec3 aColor;
layout (location = 6) in float aHasTexture;
layout (location = 7) in vec3 aColorSpecular;
layout (location = 8) in float aOpacity;

out vec3 SkyboxTexCoords;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 Color;
out vec3 PosLightNT;
out float opacity;
out float hasTexture;
out vec3 colorSpecular;
out mat3 TBN;
out float fogCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
	vec4 FragPosLightSpace;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	SkyboxTexCoords = aPos;

	opacity = aOpacity;
    TexCoords = aTexCoords; 
    FragPos = vec3(model * vec4(aPos, 1.0));

	Normal = aNormal;
	Color = aColor;
	hasTexture = aHasTexture;
	colorSpecular = aColorSpecular;
	PosLightNT =lightPos ;

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
   T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

	vec4 help = view*model*vec4(aPos, 1.0);
	fogCoord=abs(help.z); 
}