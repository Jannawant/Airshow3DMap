#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

uniform sampler2D s_texture;
uniform sampler2D s_bumpMap;
varying   vec3      fragColor;
varying vec3 fragPosition;
varying vec2 fragTexCoord;
varying vec3 fragNormal;
uniform vec3 lightPos;

void main()
{
    float distance = length(lightPos - fragPosition);
    vec3 lightVector = normalize(lightPos - fragPosition);
    float diffuse = max(dot(fragNormal, lightVector),0.0);
    diffuse = diffuse * (1.0 / (1.0 + (0.10 * distance)));
    diffuse = diffuse + 0.6;
    vec4 baseColor = texture2D(s_texture, fragTexCoord);
    vec4 bumpColor = texture2D(s_bumpMap, fragTexCoord);
    vec4 temp = mix(bumpColor, baseColor, 0.4);
    gl_FragColor = diffuse * temp;
    //gl_FragColor = vec4(bumpColor);
}
