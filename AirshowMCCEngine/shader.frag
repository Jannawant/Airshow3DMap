#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

uniform sampler2D s_texture;
uniform sampler2D s_bumpMap;
varying   vec3      fragColor;
varying vec3 worldPosition;
varying vec2 fragTexCoord;
varying vec3 positionToLight;
varying vec3 positionToEye;
varying vec3 fragNormal;

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

void main()
{
    //vec3 toLight = normalize(positionToLight);
    vec3 normal = normalize(worldPosition);
    float intensity = LightIntensity(normal, normalize(positionToLight), normalize(positionToEye), g_diffuseSpecularAmbientShininess);
    //float diffuse = max(dot(toLight, normal), 0.0);

    vec4 baseColor = texture2D(s_texture, fragTexCoord);
    vec4 bumpColor = texture2D(s_bumpMap, fragTexCoord);
    //vec4 bumpColor = texture2D(s_bumpMap, gl_PointCoord);
    vec4 temp = mix(bumpColor, baseColor, 0.4);
    gl_FragColor =     baseColor   ;
    //gl_FragColor = vec4(bumpColor);
}
