#include "geometry.h"
#include <cmath>
#include "codestd.h"

glm::mat4x4 a(1.0);
const Mat4x4 Mat4x4::g_Identity = Mat4x4(a);

Vec3::Vec3(const Vec4 &v4)
{
    m_vec3 = v4.getVec3();
}

bool operator ==(const Vec3 &op1,const Vec3 &op2)
{
    return (op1.getVec3() == op2.getVec3());
}
bool operator !=(const Vec3 &op1,const Vec3 &op2)
{
    return !(op1==op2);
}

//bool operator >(const Vec3 &op1, const Vec3 &op2)
//{
//    return op1.getVec3() > op2.getVec3();
//}
//bool operator <(const Vec3 &op1, const Vec3 &op2)
//{
//        return op1.getVec3() < op2.getVec3();
//}

Vec3 operator * (const Vec3& op1, const float &op2)
{
    return Vec3(op1.getVec3() * op2);
}
Vec3 operator * (const float &op1 , const Vec3& op2 )
{
    return Vec3(op1 * op2.getVec3());
}
Vec3 operator * (const Vec3& op1, const Vec3& op2)
{
    return Vec3(op1.getVec3() * op2.getVec3());
}

Vec3 operator /(const Vec3 &op1, const float &op2)
{
    return Vec3(op1.getVec3() / op2);
}

Vec3 operator /(const float &op1, const Vec3 &op2)
{
    return Vec3(op1 / op2.getVec3());
}

Vec3 operator /(const Vec3 &op1, const Vec3 &op2)
{
    return Vec3(op1.getVec3() / op2.getVec3());
}

Vec3 operator - (const Vec3 &op1, const Vec3 &op2)
{
    return Vec3(op1.getVec3() - op2.getVec3());
}
Vec3 operator + (const Vec3 &op1, const Vec3 &op2)
{
    return Vec3(op1.getVec3() + op2.getVec3());
}


bool operator ==(const Vec4 &op1,const Vec4 &op2)
{
    return (op1.getVec4() == op2.getVec4());
}
bool operator !=(const Vec4 &op1,const Vec4 &op2)
{
    return !(op1==op2);
}
//bool operator >(const Vec4 &op1, const Vec4 &op2)
//{
//    return op1.getVec4() > op2.getVec4();
//}
//bool operator <(const Vec4 &op1, const Vec4 &op2)
//{
//        return op1.getVec4() < op2.getVec4();
//}
Vec4 operator * (const Vec4& op1, const float &op2)
{
    return Vec4(op1.getVec4() * op2);
}
Vec4 operator * (const float &op1 , const Vec4& op2 )
{
    return Vec4(op1 * op2.getVec4());
}
Vec4 operator * (const Vec4& op1, const Vec4& op2)
{
    return Vec4(op1.getVec4() * op2.getVec4());
}


Vec4 operator /(const Vec4 &op1, const float &op2)
{
    return Vec4(op1.getVec4() / op2);
}

Vec4 operator /(const float &op1, const Vec4 &op2)
{
    return Vec4(op1 / op2.getVec4());
}

Vec4 operator /(const Vec4 &op1, const Vec4 &op2)
{
    return Vec4(op1.getVec4() / op2.getVec4());
}


Vec4 operator - (const Vec4 &op1, const Vec4 &op2)
{
    return Vec4(op1.getVec4() - op2.getVec4());
}
Vec4 operator + (const Vec4 &op1, const Vec4 &op2)
{
    return Vec4(op1.getVec4() + op2.getVec4());
}


Mat4x4 Quaternion::toMat4x4() const
{
    return Mat4x4(glm::toMat4(m_quat));
}

void Quaternion::build(const Mat4x4 &mat)
{
    m_quat = glm::toQuat(mat.getMat4x4());
}


//own function

Mat4x4::Mat4x4(const glm::mat4x4 &mat)
{
    memcpy(&m_mat4x4, &mat, sizeof(mat));
}

Mat4x4 operator*(const Mat4x4 &a,const Mat4x4 &b)
{
    glm::mat4x4 out = a.getMat4x4() * b.getMat4x4();
    return Mat4x4(out);
}


#define _PLANE_EQ_DOT_EPSILON 0.999
#define _PLANE_EQ_D_EPSILON 0.0001

void Plane::setNormal(const Vec3 &p_normal) {

    m_normal = p_normal;
}

void Plane::normalize()
{

    float l = m_normal.length();
    if (l == 0) {
        *this = Plane(0, 0, 0, 0);
        return;
    }
    m_normal /= l;
    m_d /= l;
}

Plane Plane::normalized() const {

    Plane p = *this;
    p.normalize();
    return p;
}


bool Plane::is_point_over(const Vec3 &p_point) const {

    return (glm::dot(m_normal.getVec3(),p_point.getVec3()) > m_d);
}

//float Plane::distance_to(const Vec3 &p_point) const
//{
//    return glm::dot(m_normal.getVec3(),p_point.getVec3()) - m_d;
//}

bool Plane::inside(const Vec3 &point) const
{
    float dist = glm::dot(m_normal.getVec3(),point.getVec3()) - m_d;
    dist = ABS(dist);
    return (dist <= CMP_EPSILON);
    //return (dist > 0.0f);
}


////ДОДЕЛАТЬ!!
bool Plane::inside(const Vec3 &point, float radius) const
{
    float dist = glm::dot(m_normal.getVec3(),point.getVec3()) - m_d;
    dist = ABS(dist);
    return (dist <= CMP_EPSILON);
    //return (dist >= -radius);
}

bool Plane::has_point(const Vec3 &p_point, float _epsilon) const
{
    float dist = glm::dot(m_normal.getVec3(),p_point.getVec3()) - m_d;
    dist = ABS(dist);
    return (dist <= _epsilon);
}

Plane::Plane(const Vec3 &p_point, const Vec3 &p_normal) :
        m_normal(p_normal),
        m_d(glm::dot(p_normal.getVec3(),p_point.getVec3()))
{
}

//ClockDirection == CLOCKWISE
void Plane::init(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2)
{
    m_normal = Vec3(glm::cross((p0 - p2).getVec3(),(p1 - p2).getVec3()));
    m_normal.normalize();
    m_d = m_normal.dot(p1);
    this->normalize();
}

Plane::Plane(const Vec3 &p_point1, const Vec3 &p_point2, const Vec3 &p_point3, ClockDirection p_dir)
{

    if (p_dir == CLOCKWISE)
        m_normal = Vec3(glm::cross((p_point1 - p_point3).getVec3(),(p_point1 - p_point2).getVec3()));
    else
        m_normal = Vec3(glm::cross((p_point1 - p_point2).getVec3(),(p_point1 - p_point3).getVec3()));

    m_normal.normalize();
    m_d = m_normal.dot(p_point1);
}

bool Plane::operator==(const Plane &p_plane) const {

    return m_normal.getVec3() == p_plane.m_normal.getVec3() && m_d == p_plane.m_d;
}

bool Plane::operator!=(const Plane &p_plane) const {

    return m_normal.getVec3() != p_plane.m_normal.getVec3() || m_d != p_plane.m_d;
}


Vec3 Plane::get_any_point() const {

    return getNormal() * m_d;
}

Vec3 Plane::get_any_perpendicular_normal() const {

    static const Vec3 p1 = Vec3(1, 0, 0);
    static const Vec3 p2 = Vec3(0, 1, 0);
    Vec3 p;

    if (ABS(m_normal.dot(p1)) > 0.99) // if too similar to p1
        p = p2; // use p2
    else
        p = p1; // use p1

    p -= m_normal * m_normal.dot(p);
    p.normalize();

    return p;
}

/* intersections */

bool Plane::intersect_3(const Plane &p_plane1, const Plane &p_plane2, Vec3 *r_result) const {

    const Plane &p_plane0 = *this;
    Vec3 normal0 = p_plane0.m_normal;
    Vec3 normal1 = p_plane1.m_normal;
    Vec3 normal2 = p_plane2.m_normal;

    float denom = glm::dot(glm::cross(normal0.getVec3(), normal1.getVec3()),normal2.getVec3());

    if (ABS(denom) <= CMP_EPSILON)
        return false;

    if (r_result) {
        *r_result = Vec3((glm::cross(normal1.getVec3(), normal2.getVec3()) * p_plane0.m_d) +
                         (glm::cross(normal2.getVec3(), normal0.getVec3()) * p_plane1.m_d) +
                         (glm::cross(normal0.getVec3(), normal1.getVec3()) * p_plane2.m_d)) /
                    denom;
    }
    return true;
}

bool Plane::intersects_ray(const Vec3 &p_from, const Vec3 &p_dir, Vec3 *p_intersection) const {

    Vec3 segment = p_dir;
    float den = m_normal.dot(segment);

    //printf("den is %i\n",den);
    if (std::abs(den) <= CMP_EPSILON)
    {
        return false;
    }
    float dist = (m_normal.dot(p_from) - m_d) / den;
    //printf("dist is %i\n",dist);
    if (dist > CMP_EPSILON) { //this is a ray, before the emitting pos (p_from) doesn't exist

        return false;
    }
    dist = -dist;
    *p_intersection = p_from + segment * dist;
    return true;
}

bool Plane::intersects_segment(const Vec3 &p_begin, const Vec3 &p_end, Vec3 *p_intersection) const {

    Vec3 segment = p_begin - p_end;
    float den = m_normal.dot(segment);
    //printf("den is %i\n",den);
    if (std::abs(den) <= CMP_EPSILON) {

        return false;
    }
    float dist = (m_normal.dot(p_begin) - m_d) / den;
    //printf("dist is %i\n",dist);

    if (dist < -CMP_EPSILON || dist > (1.0 + CMP_EPSILON)) {

        return false;
    }
    dist = -dist;
    *p_intersection = p_begin + segment * dist;

    return true;
}

/* misc */

bool Plane::is_almost_like(const Plane &p_plane) const {

    return (m_normal.dot(p_plane.m_normal) > _PLANE_EQ_DOT_EPSILON && std::abs(m_d - p_plane.m_d) < _PLANE_EQ_D_EPSILON);
}

//Plane::operator std::string() const {

//    return m_normal. std::string() + ", " + m_d;
//}



Frustum::Frustum()
{
//    m_Fov = GCC_PI/4.0f;    //default field of view is 90 degrees
//    m_Aspect = 1.0f;        //default aspect ratio is 1:1
//    m_Near = 1.0f;          //default near clip plane is 1m away from the camera
//    m_Far = 1000.0f;        //default near clip plane is 100m away from the camera
    m_Fov = 40.0f;    //default field of view is 90 degrees
    m_Aspect =  1920.0/static_cast<float>(1080);        //default aspect ratio is 1:1
    m_Near = 0.1f;          //default near clip plane is 1m away from the camera
    m_Far = 200.0f;        //default near clip plane is 100m away from the camera
}

void Frustum::init(const float fov, const float aspect, const float nearClip, const float farClip)
{
    m_Fov = fov;
    m_Aspect = aspect;
    m_Near = nearClip;
    m_Far = farClip;

    float tanFovOver2 = (float)tan(m_Fov/2.0f);
    Vec3 nearRight = (m_Near * tanFovOver2) * m_Aspect * g_Right;
    Vec3 farRight = (m_Far * tanFovOver2) * m_Aspect * g_Right;
    Vec3 nearUp = (m_Near * tanFovOver2 ) * g_Up;
    Vec3 farUp = (m_Far * tanFovOver2)  * g_Up;

    // points start in the upper right and go around clockwise
    m_NearClip[0] = (m_Near * g_Forward) - nearRight + nearUp;
    m_NearClip[1] = (m_Near * g_Forward) + nearRight + nearUp;
    m_NearClip[2] = (m_Near * g_Forward) + nearRight - nearUp;
    m_NearClip[3] = (m_Near * g_Forward) - nearRight - nearUp;

    m_FarClip[0] = (m_Far * g_Forward) - farRight + farUp;
    m_FarClip[1] = (m_Far * g_Forward) + farRight + farUp;
    m_FarClip[2] = (m_Far * g_Forward) + farRight - farUp;
    m_FarClip[3] = (m_Far * g_Forward) - farRight - farUp;

    // now we have all eight points. Time to construct 6 planes.
    // the normals point away from you if you use counter clockwise verts.

    Vec3 origin(0.0f, 0.0f, 0.0f);
    m_Planes[Near].init(m_NearClip[2], m_NearClip[1], m_NearClip[0]);
    m_Planes[Far].init(m_FarClip[0], m_FarClip[1], m_FarClip[2]);
    m_Planes[Right].init(m_FarClip[2], m_FarClip[1], origin);
    m_Planes[Top].init(m_FarClip[1], m_FarClip[0], origin);
    m_Planes[Left].init(m_FarClip[0], m_FarClip[3], origin);
    m_Planes[Bottom].init(m_FarClip[3], m_FarClip[2], origin);
}

bool Frustum::inside(const Vec3 &point) const
{
    for(int i=0; i<NumPlanes; i++)
    {
        if(!m_Planes[i].inside(point))
            return false;
    }
    return true;
}

bool Frustum::inside(const Vec3 &point, const float radius) const
{
    for(int i=0; i<NumPlanes; i++)
    {
        if(!m_Planes[i].inside(point,radius))
            return false;
    }
    //otherwise we are fully in view
    return true;
}

// A structure for our custom vertex type. We added texture coordinates
struct vertexColored
{
    Vec3 position; // The position
    Color color;    // The color
    static const DWORD FVF;
};

void Frustum::render()
{
    vertexColored verts[24];
    for (int i=0; i<8; ++i)
    {
        verts[i].color = g_White;
    }

    for (int i=0; i<8; ++i)
    {
        verts[i+8].color = g_Red;
    }

    for (int i=0; i<8; ++i)
    {
        verts[i+16].color = g_Blue;
    }


    // Draw the near clip plane
    verts[0].position = m_NearClip[0];	verts[1].position = m_NearClip[1];
    verts[2].position = m_NearClip[1];	verts[3].position = m_NearClip[2];
    verts[4].position = m_NearClip[2];	verts[5].position = m_NearClip[3];
    verts[6].position = m_NearClip[3];	verts[7].position = m_NearClip[0];

    // Draw the far clip plane
    verts[8].position = m_FarClip[0];	verts[9].position = m_FarClip[1];
    verts[10].position = m_FarClip[1];	verts[11].position = m_FarClip[2];
    verts[12].position = m_FarClip[2];	verts[13].position = m_FarClip[3];
    verts[14].position = m_FarClip[3];	verts[15].position = m_FarClip[0];

    // Draw the edges between the near and far clip plane
    verts[16].position = m_NearClip[0];	verts[17].position = m_FarClip[0];
    verts[18].position = m_NearClip[1];	verts[19].position = m_FarClip[1];
    verts[20].position = m_NearClip[2];	verts[21].position = m_FarClip[2];
    verts[22].position = m_NearClip[3];	verts[23].position = m_FarClip[3];

    //DWORD oldLightMode;
//    DXUTGetD3D9Device()->GetRenderState( D3DRS_LIGHTING, &oldLightMode );
//    DXUTGetD3D9Device()->SetRenderState( D3DRS_LIGHTING, FALSE );

//    DXUTGetD3D9Device()->SetFVF( D3D9Vertex_Colored::FVF );
//    DXUTGetD3D9Device()->DrawPrimitiveUP( D3DPT_LINELIST, 12, verts, sizeof(D3D9Vertex_Colored) );

//    DXUTGetD3D9Device()->SetRenderState( D3DRS_LIGHTING, oldLightMode );
}

std::ostream &operator <<(std::ostream &os, const Mat4x4 &m)
{
    for(int i=0 ; i<m.getMat4x4().length(); i++)
        for(int j=0 ; j<m.getMat4x4()[i].length(); j++)
            os << m.getMat4x4()[i][j] << " ";
    return os;
}

std::ostream &operator <<(std::ostream &os, const Vec4 &v)
{
    for(int i=0 ; i<v.getVec4().length(); i++)
        os << v.getVec4()[i] << " ";
    return os;
}

std::ostream &operator <<(std::ostream &os, const Vec3 &v)
{
    for(int i=0 ; i<v.getVec3().length(); i++)
        os << v.getVec3()[i] << " ";
    return os;
}
