uniform    mat4 g_WVP;
uniform mat4 gWorld;
uniform vec3 g_cameraLightPosition;
uniform vec3 g_cameraEye;
uniform mat4 gLightWVP;  
uniform sampler2D s_bumpMap;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec2 TexCoord;
attribute vec3 Tangent;

varying vec4 LightSpacePos;  
varying vec2 fragTexCoord;
varying vec3 fragNormal;
varying vec3 fragTangent;
varying vec3 worldPosition;
varying vec3 positionToLight;
varying vec3 positionToEye;

void main()
{

    vec3 heightBumpNormal = texture2D(s_bumpMap, TexCoord).xyz;
    heightBumpNormal = 2.0 * heightBumpNormal - 1.0; //vec3(1.0,1.0,1.0);
    vec4 height = vec4(heightBumpNormal, 0.0);
    height = height * 0.05;
    vec4 newPosition = Position + vec4(0.0, 1.0, 0.0, 0.0) * height;

   gl_Position = g_WVP * newPosition;

    //   vec4 height = (normalize(texture2D(s_bumpMap, TexCoord).xyz * 2.0 - vec3(1.0,1.0,1.0)),  0.0);
   //   gl_Position.z = gl_Position.z + 0.1;
   worldPosition = (gWorld * Position).xyz;
   fragNormal = (gWorld * vec4(Normal, 0.0)).xyz;
   fragTangent = (gWorld * vec4(Tangent, 0.0)).xyz;
   LightSpacePos = gLightWVP * Position;
  
   positionToLight = g_cameraLightPosition - worldPosition;
   positionToEye = g_cameraEye - worldPosition;

   fragTexCoord = TexCoord;
   gl_PointSize = 5.0;
}
