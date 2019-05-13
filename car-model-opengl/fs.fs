#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal; 
in vec3 FragPos;
in vec3 Color;
in vec3 PosLightNT;
in float opacity;
in float hasTexture;
in vec3 colorSpecular;
in mat3 TBN;
in vec3 SkyboxTexCoords;
in float fogCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D shadowMap;
uniform samplerCube skybox;


uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float transparency;
uniform float isSkybox;
uniform float inMirror;
uniform vec4 distortion;

uniform float FogDensity;


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 


in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} fs_in;

  
uniform Material material;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005f;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;



    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


void main()
{    

    // diffuse 
   // vec3 norm = normalize(Normal);
   // vec3 lightDir = normalize(lightPos - FragPos);
   // float diff = max(dot(norm, lightDir), 0.0);
  //  vec3 diffuse = diff * lightColor;
  vec3 FogColor = vec3(0.6,0.6,0.6);
  float FogFactor = 1.0 - clamp(exp(-FogDensity*fogCoord),0.0,1.0);
 
  	
    // diffuse 
	if(isSkybox == 1){
		FragColor = mix(texture(skybox, SkyboxTexCoords), vec4(FogColor,1.0),FogFactor);
	}else{
		if(opacity < 0.1)discard;

	if(hasTexture == 1){
	    vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
        // transform normal vector to range [-1,1]
		normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
		
		vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
		// ambient
		 vec3 ambient = material.ambient * color;

		vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = diff * color;
		// specular
		vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);  
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
		vec3 specular = vec3(0.2) * spec;

        //shadow
        float shadow = ShadowCalculation(fs_in.FragPosLightSpace); 
        FragColor = mix(vec4(ambient + (1.0 - shadow) * (diffuse + specular), transparency),vec4(FogColor,1.0),FogFactor);

		//FragColor = texture(texture_diffuse1, fs_in.TexCoords);
	} else {
		//ambient
		vec3 ambient = material.ambient * lightColor;

		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(PosLightNT - FragPos); 
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		//specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = lightColor * (spec * colorSpecular); 

        //shadow
        float shadow = ShadowCalculation(fs_in.FragPosLightSpace); 

        FragColor = mix(vec4((ambient + (1.0 - shadow) * (diffuse + specular))* Color, transparency),vec4(FogColor,1.0),FogFactor);
	}
	}
	

    if(inMirror == 1) FragColor *=distortion;
}