#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <iostream>

using std::ostream;

enum ClockDirection {

    CLOCKWISE,
    COUNTERCLOCKWISE
};

#define CMP_EPSILON 0.00001
#define CMP_EPSILON2 (CMP_EPSILON * CMP_EPSILON)
#define ABS(m_v) ((m_v < 0) ? (-(m_v)) : (m_v))

const float  GCC_PI = 3.14159265358979f;
const float  GCC_2PI = 2 * GCC_PI;
#define RADIANS_TO_DEGREES(x) ((x) * 180.0f / GCC_PI)
#define DEGREES_TO_RADIANS(x) ((x) * GCC_PI / 180.0f)

typedef glm::vec2 Vec2;
class Vec4;
class Mat4x4;

class Vec3
{
public:
    glm::vec3 m_vec3;
    Vec3(){}
    explicit Vec3(const glm::vec3 &v){m_vec3 = v;}
    explicit Vec3(glm::vec4 v){m_vec3 = glm::vec3(v);}
    explicit Vec3(const Vec4 &v4);
    Vec3(const float _x, const float _y, const float _z){m_vec3 = glm::vec3(_x,_y,_z);}
    //Vec3(const Vec3 &v3);
    glm::vec3 getVec3() const {return m_vec3;}
    glm::vec4 getVec4() const {return glm::vec4(m_vec3,1.0);}
    float length() const { return m_vec3.length(); }
    inline Vec3 normalize() const { return Vec3(glm::normalize(m_vec3));} //static_cast<Vec3*>
    inline float dot(const Vec3 &b) const {return glm::dot(m_vec3, b.getVec3());}
    inline Vec3 cross(const Vec3 &b) const
    {
        return Vec3(glm::cross(m_vec3,b.getVec3()));
    }
    inline Vec3 radiansToDegrees() const
    {
        return Vec3(RADIANS_TO_DEGREES(m_vec3.x), RADIANS_TO_DEGREES(m_vec3.y), RADIANS_TO_DEGREES(m_vec3.z));
    }
    inline Vec3 degreesToRadians() const
    {
        return Vec3(DEGREES_TO_RADIANS(m_vec3.x), DEGREES_TO_RADIANS(m_vec3.y), DEGREES_TO_RADIANS(m_vec3.z));
    }

//    Vec3 operator =(const Vec3& op1)
//    {
//        if(&op1 == *this)
//            return *this;
//        m_vec3.x = op1.getVec3().x;
//        m_vec3.y = op1.getVec3().y;
//        m_vec3.z = op1.getVec3().z;
//        return *this;
//    }


    Vec3 operator -() const
    {
        return Vec3(-m_vec3);
    }
    Vec3 operator +() const
    {
        return *this;
    }
    Vec3 operator +=(const Vec3 & op1)
    {
        m_vec3 += op1.getVec3();
        return *this;
    }
    Vec3 operator -=(const Vec3 & op1)
    {
        m_vec3 -= op1.getVec3();
        return *this;
    }
    Vec3 operator *=(const Vec3 & op1)
    {
        m_vec3 *= op1.getVec3();
        return *this;
    }
    Vec3 operator *=(const float & op1)
    {
        m_vec3 *= op1;
        return *this;
    }
    Vec3 operator /=(const Vec3 & op1)
    {
        m_vec3 /= op1.getVec3();
        return *this;
    }
    Vec3 operator /=(const float & op1)
    {
        m_vec3 /= op1;
        return *this;
    }
    friend bool operator ==(const Vec3 &op1,const Vec3 &op2);
    friend bool operator !=(const Vec3 &op1,const Vec3 &op2);
    friend bool operator >(const Vec3 &op1, const Vec3 &op2);
    friend bool operator <(const Vec3 &op1, const Vec3 &op2);
    friend Vec3 operator * (const Vec3& op1, const float &op2);
    friend Vec3 operator * (const float &op1 , const Vec3& op2 );
    friend Vec3 operator * (const Vec3& op1, const Vec3& op2);
    friend Vec3 operator / (const Vec3& op1, const float &op2);
    friend Vec3 operator / (const float &op1 , const Vec3& op2 );
    friend Vec3 operator / (const Vec3& op1, const Vec3& op2);
    friend Vec3 operator - (const Vec3 &op1, const Vec3 &op2);
    friend Vec3 operator + (const Vec3 &op1, const Vec3 &op2);

};

ostream & operator <<(ostream & os, const Vec3 & v);

class Vec4
{
public:
    glm::vec4 m_vec4;
    Vec4(){}
    explicit Vec4(const Vec3 &v3){ m_vec4 = glm::vec4(v3.getVec3(),1.0f);}
    explicit Vec4(const glm::vec3 &v){m_vec4 = glm::vec4(v,1.0f);}
    explicit Vec4(const glm::vec4 &v){m_vec4 = v;}
    Vec4(const float _x, const float _y, const float _z, const float _w){m_vec4 = glm::vec4(_x,_y,_z,_w);}
    glm::vec4 getVec4() const {return m_vec4;}
    glm::vec3 getVec3() const {return glm::vec3(m_vec4);}
    float length() const
    {
        return m_vec4.length();
    }
    Vec4 normalize() const
    {
        glm::vec4 temp = glm::normalize(m_vec4);
        return Vec4(temp);
    }
    float dot(const Vec4 &b) const
    {
        return glm::dot(m_vec4,b.getVec4());
    }
    Vec4 cross(const Vec4 &b) const
    {
        glm::vec3 temp = glm::cross(getVec3(),b.getVec3());
        return Vec4(temp);
    }
    Vec4 operator -() const
    {
        return Vec4(-m_vec4);
    }
    Vec4 operator +() const
    {
        return *this;
    }
    Vec4 operator +=(const Vec4 & op1)
    {
        m_vec4 += op1.getVec4();
        return *this;
    }
    Vec4 operator -=(const Vec4 & op1)
    {
        m_vec4 -= op1.getVec4();
        return *this;
    }
    Vec4 operator *=(const Vec4 & op1)
    {
        m_vec4 *= op1.getVec4();
        return *this;
    }
    Vec4 operator *=(const float & op1)
    {
        m_vec4 *= op1;
        return *this;
    }
    Vec4 operator /=(const Vec4 & op1)
    {
        m_vec4 /= op1.getVec4();
        return *this;
    }
    Vec4 operator /=(const float &op1)
    {
        m_vec4 /= op1;
        return *this;
    }
    friend bool operator ==(const Vec4 &op1,const Vec4 &op2);
    friend bool operator !=(const Vec4 &op1,const Vec4 &op2);
    friend bool operator >(const Vec4 &op1, const Vec4 &op2);
    friend bool operator <(const Vec4 &op1, const Vec4 &op2);
    friend Vec4 operator * (const Vec4& op1, const float &op2);
    friend Vec4 operator * (const float &op1 , const Vec4& op2 );
    friend Vec4 operator * (const Vec4& op1, const Vec4& op2);
    friend Vec3 operator / (const Vec3& op1, const float &op2);
    friend Vec3 operator / (const float &op1 , const Vec3& op2 );
    friend Vec3 operator / (const Vec3& op1, const Vec3& op2);
    friend Vec4 operator - (const Vec4 &op1, const Vec4 &op2);
    friend Vec4 operator + (const Vec4 &op1, const Vec4 &op2);
};

ostream & operator <<(ostream & os, const Vec4 & v);

extern Vec3 g_Up;
extern Vec3 g_Right;
extern Vec3 g_Forward;
extern Vec4 g_Up4;
extern Vec4 g_Right4;
extern Vec4 g_Forward4;

class Quaternion
{
    glm::quat m_quat;
public:
    static const Quaternion g_Identity;
    Quaternion(){}
    Quaternion(glm::quat quat){m_quat = quat;}
    Quaternion(const Vec3 &vec)
    {
        Vec3 v = vec.degreesToRadians();
        m_quat = glm::quat(v.getVec3());
    }
    Vec3 eulerAngles() const
    {
        return Vec3(glm::degrees(glm::eulerAngles(m_quat)));
    }
    glm::quat getQuat() const { return m_quat;}
    Mat4x4 toMat4x4() const;
    //Modifiers
    void normalize() { m_quat = glm::normalize(m_quat);}
    void slerp(const Quaternion &begin, const Quaternion &end, float cooef)
    {
        //performs spherical linear interpolation between begin & end
        //Note : set cooef between 0,0f - 1.0f
        m_quat = glm::mix(begin.getQuat(), end.getQuat(), cooef);
    }
    //Accessors
    void getAxisAngle(Vec3 &axis, float &angle) const
    {
        angle = glm::angle(m_quat);
        axis = Vec3(glm::axis(m_quat));
    }
    //Initializers
    void build(const class Mat4x4 &mat);
    void buildRotYawPitchRoll(const float yawRadians,const float pitchRadians,const float rollRadians )
    {
        m_quat = glm::toQuat(glm::yawPitchRoll(yawRadians,pitchRadians,rollRadians));
    }
    void buildAxisAngle(const Vec3 &axis, const float radians)
    {
        m_quat = glm::angleAxis(radians,axis.getVec3());
    }
    inline Quaternion operator *(float delta)
    {
        return Quaternion(m_quat * delta);
    }
};

inline Quaternion operator *(float delta, const Quaternion &a)
{
    return Quaternion(a.getQuat() * delta);
}

inline Quaternion operator *(const Quaternion &a, const Quaternion &b)
{
    //for rotations , this is exactly like concatenating
    //matrices - the new quat represents rot A followed by rot B
    return Quaternion(a.getQuat() * b.getQuat());
}



class Mat4x4
{
    glm::mat4x4 m_mat4x4;
public:
    Mat4x4(){m_mat4x4 = glm::mat4x4(1.0);}
    Mat4x4(const glm::mat4x4 &mat);
    static const Mat4x4 g_Identity;
    //Modifiers
    inline void setPosition(Vec3 const &pos)
    {
        m_mat4x4[3] = glm::vec4(pos.getVec3(),1.0f);
    }
    inline void setPosition(Vec4 const &pos)
    {
        m_mat4x4[3] = pos.getVec4();
    }
    inline void setScale(Vec3 const &scale)
    {
        m_mat4x4[1][1] = scale.getVec3().x;
        m_mat4x4[2][2] = scale.getVec3().y;
        m_mat4x4[3][3] = scale.getVec3().z;
    }
    //Accessors and Calculation Methods
    inline glm::mat4x4 getMat4x4() const
    {
        return m_mat4x4;
    }
    inline Vec3 getPosition() const
    {
        return Vec3(m_mat4x4[3][0],m_mat4x4[3][1],m_mat4x4[3][2]);
    }
    inline Vec3 getDirection() const
    {
        Mat4x4 justRot = *this;
        justRot.setPosition(Vec3(0.f,0.f,0.f));
        Vec3 forward = justRot.xForm(g_Forward);
        return forward;
    }
    inline Vec3 getDirection(const Vec3 &pos) const
    {
        Mat4x4 justRot = *this;
        justRot.setPosition(pos);
        Vec3 forward = justRot.xForm(g_Forward);
        return forward;
    }
    inline Vec3 getUp() const
    {
        Mat4x4 justRot = *this;
        justRot.setPosition(Vec3(0.f,0.f,0.f));
        Vec3 up = justRot.xForm(g_Up);
        return up;
    }
    inline Vec3 getRight() const
    {
        Mat4x4 justRot = *this;
        justRot.setPosition(Vec3(0.f,0.f,0.f));
        Vec3 right = justRot.xForm(g_Right);
        return right;
    }
    inline Vec3 getYawPitchRoll() const
    {
        float yaw, pitch, roll;
        pitch = asin(-32);
        double threshold = 0.001; // Hardcoded constant - burn him, he's a witch
        double test = cos(pitch);

        if(test > threshold)
        {
            roll = atan2(12, 22);
            yaw = atan2(31, 33);
        }
        else
        {
            roll = atan2(-21, 11);
            yaw = 0.0;
        }
        return (Vec3(yaw, pitch, roll));
    }
    inline Vec3 getScale() const
    {
        return Vec3(m_mat4x4[0][0],m_mat4x4[1][1],m_mat4x4[2][2]);
    }
    inline Vec4 xForm(Vec4 &v) const
    {
        glm::vec4 transformed = m_mat4x4 * v.getVec4();
        return Vec4(transformed);
    }
    inline Vec3 xForm(Vec3 &v) const
    {
        glm::vec4 transformed = m_mat4x4 * v.getVec4();
        return Vec3(transformed);
    }
    inline Mat4x4 inverse() const
    {
        glm::mat4x4 out = glm::inverse(m_mat4x4);
        return Mat4x4(out);
    }
    inline Mat4x4 transpose() const
    {
        glm::mat4x4 out = glm::transpose(m_mat4x4);
        return Mat4x4(out);
    }
    //Initialization methods
    inline void buildTranslation(const Vec3 &pos)
    {
        *this = Mat4x4::g_Identity;
        m_mat4x4[3] = glm::vec4(pos.getVec3(),1.0f);
    }
    inline void buildTranslation(const float x,const float y,const float z)
    {
        *this = Mat4x4::g_Identity;
        m_mat4x4[3] = glm::vec4(x,y,z,1.0f);
    }
    inline void buildRotationX(const float radians)
    {
        glm::rotate(m_mat4x4, glm::radians(radians), glm::vec3(1,0,0));
    }
    inline void buildRotationY(const float radians)
    {
        m_mat4x4=glm::rotate(glm::mat4x4(1.0), glm::radians(radians), glm::vec3(0,1,0));
    }
    inline void buildRotationZ(const float radians)
    {
        glm::rotate(m_mat4x4, glm::radians(radians), glm::vec3(0,0,1));
    }
    inline void buildYawPitchRoll(const float yawRadians,const float pitchRadians,const float rollRadians)
    {
        m_mat4x4 = glm::yawPitchRoll(yawRadians,pitchRadians,rollRadians);
    }
    inline void buildYawPitchRoll(const Vec3& vec3)
    {
        Vec3 temp = vec3.degreesToRadians();
        m_mat4x4 = glm::yawPitchRoll(temp.m_vec3.x,temp.m_vec3.y,temp.m_vec3.z);
    }
    inline void buildRotationQuat(const Quaternion &q)
    {
        m_mat4x4 = glm::toMat4(q.getQuat());
    }
    inline void buildRotationLookAt(const Vec3 &eye, const Vec3& at, const Vec3 &up)
    {
        m_mat4x4 = glm::lookAtRH(eye.getVec3(),at.getVec3(),up.getVec3());
    }
    inline void buildScale(const float x,const float y,const float z)
    {
        *this = Mat4x4::g_Identity;
        m_mat4x4[1][1] = x;
        m_mat4x4[2][2] = y;
        m_mat4x4[3][3] = z;
    }

    friend Mat4x4 operator *(const Mat4x4 &a, const Mat4x4 &b);
};

ostream & operator <<(ostream & os, const Mat4x4 & m);

class Plane {
public:
    Vec3 m_normal;
    float m_d;

    void setNormal(const Vec3 &p_normal);
    inline Vec3 getNormal() const { return m_normal; } ///Point is coplanar, CMP_EPSILON for precision

    void normalize();
    Plane normalized() const;

    inline void init(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2);
    bool inside(const Vec3 &point) const;
    bool inside(const Vec3 &point, float radius) const;
    /* Plane-Point operations */

    inline Vec3 center() const { return m_normal * m_d; }
    Vec3 get_any_point() const;
    Vec3 get_any_perpendicular_normal() const;

    inline bool is_point_over(const Vec3 &p_point) const; ///< Point is over plane
    inline float distance_to(const Vec3 &p_point) const
    { return glm::dot(m_normal.getVec3(),p_point.getVec3()) - m_d;}
    inline bool has_point(const Vec3 &p_point, float _epsilon = CMP_EPSILON) const;

    /* intersections */

    bool intersect_3(const Plane &p_plane1, const Plane &p_plane2, Vec3 *r_result = nullptr) const;
    bool intersects_ray(const Vec3 &p_from, const Vec3 &p_dir, Vec3 *p_intersection) const;
    bool intersects_segment(const Vec3 &p_begin, const Vec3 &p_end, Vec3 *p_intersection) const;

    inline Vec3 project(const Vec3 &p_point) const
    {
        return p_point - m_normal* distance_to(p_point);
    }

    /* misc */

    Plane operator-() const { return Plane(-m_normal, -m_d); }
    bool is_almost_like(const Plane &p_plane) const;

    inline bool operator==(const Plane &p_plane) const;
    inline bool operator!=(const Plane &p_plane) const;
    //operator std::string() const;

    inline Plane() { m_d = 0; }
    inline Plane(float p_a, float p_b, float p_c, float p_d) :
            m_normal(p_a, p_b, p_c),
            m_d(p_d){}

    inline Plane(const Vec3 &p_normal, float p_d) :
        m_normal(p_normal),
        m_d(p_d)
    {
    }
    inline Plane(const Vec3 &p_point, const Vec3 &p_normal);
    inline Plane(const Vec3 &p_point1, const Vec3 &p_point2, const Vec3 &p_point3, ClockDirection p_dir = CLOCKWISE);
};

class Frustum
{
public:
    enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes};
    Plane m_Planes[NumPlanes]; //planes of the frustum in camera space
    Vec3 m_NearClip[4];        //verts of the near clip plane in camera space
    Vec3 m_FarClip[4];         //verts of the far clip plane in camera space
    float m_Fov;               //field of view in radians
    float m_Aspect;            //aspect ratio - width divided by height
    float m_Near;              //near clipping distance
    float m_Far;               //far clipping distance
public:
    Frustum();
    void init(const float fov, const float aspect, const float nearClip, const float farClip);
    bool inside(const Vec3 &point) const;
    bool inside(const Vec3 &point, const float radius) const;
    const Plane &get(Side side){return m_Planes[side];}
    void setFov(float fov){m_Fov=fov;init(m_Fov,m_Aspect,m_Near,m_Far);}
    void setAspect(float aspect){m_Aspect=aspect;init(m_Fov,m_Aspect,m_Near,m_Far);}
    void setNear(float nearClip){m_Near=nearClip;init(m_Fov,m_Aspect,m_Near,m_Far);}
    void setFar(float farClip){m_Far=farClip;init(m_Fov,m_Aspect,m_Near,m_Far);}
    void render();
};


inline Vec3 CalcVelocity(Vec3 const &pos0, Vec3 const &pos1, float time)
{
    // CalcVelocity - Chapter 15, page 526
    return (pos1 - pos0) / time;
}

inline Vec3 CalcAcceleration(Vec3 const &vel0, Vec3 const &vel1, float time)
{
    // CalcVelocity - Chapter 15, page 526
    return (vel1 - vel0) / time;
}

inline void HandleAccel(Vec3 &pos, Vec3 &vel, Vec3 &accel, float time)
{
    // CalcVelocity - Chapter 15, page 526
    vel += accel * time;
    pos += vel * time;
}

// converts Barycentric coordinates to world coordinates
// inputs are the 3 verts of the triangle, and the u,v barycentric coordinates
extern Vec3 BarycentricToVec3(Vec3 v0, Vec3 v1, Vec3 v2, float u, float v);

extern bool IntersectTriangle( const Vec3& orig, const Vec3& dir,
                        Vec3& v0, Vec3& v1, Vec3& v2,
                        float* t, float* u, float* v );

#endif // GEOMETRY_H
