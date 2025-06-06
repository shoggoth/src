//
//  dvector.h
//  spring_mass
//
//  Created by Richard Henry on 13/02/2023.
//

/*
dvector.h - Portable, single-file, 2D/3D vector/quaternion/matrix math library, originally based on ccVector

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring
rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software.
If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

/*
dvector supports the following three configurations:
#define DVECTOR_EXTERN
    Default, should be used when using dvector in multiple compilation units within the same project.
#define DVECTOR_IMPLEMENTATION
    Must be defined in exactly one source file within a project for dvector to be found by the linker.
#define DVECTOR_STATIC
    Defines all dvector functions as static, useful if dvector is only used in a single compilation unit.

dvector supports the following additional options:
#define DVECTOR_DOUBLE
    Configures dvector to use double instead of the default float. Cannot use both float and double versions in the same compilation unit.
#define DVECTOR_ISOC
    Changes the way dvector types are defined for compatibility with ISO C99. Only affects convenience without limiting functionality.

dvector types:
    Supports vec2, vec3, vec4, quat, mat2, mat3, and mat4 types with various property aliases for flexible use and concise code.
    Each of the above types comes with a TYPE_ZERO and TYPE_IDEN (for quat and matrix types) constant which translates to a literal.
    Function-like macros of the form TYPE(...) also exist and are meant to provide a concise alternative to typing out literals.
    The auxiliary frst type supports frustum generation from a projview matrix and simple frustum culling of bounding volumes.

dvector functions:
    Provided functions should be reasonably self-explanatory, and use of macros was deliberately kept low for better readability.
    All equality functions use direct comparison (no epsilon), therefore floating point errors may break equality for some values.
    All quat functions should return normalized quats, occasional normalization is recommended due to float error accumulation.
    All angles are in radians. Frustum culling of volumes returns 1 for volumes inside the frustum, 0 for volumes outside it.
    All coordinate systems are right-handed unless otherwise noted. Projection functions assume OpenGL-style screen space.
*/

//header section
#ifndef DVECTOR_H
#define DVECTOR_H

//process configuration
#ifdef DVECTOR_STATIC
    #define DVECTOR_IMPLEMENTATION
    #define DVDEF static
#else //DVECTOR_EXTERN
    #define DVDEF extern
#endif

/*
Use the below macros and constants for function parameters and variable
definitions in local scope. To initialize globals, use naked literals.
Example:
vec3 default_position = {0, 0, 0};
void foo(){
    bar(VEC3_ZERO);
}
*/

//macros
#ifdef DVECTOR_ISOC
    #define VEC2(X, Y) (vec2){X, Y}
    #define VEC3(X, Y, Z) (vec3){X, Y, Z}
    #define VEC4(X, Y, Z, W) (vec4){X, Y, Z, W}
    #define QUAT(X, Y, Z, W) (quat){X, Y, Z, W}
#else
    #define VEC2(X, Y) (vec2){{X, Y}}
    #define VEC3(X, Y, Z) (vec3){{X, Y, Z}}
    #define VEC4(X, Y, Z, W) (vec4){{X, Y, Z, W}}
    #define QUAT(X, Y, Z, W) (quat){{X, Y, Z, W}}
#endif
#define MAT2(M00, M01, M10, M11) (mat2){{{M00, M01}, {M10, M11}}}
#define MAT3(M00, M01, M02, M10, M11, M12, M20, M21, M22) (mat3){{{M00, M01, M02}, {M10, M11, M12}, {M20, M21, M22}}}
#define MAT4(M00, M01, M02, M03, M10, M11, M12, M13, M20, M21, M22, M23, M30, M31, M32, M33) \
    (mat4){{{M00, M01, M02, M03}, {M10, M11, M12, M13}, {M20, M21, M22, M23}, {M30, M31, M32, M33}}}

//constants
#define VEC2_ZERO VEC2(0, 0)
#define VEC3_ZERO VEC3(0, 0, 0)
#define VEC4_ZERO VEC4(0, 0, 0, 0)
#define QUAT_IDEN QUAT(0, 0, 0, 1)
#define MAT2_ZERO MAT2(0, 0,  0, 0)
#define MAT2_IDEN MAT2(1, 0,  0, 1)
#define MAT3_ZERO MAT3(0, 0, 0,  0, 0, 0,  0, 0, 0)
#define MAT3_IDEN MAT3(1, 0, 0,  0, 1, 0,  0, 0, 1)
#define MAT4_ZERO MAT4(0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0)
#define MAT4_IDEN MAT4(1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1)

//math configuration
#include <math.h>
#ifdef DVECTOR_DOUBLE
    #define DVTYPE double
    #define DVCOS cos
    #define DVSIN sin
    #define DVTAN tan
    #define DVACOS acos
    #define DVHYPOT hypot
#else
    #define DVTYPE float
    #define DVCOS cosf
    #define DVSIN sinf
    #define DVTAN tanf
    #define DVACOS acosf
    #define DVHYPOT hypotf
#endif

//types
#ifdef DVECTOR_ISOC
    typedef struct vec2 {
        DVTYPE x, y;
    } vec2;
    typedef struct vec3 {
        DVTYPE x, y, z;
    } vec3;
    typedef struct vec4 {
        DVTYPE x, y, z, w;
    } vec4, quat;
    typedef union mat2 {
        DVTYPE m[2][2];
        vec2 col[2];
    } mat2;
    typedef union mat3 {
        DVTYPE m[3][3];
        vec3 col[3];
    } mat3;
    typedef union mat4 {
        DVTYPE m[4][4];
        vec4 col[4];
    } mat4;
    typedef union frst {
        DVTYPE f[24];
        vec4 pln[6];
    } frst;
#else
    typedef union vec2 {
        DVTYPE v[2];
        struct {DVTYPE x, y;};
    } vec2;
    typedef union vec3 {
        DVTYPE v[3];
        struct {DVTYPE x, y, z;};
        struct {vec2 xy; DVTYPE _z;};
        struct {DVTYPE _x; vec2 yz;};
    } vec3;
    typedef union vec4 {
        DVTYPE v[4];
        struct {DVTYPE x, y, z, w;};
        struct {vec3 xyz; DVTYPE _w;};
        struct {DVTYPE _x; vec3 yzw;};
        struct {vec2 xy; vec2 zw;};
        struct {DVTYPE __x; vec2 yz; DVTYPE __w;};
    } vec4, quat;
    typedef union mat2 {
        DVTYPE m[2][2];
        struct {DVTYPE m00, m01, m10, m11;};
        vec2 col[2];
        struct {vec2 col0, col1;};
    } mat2;
    typedef union mat3 {
        DVTYPE m[3][3];
        struct {DVTYPE m00, m01, m02, m10, m11, m12, m20, m21, m22;};
        vec3 col[3];
        struct {vec3 col0, col1, col2;};
    } mat3;
    typedef union mat4 {
        DVTYPE m[4][4];
        struct {DVTYPE m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33;};
        vec4 col[4];
        struct {vec4 col0, col1, col2, col3;};
    } mat4;
    typedef union frst {
        DVTYPE f[24];
        vec4 pln[6];
        struct {vec4 left, right, top, bottom, near, far;};
    } frst;
#endif

//vec2 function declarations
DVDEF DVTYPE vec2Length(vec2);
DVDEF DVTYPE vec2DotProduct(vec2, vec2);
DVDEF vec2 vec2Negate(vec2);
DVDEF vec2 vec2Normalize(vec2);
DVDEF vec2 vec2Multiply(vec2, DVTYPE);
DVDEF vec2 vec2Divide(vec2, DVTYPE);
DVDEF vec2 vec2Rotate(vec2, DVTYPE);
DVDEF vec2 vec2RotateXZ(vec2, DVTYPE);
DVDEF vec2 vec2Add(vec2, vec2);
DVDEF vec2 vec2Subtract(vec2, vec2);
DVDEF vec2 vec2Reflect(vec2, vec2);
DVDEF vec2 vec2Mix(vec2, vec2, DVTYPE);
DVDEF int vec2Equal(vec2, vec2);

//vec3 function declarations
DVDEF DVTYPE vec3Length(vec3);
DVDEF DVTYPE vec3DotProduct(vec3, vec3);
DVDEF vec2 vec3GetXZ(vec3);
DVDEF vec3 vec3Negate(vec3);
DVDEF vec3 vec3Normalize(vec3);
DVDEF vec3 vec3Multiply(vec3, DVTYPE);
DVDEF vec3 vec3Divide(vec3, DVTYPE);
DVDEF vec3 vec3Add(vec3, vec3);
DVDEF vec3 vec3Subtract(vec3, vec3);
DVDEF vec3 vec3Reflect(vec3, vec3);
DVDEF vec3 vec3CrossProduct(vec3, vec3);
DVDEF vec3 vec3Mix(vec3, vec3, DVTYPE);
DVDEF int vec3Equal(vec3, vec3);

//vec4 function declarations
DVDEF vec3 vec4GetXYZ(vec4);
DVDEF DVTYPE vec4Length(vec4);
DVDEF DVTYPE vec4DotProduct(vec4, vec4);
DVDEF vec4 vec4Negate(vec4);
DVDEF vec4 vec4Normalize(vec4);
DVDEF vec4 vec4Multiply(vec4, DVTYPE);
DVDEF vec4 vec4Divide(vec4, DVTYPE);
DVDEF vec4 vec4Add(vec4, vec4);
DVDEF vec4 vec4Subtract(vec4, vec4);
DVDEF vec4 vec4Reflect(vec4, vec4);
DVDEF vec4 vec4Mix(vec4, vec4, DVTYPE);
DVDEF int vec4Equal(vec4, vec4);

//quat function declarations
DVDEF vec3 quatGetXYZ(quat);
DVDEF vec3 quatMultiplyVector(quat, vec3);
DVDEF quat quatAxisAngle(vec3, DVTYPE);
DVDEF quat quatEulerXYZ(DVTYPE, DVTYPE, DVTYPE);
DVDEF quat quatEulerXZY(DVTYPE, DVTYPE, DVTYPE);
DVDEF quat quatEulerYXZ(DVTYPE, DVTYPE, DVTYPE);
DVDEF quat quatEulerYZX(DVTYPE, DVTYPE, DVTYPE);
DVDEF quat quatEulerZXY(DVTYPE, DVTYPE, DVTYPE);
DVDEF quat quatEulerZYX(DVTYPE, DVTYPE, DVTYPE);
DVDEF quat quatNormalize(quat);
DVDEF quat quatConjugate(quat);
DVDEF quat quatLerp(quat, quat, DVTYPE);
DVDEF quat quatSlerp(quat, quat, DVTYPE);
DVDEF quat quatMultiply(quat, quat);
DVDEF int quatEqual(quat, quat);

//mat2 function declarations
DVDEF vec2 mat2MultiplyVector(mat2, vec2);
DVDEF mat2 mat2Transpose(mat2);
DVDEF mat2 mat2Inverse(mat2);
DVDEF mat2 mat2MultiplyScalar(mat2, DVTYPE);
DVDEF mat2 mat2MultiplyMatrix(mat2, mat2);
DVDEF mat2 mat2Add(mat2, mat2);
DVDEF mat2 mat2Subtract(mat2, mat2);
DVDEF int mat2Equal(mat2, mat2);

//mat3 function declarations
DVDEF vec3 mat3MultiplyVector(mat3, vec3);
DVDEF mat3 mat3SetRotation(DVTYPE);
DVDEF mat3 mat3SetScale(DVTYPE);
DVDEF mat3 mat3SetScaleXY(vec2);
DVDEF mat3 mat3SetTranslation(vec2);
DVDEF mat3 mat3Transpose(mat3);
DVDEF mat3 mat3Inverse(mat3);
DVDEF mat3 mat3MultiplyScalar(mat3, DVTYPE);
DVDEF mat3 mat3Rotate(mat3, DVTYPE);
DVDEF mat3 mat3Scale(mat3, DVTYPE);
DVDEF mat3 mat3ScaleXY(mat3, vec2);
DVDEF mat3 mat3Translate(mat3, vec2);
DVDEF mat3 mat3MultiplyMatrix(mat3, mat3);
DVDEF mat3 mat3Add(mat3, mat3);
DVDEF mat3 mat3Subtract(mat3, mat3);
DVDEF int mat3Equal(mat3, mat3);

//mat4 function declarations
DVDEF vec4 mat4MultiplyVector(mat4, vec4);
DVDEF mat4 mat4SetRotationX(DVTYPE);
DVDEF mat4 mat4SetRotationY(DVTYPE);
DVDEF mat4 mat4SetRotationZ(DVTYPE);
DVDEF mat4 mat4SetRotationQuaternion(quat);
DVDEF mat4 mat4SetScale(DVTYPE);
DVDEF mat4 mat4SetScaleXYZ(vec3);
DVDEF mat4 mat4SetTranslation(vec3);
DVDEF mat4 mat4LookAt(vec3, vec3, vec3);
DVDEF mat4 mat4Perspective(DVTYPE, DVTYPE, DVTYPE, DVTYPE);
DVDEF mat4 mat4Ortho(DVTYPE, DVTYPE, DVTYPE, DVTYPE, DVTYPE, DVTYPE);
DVDEF mat4 mat4Transpose(mat4);
DVDEF mat4 mat4Inverse(mat4);
DVDEF mat4 mat4MultiplyScalar(mat4, DVTYPE);
DVDEF mat4 mat4RotateX(mat4, DVTYPE);
DVDEF mat4 mat4RotateY(mat4, DVTYPE);
DVDEF mat4 mat4RotateZ(mat4, DVTYPE);
DVDEF mat4 mat4RotateQuaternion(mat4, quat);
DVDEF mat4 mat4Scale(mat4, DVTYPE);
DVDEF mat4 mat4ScaleXYZ(mat4, vec3);
DVDEF mat4 mat4Translate(mat4, vec3);
DVDEF mat4 mat4MultiplyMatrix(mat4, mat4);
DVDEF mat4 mat4Add(mat4, mat4);
DVDEF mat4 mat4Subtract(mat4, mat4);
DVDEF int mat4Equal(mat4, mat4);

//frst function declarations
DVDEF frst frstFromMatrix(mat4);
DVDEF int frstCullSphere(frst, vec3, DVTYPE);
DVDEF int frstCullAABB(frst, vec3, vec3);

#endif //DVECTOR_H

//implementation section
#ifdef DVECTOR_IMPLEMENTATION
#undef DVECTOR_IMPLEMENTATION

//vec2 functions
DVDEF DVTYPE vec2Length (vec2 v) {
    return DVHYPOT(v.x, v.y);
}
DVDEF DVTYPE vec2DotProduct (vec2 v1, vec2 v2) {
    return v1.x*v2.x + v1.y*v2.y;
}
DVDEF vec2 vec2Negate (vec2 v) {
    return VEC2(-v.x, -v.y);
}
DVDEF vec2 vec2Normalize (vec2 v) {
    return vec2Divide(v, vec2Length(v));
}
DVDEF vec2 vec2Multiply (vec2 v, DVTYPE s) {
    return VEC2(v.x*s, v.y*s);
}
DVDEF vec2 vec2Divide (vec2 v, DVTYPE s) {
    return VEC2(v.x/s, v.y/s);
}
DVDEF vec2 vec2Rotate (vec2 v, DVTYPE r) {
    DVTYPE c = DVCOS(r), s = DVSIN(r);
    return VEC2(v.x*c-v.y*s, v.y*c+v.x*s);
}
DVDEF vec2 vec2RotateXZ (vec2 v, DVTYPE r) {
    DVTYPE c = DVCOS(r), s = DVSIN(r);
    return VEC2(v.x*c+v.y*s, v.y*c-v.x*s);
}
DVDEF vec2 vec2Add (vec2 v1, vec2 v2) {
    return VEC2(v1.x+v2.x, v1.y+v2.y);
}
DVDEF vec2 vec2Subtract (vec2 v1, vec2 v2) {
    return VEC2(v1.x-v2.x, v1.y-v2.y);
}
DVDEF vec2 vec2Reflect (vec2 v1, vec2 v2) {
    return vec2Subtract(v1, vec2Multiply(v2, 2*vec2DotProduct(v1, v2)));
}
DVDEF vec2 vec2Mix (vec2 v1, vec2 v2, DVTYPE s) {
    return VEC2(v1.x+(v2.x-v1.x)*s, v1.y+(v2.y-v1.y)*s);
}
DVDEF int vec2Equal (vec2 v1, vec2 v2) {
    return (v1.x == v2.x)&&(v1.y == v2.y);
}

//vec3 functions
DVDEF DVTYPE vec3Length (vec3 v) {
    return DVHYPOT(DVHYPOT(v.x, v.y), v.z);
}
DVDEF DVTYPE vec3DotProduct (vec3 v1, vec3 v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
DVDEF vec2 vec3GetXZ (vec3 v) {
    return VEC2(v.x, v.z);
}
DVDEF vec3 vec3Negate (vec3 v) {
    return VEC3(-v.x, -v.y, -v.z);
}
DVDEF vec3 vec3Normalize (vec3 v) {
    return vec3Divide(v, vec3Length(v));
}
DVDEF vec3 vec3Multiply (vec3 v, DVTYPE s) {
    return VEC3(v.x*s, v.y*s, v.z*s);
}
DVDEF vec3 vec3Divide (vec3 v, DVTYPE s) {
    return VEC3(v.x/s, v.y/s, v.z/s);
}
DVDEF vec3 vec3Add (vec3 v1, vec3 v2) {
    return VEC3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}
DVDEF vec3 vec3Subtract (vec3 v1, vec3 v2) {
    return VEC3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}
DVDEF vec3 vec3Reflect (vec3 v1, vec3 v2) {
    return vec3Subtract(v1, vec3Multiply(v2, 2*vec3DotProduct(v1, v2)));
}
DVDEF vec3 vec3CrossProduct (vec3 v1, vec3 v2) {
    return VEC3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}
DVDEF vec3 vec3Mix (vec3 v1, vec3 v2, DVTYPE s) {
    return VEC3(v1.x+(v2.x-v1.x)*s, v1.y+(v2.y-v1.y)*s, v1.z+(v2.z-v1.z)*s);
}
DVDEF int vec3Equal (vec3 v1, vec3 v2) {
    return (v1.x == v2.x)&&(v1.y == v2.y)&&(v1.z == v2.z);
}


//vec4 functions
DVDEF vec3 vec4GetXYZ (vec4 v) {
    return VEC3(v.x, v.y, v.z);
}
DVDEF DVTYPE vec4Length (vec4 v) {
    return DVHYPOT(DVHYPOT(v.x, v.y), DVHYPOT(v.z, v.w));
}
DVDEF DVTYPE vec4DotProduct (vec4 v1, vec4 v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}
DVDEF vec4 vec4Negate (vec4 v) {
    return VEC4(-v.x, -v.y, -v.z, -v.w);
}
DVDEF vec4 vec4Normalize (vec4 v) {
    return vec4Divide(v, vec4Length(v));
}
DVDEF vec4 vec4Multiply (vec4 v, DVTYPE s) {
    return VEC4(v.x*s, v.y*s, v.z*s, v.w*s);
}
DVDEF vec4 vec4Divide (vec4 v, DVTYPE s) {
    return VEC4(v.x/s, v.y/s, v.z/s, v.w/s);
}
DVDEF vec4 vec4Add (vec4 v1, vec4 v2) {
    return VEC4(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w);
}
DVDEF vec4 vec4Subtract (vec4 v1, vec4 v2) {
    return VEC4(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w);
}
DVDEF vec4 vec4Reflect (vec4 v1, vec4 v2) {
    return vec4Subtract(v1, vec4Multiply(v2, 2*vec4DotProduct(v1, v2)));
}
DVDEF vec4 vec4Mix (vec4 v1, vec4 v2, DVTYPE s) {
    return VEC4(v1.x+(v2.x-v1.x)*s, v1.y+(v2.y-v1.y)*s, v1.z+(v2.z-v1.z)*s, v1.w+(v2.w-v1.w)*s);
}
DVDEF int vec4Equal (vec4 v1, vec4 v2) {
    return (v1.x == v2.x)&&(v1.y == v2.y)&&(v1.z == v2.z)&&(v1.w == v2.w);
}

//quat functions
DVDEF vec3 quatGetXYZ (quat q) {
    return VEC3(q.x, q.y, q.z);
}
DVDEF vec3 quatMultiplyVector (quat q, vec3 v) {
    vec3 q_xyz = VEC3(q.x, q.y, q.z);
    vec3 t = vec3Multiply(vec3CrossProduct(q_xyz, v), 2);
    return vec3Add(vec3Add(v, vec3Multiply(t, q.w)), vec3CrossProduct(q_xyz, t));
}
DVDEF quat quatAxisAngle (vec3 a, DVTYPE r) {
    DVTYPE s = DVSIN(r/2);
    return QUAT(a.x*s, a.y*s, a.z*s, DVCOS(r/2));
}
DVDEF quat quatEulerXYZ (DVTYPE a, DVTYPE b, DVTYPE c) {
    return quatMultiply(quatAxisAngle(VEC3(0, 0, 1), c), quatMultiply(quatAxisAngle(VEC3(0, 1, 0), b), quatAxisAngle(VEC3(1, 0, 0), a)));
}
DVDEF quat quatEulerXZY (DVTYPE a, DVTYPE b, DVTYPE c) {
    return quatMultiply(quatAxisAngle(VEC3(0, 1, 0), c), quatMultiply(quatAxisAngle(VEC3(0, 0, 1), b), quatAxisAngle(VEC3(1, 0, 0), a)));
}
DVDEF quat quatEulerYXZ (DVTYPE a, DVTYPE b, DVTYPE c) {
    return quatMultiply(quatAxisAngle(VEC3(0, 0, 1), c), quatMultiply(quatAxisAngle(VEC3(1, 0, 0), b), quatAxisAngle(VEC3(0, 1, 0), a)));
}
DVDEF quat quatEulerYZX (DVTYPE a, DVTYPE b, DVTYPE c) {
    return quatMultiply(quatAxisAngle(VEC3(1, 0, 0), c), quatMultiply(quatAxisAngle(VEC3(0, 0, 1), b), quatAxisAngle(VEC3(0, 1, 0), a)));
}
DVDEF quat quatEulerZXY (DVTYPE a, DVTYPE b, DVTYPE c) {
    return quatMultiply(quatAxisAngle(VEC3(0, 1, 0), c), quatMultiply(quatAxisAngle(VEC3(1, 0, 0), b), quatAxisAngle(VEC3(0, 0, 1), a)));
}
DVDEF quat quatEulerZYX (DVTYPE a, DVTYPE b, DVTYPE c) {
    return quatMultiply(quatAxisAngle(VEC3(1, 0, 0), c), quatMultiply(quatAxisAngle(VEC3(0, 1, 0), b), quatAxisAngle(VEC3(0, 0, 1), a)));
}
DVDEF quat quatNormalize (quat q) {
    return vec4Normalize(q);
}
DVDEF quat quatConjugate (quat q) {
    return QUAT(-q.x, -q.y, -q.z, q.w);
}
DVDEF quat quatLerp (quat q1, quat q2, DVTYPE s) {
    return quatNormalize(QUAT((1-s)*q1.x + s*q2.x, (1-s)*q1.y + s*q2.y, (1-s)*q1.z + s*q2.z, (1-s)*q1.w + s*q2.w));
}
DVDEF quat quatSlerp (quat q1, quat q2, DVTYPE s) {
    DVTYPE th = DVACOS(q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w), sn = DVSIN(th), wa = DVSIN((1-s)*th)/sn, wb = DVSIN(s*th)/sn;
    return quatNormalize(QUAT(wa*q1.x + wb*q2.x, wa*q1.y + wb*q2.y, wa*q1.z + wb*q2.z, wa*q1.w + wb*q2.w));
}
DVDEF quat quatMultiply (quat q1, quat q2) {
    return QUAT(q1.x*q2.w + q1.y*q2.z - q1.z*q2.y + q1.w*q2.x, -q1.x*q2.z + q1.y*q2.w + q1.z*q2.x + q1.w*q2.y,
        q1.x*q2.y - q1.y*q2.x + q1.z*q2.w + q1.w*q2.z, -q1.x*q2.x - q1.y*q2.y - q1.z*q2.z + q1.w*q2.w);
}
DVDEF int quatEqual (quat q1, quat q2) {
    return (q1.x == q2.x)&&(q1.y == q2.y)&&(q1.z == q2.z)&&(q1.w == q2.w);
}

//mat2 functions
DVDEF vec2 mat2MultiplyVector (mat2 m, vec2 v) {
    return VEC2(m.m[0][0]*v.x + m.m[1][0]*v.y, m.m[0][1]*v.x + m.m[1][1]*v.y);
}
DVDEF mat2 mat2Transpose (mat2 m) {
    return MAT2(m.m[0][0], m.m[1][0], m.m[0][1], m.m[1][1]);
}
DVDEF mat2 mat2Inverse (mat2 m) {
    return mat2MultiplyScalar(MAT2(m.m[1][1], -m.m[0][1], -m.m[1][0], m.m[0][0]), 1/(m.m[0][0]*m.m[1][1]-m.m[1][0]*m.m[0][1]));
}
DVDEF mat2 mat2MultiplyScalar (mat2 m, DVTYPE s) {
    return MAT2(m.m[0][0]*s, m.m[0][1]*s, m.m[1][0]*s, m.m[1][1]*s);
}
DVDEF mat2 mat2MultiplyMatrix (mat2 m1, mat2 m2) {
    mat2 m = MAT2_IDEN;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m.m[i][j] = m1.m[i][0]*m2.m[0][j] + m1.m[i][1]*m2.m[1][j];
    return m;
}
DVDEF mat2 mat2Add (mat2 m1, mat2 m2) {
    return MAT2(m1.m[0][0]+m2.m[0][0], m1.m[0][1]+m2.m[0][1], m1.m[1][0]+m2.m[1][0], m1.m[1][1]+m2.m[1][1]);
}
DVDEF mat2 mat2Subtract (mat2 m1, mat2 m2) {
    return MAT2(m1.m[0][0]-m2.m[0][0], m1.m[0][1]-m2.m[0][1], m1.m[1][0]-m2.m[1][0], m1.m[1][1]-m2.m[1][1]);
}
DVDEF int mat2Equal (mat2 m1, mat2 m2) {
    return (m1.m[0][0] == m2.m[0][0])&&(m1.m[0][1] == m2.m[0][1])&&(m1.m[1][0] == m2.m[1][0])&&(m1.m[1][1] == m2.m[1][1]);
}

//mat3 function declarations
DVDEF vec3 mat3MultiplyVector (mat3 m, vec3 v) {
    DVTYPE r[3];
    for (int i = 0; i < 3; i++)
        r[i] = m.m[0][i]*v.x + m.m[1][i]*v.y + m.m[2][i]*v.z;
    return VEC3(r[0], r[1], r[2]);
}
DVDEF mat3 mat3SetRotation (DVTYPE r) {
    DVTYPE c = DVCOS(r), s = DVSIN(r);
    return MAT3(c, s, 0, -s, c, 0, 0, 0, 1);
}
DVDEF mat3 mat3SetScale (DVTYPE s) {
    return MAT3(s, 0, 0, 0, s, 0, 0, 0, 1);
}
DVDEF mat3 mat3SetScaleXY (vec2 s) {
    return MAT3(s.x, 0, 0, 0, s.y, 0, 0, 0, 1);
}
DVDEF mat3 mat3SetTranslation (vec2 v) {
    return MAT3(1, 0, 0, 0, 1, 0, v.x, v.y, 1);
}
DVDEF mat3 mat3Transpose (mat3 m) {
    return MAT3(m.m[0][0], m.m[1][0], m.m[2][0], m.m[0][1], m.m[1][1], m.m[2][1], m.m[0][2], m.m[1][2], m.m[2][2]);
}
DVDEF mat3 mat3Inverse (mat3 m) {
    mat3 t = MAT3(m.m[1][1]*m.m[2][2] - m.m[2][1]*m.m[1][2], m.m[1][2]*m.m[2][0] - m.m[1][0]*m.m[2][2], m.m[1][0]*m.m[2][1] - m.m[1][1]*m.m[2][0],
        m.m[2][1]*m.m[0][2] - m.m[0][1]*m.m[2][2], m.m[0][0]*m.m[2][2] - m.m[0][2]*m.m[2][0], m.m[0][1]*m.m[2][0] - m.m[0][0]*m.m[2][1],
        m.m[0][1]*m.m[1][2] - m.m[1][1]*m.m[0][2], m.m[0][2]*m.m[1][0] - m.m[0][0]*m.m[1][2], m.m[0][0]*m.m[1][1] - m.m[0][1]*m.m[1][0]);
    return mat3MultiplyScalar(t, 1/(m.m[0][0]*t.m[0][0] + m.m[1][0]*t.m[1][0] + m.m[2][0]*t.m[2][0]));
}
DVDEF mat3 mat3MultiplyScalar (mat3 m, DVTYPE s) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m.m[i][j] *= s;
    return m;
}
DVDEF mat3 mat3Rotate (mat3 m, DVTYPE r) {
    return mat3MultiplyMatrix(m, mat3SetRotation(r));
}
DVDEF mat3 mat3Scale (mat3 m, DVTYPE s) {
    return mat3MultiplyMatrix(m, mat3SetScale(s));
}
DVDEF mat3 mat3ScaleXY (mat3 m, vec2 v) {
    return mat3MultiplyMatrix(m, mat3SetScaleXY(v));
}
DVDEF mat3 mat3Translate (mat3 m, vec2 v) {
    return mat3MultiplyMatrix(m, mat3SetTranslation(v));
}
DVDEF mat3 mat3MultiplyMatrix (mat3 m1, mat3 m2) {
    mat3 m = MAT3_IDEN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m.m[i][j] = m1.m[i][0]*m2.m[0][j] + m1.m[i][1]*m2.m[1][j] + m1.m[i][2]*m2.m[2][j];
    return m;
}
DVDEF mat3 mat3Add (mat3 m1, mat3 m2) {
    mat3 m = MAT3_IDEN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m.m[i][j] = m1.m[i][j] + m2.m[i][j];
    return m;
}
DVDEF mat3 mat3Subtract (mat3 m1, mat3 m2) {
    mat3 m = MAT3_IDEN;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m.m[i][j] = m1.m[i][j] - m2.m[i][j];
    return m;
}
DVDEF int mat3Equal (mat3 m1, mat3 m2) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (m1.m[i][j] != m2.m[i][j])
                return 0;
    return 1;
}

//mat4 function declarations
DVDEF vec4 mat4MultiplyVector (mat4 m, vec4 v) {
    DVTYPE r[4];
    for (int i = 0; i < 4; i++)
        r[i] = m.m[0][i]*v.x + m.m[1][i]*v.y + m.m[2][i]*v.z + m.m[3][i]*v.w;
    return VEC4(r[0], r[1], r[2], r[3]);
}
DVDEF mat4 mat4SetRotationX (DVTYPE r) {
    DVTYPE c = DVCOS(r), s = DVSIN(r);
    return MAT4(1, 0, 0, 0, 0, c, s, 0, 0, -s, c, 0, 0, 0, 0, 1);
}
DVDEF mat4 mat4SetRotationY (DVTYPE r) {
    DVTYPE c = DVCOS(r), s = DVSIN(r);
    return MAT4(c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, 0, 0, 0, 1);
}
DVDEF mat4 mat4SetRotationZ (DVTYPE r) {
    DVTYPE c = DVCOS(r), s = DVSIN(r);
    return MAT4(c, s, 0, 0, -s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}
DVDEF mat4 mat4SetRotationQuaternion (quat q) {
    DVTYPE xx = q.x*q.x, xy = q.x*q.y, xz = q.x*q.z, xw = q.x*q.w, yy = q.y*q.y, yz = q.y*q.z, yw = q.y*q.w, zz = q.z*q.z, zw = q.z*q.w;
    return MAT4(1-2*yy-2*zz, 2*xy+2*zw, 2*xz-2*yw, 0, 2*xy-2*zw, 1-2*xx-2*zz, 2*yz+2*xw, 0, 2*xz+2*yw, 2*yz-2*xw, 1-2*xx-2*yy, 0, 0, 0, 0, 1);
}
DVDEF mat4 mat4SetScale (DVTYPE s) {
    return MAT4(s, 0, 0, 0, 0, s, 0, 0, 0, 0, s, 0, 0, 0, 0, 1);
}
DVDEF mat4 mat4SetScaleXYZ (vec3 v) {
    return MAT4(v.x, 0, 0, 0, 0, v.y, 0, 0, 0, 0, v.z, 0, 0, 0, 0, 1);
}
DVDEF mat4 mat4SetTranslation (vec3 v) {
    return MAT4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, v.x, v.y, v.z, 1);
}
DVDEF mat4 mat4LookAt (vec3 from, vec3 to, vec3 up) {
    vec3 f = vec3Normalize(vec3Subtract(to, from)), s = vec3Normalize(vec3CrossProduct(f, up)), t = vec3CrossProduct(s, f);
    mat4 m = MAT4(s.x, t.x, -f.x, 0, s.y, t.y, -f.y, 0, s.z, t.z, -f.z, 0, 0, 0, 0, 1);
    for (int i = 0; i < 3; i++)
        m.m[3][i] = -vec3DotProduct(VEC3(m.m[0][i], m.m[1][i], m.m[2][i]), from);
    return m;
}
DVDEF mat4 mat4Perspective (DVTYPE vfov, DVTYPE aspect, DVTYPE ndist, DVTYPE fdist) {
    DVTYPE a = 1/DVTAN(vfov/2);
    return MAT4(a/aspect, 0, 0, 0, 0, a, 0, 0, 0, 0, -((fdist+ndist)/(fdist-ndist)), -1, 0, 0, -((2*fdist*ndist)/(fdist-ndist)), 0);
}
DVDEF mat4 mat4Ortho (DVTYPE left, DVTYPE right, DVTYPE bottom, DVTYPE top, DVTYPE ndist, DVTYPE fdist) {
    return MAT4(2/(right-left), 0, 0, 0, 0, 2/(top-bottom), 0, 0, 0, 0, -2/(fdist-ndist), 0,
        -(right+left)/(right-left), -(top+bottom)/(top-bottom), -(fdist+ndist)/(fdist-ndist), 1);
}
DVDEF mat4 mat4Transpose (mat4 m) {
    return MAT4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0], m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
        m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2], m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}
DVDEF mat4 mat4Inverse (mat4 m) {
    DVTYPE s[6] = {m.m[0][0]*m.m[1][1] - m.m[1][0]*m.m[0][1], m.m[0][0]*m.m[1][2] - m.m[1][0]*m.m[0][2], m.m[0][0]*m.m[1][3] - m.m[1][0]*m.m[0][3],
        m.m[0][1]*m.m[1][2] - m.m[1][1]*m.m[0][2], m.m[0][1]*m.m[1][3] - m.m[1][1]*m.m[0][3], m.m[0][2]*m.m[1][3] - m.m[1][2]*m.m[0][3]};
    DVTYPE c[6] = {m.m[2][0]*m.m[3][1] - m.m[3][0]*m.m[2][1], m.m[2][0]*m.m[3][2] - m.m[3][0]*m.m[2][2], m.m[2][0]*m.m[3][3] - m.m[3][0]*m.m[2][3],
        m.m[2][1]*m.m[3][2] - m.m[3][1]*m.m[2][2], m.m[2][1]*m.m[3][3] - m.m[3][1]*m.m[2][3], m.m[2][2]*m.m[3][3] - m.m[3][2]*m.m[2][3]};
    return mat4MultiplyScalar(MAT4(m.m[1][1]*c[5] - m.m[1][2]*c[4] + m.m[1][3]*c[3], m.m[0][2]*c[4] - m.m[0][1]*c[5] - m.m[0][3]*c[3],
        m.m[3][1]*s[5] - m.m[3][2]*s[4] + m.m[3][3]*s[3], m.m[2][2]*s[4] - m.m[2][1]*s[5] - m.m[2][3]*s[3], m.m[1][2]*c[2] - m.m[1][0]*c[5] - m.m[1][3]*c[1],
        m.m[0][0]*c[5] - m.m[0][2]*c[2] + m.m[0][3]*c[1], m.m[3][2]*s[2] - m.m[3][0]*s[5] - m.m[3][3]*s[1], m.m[2][0]*s[5] - m.m[2][2]*s[2] + m.m[2][3]*s[1],
        m.m[1][0]*c[4] - m.m[1][1]*c[2] + m.m[1][3]*c[0], m.m[0][1]*c[2] - m.m[0][0]*c[4] - m.m[0][3]*c[0], m.m[3][0]*s[4] - m.m[3][1]*s[2] + m.m[3][3]*s[0],
        m.m[2][1]*s[2] - m.m[2][0]*s[4] - m.m[2][3]*s[0], m.m[1][1]*c[1] - m.m[1][0]*c[3] - m.m[1][2]*c[0], m.m[0][0]*c[3] - m.m[0][1]*c[1] + m.m[0][2]*c[0],
        m.m[3][1]*s[1] - m.m[3][0]*s[3] - m.m[3][2]*s[0], m.m[2][0]*s[3] - m.m[2][1]*s[1] + m.m[2][2]*s[0]),
        1/(s[0]*c[5] - s[1]*c[4] + s[2]*c[3] + s[3]*c[2] - s[4]*c[1] + s[5]*c[0]));
}
DVDEF mat4 mat4MultiplyScalar (mat4 m, DVTYPE s) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m.m[i][j] *= s;
    return m;
}
DVDEF mat4 mat4RotateX (mat4 m, DVTYPE r) {
    return mat4MultiplyMatrix(mat4SetRotationX(r), m);
}
DVDEF mat4 mat4RotateY (mat4 m, DVTYPE r) {
    return mat4MultiplyMatrix(mat4SetRotationY(r), m);
}
DVDEF mat4 mat4RotateZ (mat4 m, DVTYPE r) {
    return mat4MultiplyMatrix(mat4SetRotationZ(r), m);
}
DVDEF mat4 mat4RotateQuaternion (mat4 m, quat q) {
    return mat4MultiplyMatrix(mat4SetRotationQuaternion(q), m);
}
DVDEF mat4 mat4Scale (mat4 m, DVTYPE s) {
    return mat4MultiplyMatrix(mat4SetScale(s), m);
}
DVDEF mat4 mat4ScaleXYZ (mat4 m, vec3 v) {
    return mat4MultiplyMatrix(mat4SetScaleXYZ(v), m);
}
DVDEF mat4 mat4Translate (mat4 m, vec3 v) {
    return mat4MultiplyMatrix(mat4SetTranslation(v), m);
}
DVDEF mat4 mat4MultiplyMatrix (mat4 m1, mat4 m2) {
    mat4 m = MAT4_IDEN;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m.m[i][j] = m1.m[0][j]*m2.m[i][0] + m1.m[1][j]*m2.m[i][1] + m1.m[2][j]*m2.m[i][2] + m1.m[3][j]*m2.m[i][3];
    return m;
}
DVDEF mat4 mat4Add (mat4 m1, mat4 m2) {
    mat4 m = MAT4_IDEN;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m.m[i][j] = m1.m[i][j] + m2.m[i][j];
    return m;
}
DVDEF mat4 mat4Subtract (mat4 m1, mat4 m2) {
    mat4 m = MAT4_IDEN;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m.m[i][j] = m1.m[i][j] - m2.m[i][j];
    return m;
}
DVDEF int mat4Equal (mat4 m1, mat4 m2) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (m1.m[i][j] != m2.m[i][j])
                return 0;
    return 1;
}

//frst functions
DVDEF frst frstFromMatrix (mat4 m) {
    frst f;
    f.pln[0] = VEC4(m.m[0][3]+m.m[0][0], m.m[1][3]+m.m[1][0], m.m[2][3]+m.m[2][0], m.m[3][3]+m.m[3][0]); //left
    f.pln[1] = VEC4(m.m[0][3]-m.m[0][0], m.m[1][3]-m.m[1][0], m.m[2][3]-m.m[2][0], m.m[3][3]-m.m[3][0]); //right
    f.pln[2] = VEC4(m.m[0][3]-m.m[0][1], m.m[1][3]-m.m[1][1], m.m[2][3]-m.m[2][1], m.m[3][3]-m.m[3][1]); //top
    f.pln[3] = VEC4(m.m[0][3]+m.m[0][1], m.m[1][3]+m.m[1][1], m.m[2][3]+m.m[2][1], m.m[3][3]+m.m[3][1]); //bottom
    f.pln[4] = VEC4(m.m[0][3]+m.m[0][2], m.m[1][3]+m.m[1][2], m.m[2][3]+m.m[2][2], m.m[3][3]+m.m[3][2]); //near
    f.pln[5] = VEC4(m.m[0][3]-m.m[0][2], m.m[1][3]-m.m[1][2], m.m[2][3]-m.m[2][2], m.m[3][3]-m.m[3][2]); //far
    for (int i = 0; i < 6; i++)
        f.pln[i] = vec4Divide(f.pln[i], vec3Length(vec4GetXYZ(f.pln[i])));
    return f;
}
DVDEF int frstCullSphere (frst f, vec3 v, DVTYPE s) {
    for (int i = 0; i < 6; i++)
        if (vec3DotProduct(vec4GetXYZ(f.pln[i]), v) + f.pln[i].w + s < 0)
            return 0;
    return 1;
}
DVDEF int frstCullAABB (frst f, vec3 min, vec3 max) {
    for (int i = 0; i < 6; i++) {
        const vec3 v = VEC3(f.pln[i].x > 0 ? max.x : min.x, f.pln[i].y > 0 ? max.y : min.y, f.pln[i].z > 0 ? max.z : min.z);
        if (vec3DotProduct(vec4GetXYZ(f.pln[i]), v) + f.pln[i].w < 0)
            return 0;
    }
    return 1;
}

#endif //DVECTOR_IMPLEMENTATION
