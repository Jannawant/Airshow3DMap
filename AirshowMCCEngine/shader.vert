uniform    mat4 g_matModelView;
//uniform    mat4 g_matModelViewProj;
uniform    mat4 g_matProj;
uniform mat4 g_matModelMatrix;
uniform vec3 g_cameraLightPosition;
uniform vec3 g_cameraEye;
attribute vec4 g_vPosition;
attribute vec3 g_vNormal;
attribute vec3 g_vColor;
attribute vec2 g_vTexCoord;
varying vec3 fragColor;
varying vec2 fragTexCoord;
varying vec3 fragNormal;
varying vec3 worldPosition;
varying vec3 positionToLight;
varying vec3 positionToEye;
void main()
{
   vec4 vPositionES = g_matModelView * g_vPosition;
   //fragPosition = vec3(g_matModelView * g_vPosition);
   worldPosition = g_vPosition.xyz;
   //worldPosition = vec3(g_matModelMatrix * g_vPosition);
   positionToLight = g_cameraLightPosition - worldPosition;
   positionToEye = g_cameraEye - worldPosition;
   fragColor = g_vColor;
   //fragNormal = normalize(g_vNormal);
   fragNormal = vec3(g_matModelMatrix * vec4(g_vNormal, 0.0));
   fragTexCoord = g_vTexCoord;
   gl_PointSize = 5.0;
   gl_Position = g_matProj *vPositionES;
}
