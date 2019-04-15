#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                                 
    vec3 Direction;                                                                 
};

uniform sampler2D s_texture;
uniform sampler2D s_normalMap;
uniform vec3 gEyeWorldPos;  
uniform float gSpecularPower;
uniform float gMatSpecularIntensity;
uniform DirectionalLight gDirectionalLight;

varying vec4 LightSpacePos; 
varying vec3 fragTangent;
varying vec3 fragNormal;
varying vec3 worldPosition;
varying vec2 fragTexCoord;
varying vec3 positionToLight;
varying vec3 positionToEye;


uniform vec4 g_diffuseSpecularAmbientShininess;

float LightIntensity(vec3 norm, vec3 toLight, vec3 toEye, vec4 diffuseSpecularAmbientShininess)
{
	vec3 toReflectedLight = reflect(-toLight, norm);
	float diffuse = max(dot(toLight, norm), 0.0);
	float specular = max(dot(toReflectedLight, toEye),0.0);
	specular = pow(specular, diffuseSpecularAmbientShininess.w);
	return (diffuseSpecularAmbientShininess.x * diffuse) +
		(diffuseSpecularAmbientShininess.y * specular) +
		diffuseSpecularAmbientShininess.z;
}


vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal,  float ShadowFactor)                                                  
{                                                                                           
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0);
    float DiffuseFactor = dot(Normal, -LightDirection);                                     
                                                                                            
    vec4 DiffuseColor  = vec4(0.0, 0.0, 0.0, 0.0);                                                  
    vec4 SpecularColor = vec4(0.0, 0.0, 0.0, 0.0);                                                 
                                                                                            
    if (DiffuseFactor > 0.0)
    {                                                                
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0);
                                                                                            
        vec3 VertexToEye = normalize(gEyeWorldPos - worldPosition);                             
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     
        float SpecularFactor = dot(VertexToEye, LightReflect);                                      
        if (SpecularFactor > 0.0) {                                                           
            SpecularFactor = pow(SpecularFactor, gSpecularPower);                               
            SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0);
        }                                                                                   
    }                                                                                       
                                                                                            
    return (AmbientColor + ShadowFactor * (DiffuseColor + SpecularColor));                  
}         

vec4 CalcDirectionalLight(vec3 Normal)                                                      
{                                                                                                
//    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal, 1.0);  
	BaseLight gLight;
	gLight.Color = vec3(1.0, 1.0, 1.0);
	gLight.AmbientIntensity = 1.0;
	gLight.DiffuseIntensity = 0.0;
	return CalcLightInternal(gLight, vec3(0.0,0.0,-1.0), Normal, 1.0);  
}          


vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(fragNormal);
    vec3 Tangent = normalize(fragTangent);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture2D(s_normalMap, fragTexCoord).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main()
{
    vec3 normal = normalize(worldPosition);
    //float intensity = LightIntensity(normal, normalize(positionToLight), normalize(positionToEye), g_diffuseSpecularAmbientShininess);
    //float diffuse = max(dot(toLight, normal), 0.0);

    vec3 Normal = CalcBumpedNormal();
    vec4 TotalLight = CalcDirectionalLight(Normal);

    vec4 baseColor = texture2D(s_texture, fragTexCoord);
    vec4 normalColor = texture2D(s_normalMap, fragTexCoord);
    //vec4 temp = mix(baseColor, bumpColor, 0.5);
    gl_FragColor = baseColor * TotalLight;
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
