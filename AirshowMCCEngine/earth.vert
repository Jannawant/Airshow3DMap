//attribute highp vec4 qt_Vertex;
//attribute highp vec4 qt_MultiTexCoord0;
//uniform highp mat4 qt_ModelViewProjectionMatrix;
//varying highp vec4 qt_TexCoord0;

//void main(void)
//{
//    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
//    qt_TexCoord0 = qt_MultiTexCoord0;
//}

uniform    mat4 g_matModelView;
uniform    mat4 g_matProj;
attribute vec4 g_vPosition;
attribute vec3 g_vNormal;
attribute vec3 g_vColor;
attribute vec2 g_vTexCoord;
varying vec3 fragColor;
varying vec2 fragTexCoord;
varying vec3 fragNormal;
varying vec3 fragPosition;
void main()
{
   vec4 vPositionES = g_matModelView * g_vPosition;
   fragPosition = vec3(g_matModelView * g_vPosition);
   fragColor = g_vColor;
   //fragNormal = normalize(g_vNormal);
   fragNormal = vec3(g_matModelView * vec4(g_vNormal, 0.0));
   fragTexCoord = g_vTexCoord;
   gl_PointSize = 5.0;
   gl_Position = g_matProj * vPositionES;
}
