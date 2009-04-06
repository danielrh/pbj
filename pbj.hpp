#include "util/Platform.hpp"
#include "util/UUID.hpp"
#include "util/BoundingSphere.hpp"
#include "util/BoundingBox.hpp"
#include "util/Vector2.hpp"
#include "util/Time.hpp"
namespace PBJ {
typedef Sirikata::UUID UUID;
typedef Sirikata::uint64 uint64;
typedef Sirikata::uint32 uint32;
typedef Sirikata::uint16 uint16;
typedef Sirikata::int8 int8;
typedef Sirikata::int64 int64;
typedef Sirikata::int32 int32;
typedef Sirikata::int16 int16;
typedef Sirikata::int8 int8;
typedef Sirikata::Time Time;

typedef Sirikata::Duration Duration;
typedef Sirikata::Vector2<float> Vector2f;
typedef Sirikata::Vector2<double> Vector2d;
typedef Sirikata::Vector3f Vector3f;
typedef Sirikata::Vector3d Vector3d;
typedef Sirikata::Vector4f Vector4f;
typedef Sirikata::Vector4d Vector4d;
typedef Sirikata::Quaternion Quaternion;
typedef Sirikata::BoundingBox3f3f BoundingBox3f3f;
typedef Sirikata::BoundingBox3d3f BoundingBox3d3f;
typedef Sirikata::BoundingSphere3f BoundingSphere3f;
typedef Sirikata::BoundingSphere3d BoundingSphere3d;
class angle {};
class normal :public Vector3f{public:normal(const Vector3f&v):Vector3f(v){}};
class vector2f {};
class vector3f {};
class vector4f {};
class vector2d {};
class vector3d {};
class vector4d {};
class quaternion{};
class boundingsphere3f{};
class boundingsphere3d{};
class boundingbox3d3f{};
class boundingbox3dff{};
class utf8string:public ::std::string{public:utf8string(const std::string&s):std::string(s){}};
class bytes:public ::std::string{public:bytes(const std::string&s):std::string(s){}};
}



template <typename convertTo> class _PBJConstruct {
public:
    convertTo operator()(const convertTo&ct) {
        return ct;
    }
};
template <> class _PBJConstruct<PBJ::angle> {
public:
    float operator()(const float &ang) {
        return ang;
    }
};
template <> class _PBJConstruct<PBJ::Time> {
public:
    PBJ::int64 operator()(const PBJ::Time &time) {
        return (time-PBJ::Time::null()).toMicro();
    }
};
template <> class _PBJConstruct<PBJ::Duration> {
public:
    PBJ::int64 operator()(const PBJ::Duration &time) {
        return time.toMicro();
    }
};
template <> class _PBJConstruct<PBJ::UUID> {
public:
    std::string operator()(const PBJ::UUID&ct) {
        return ct.rawHexData();
    }
};
template <> class _PBJConstruct<PBJ::utf8string> {
public:
    const std::string& operator()(const std::string&ct) {
        return ct;
    }
};
template <> class _PBJConstruct<PBJ::bytes> {
public:
    const std::string& operator()(const std::string&ct) {
        return ct;
    }
};



template <typename convertTo> class _PBJCast {
public:
    const convertTo& operator()(const convertTo&ct) {
        return ct;
    }
    convertTo operator()() {
        return convertTo();
    }
};
template <> class _PBJCast<PBJ::normal> {
public:
    PBJ::Vector3f operator()() {
        return PBJ::Vector3f(0,0,1);
    }
    PBJ::Vector3f operator()(float x, float y) {
        float neg=(x>1||y>1)?-1.0:1.0;
        if (x>1)
            x-=3;
        if (y>1)
            y-=3;
        
        return PBJ::Vector3f(x,y,neg-neg*sqrt(x*x+y*y));
    }

};

template <> class _PBJCast<PBJ::UUID> {
public:
    PBJ::UUID operator()(const std::string bytes) {
        return PBJ::UUID(bytes,PBJ::UUID::BinaryString());
    }
    PBJ::UUID operator()() {
        return PBJ::UUID::null();
    }
};
template <> class _PBJCast<PBJ::angle> {
public:
    float operator()(const float ang) {
        return ang;
    }
    float operator()() {
        return 0;
    }
};
template <> class _PBJCast<PBJ::Time> {
public:
    PBJ::Time operator()(const PBJ::int64 &time) {
        return PBJ::Time::microseconds(time);
    }
    PBJ::Time operator()() {
        return PBJ::Time::null();
    }
};
template <> class _PBJCast<PBJ::Duration> {
public:
    PBJ::Duration operator()(const PBJ::int64 &time) {
        return PBJ::Duration::microseconds(time);
    }
    PBJ::Duration operator()() {
        return PBJ::Duration::seconds(0);
    }
};


template <> class _PBJCast<PBJ::Quaternion> {
public:
    PBJ::Quaternion operator()(float x, float y,float z) {
        float neg=(x>1||y>1||z>1)?-1.0:1.0;        
        if (x>1) x-=3;
        if (y>1) y-=3;
        if (z>1) z-=3;
        return PBJ::Quaternion(x,y,z,neg-neg*sqrt(x*x+y*y+z*z),PBJ::Quaternion::XYZW());
    }
    PBJ::Quaternion operator()() {
        return PBJ::Quaternion::identity();
    }
};
template <> class _PBJCast<PBJ::Vector2f> {
public:
    PBJ::Vector2f operator()(float x, float y) {
        return PBJ::Vector2f(x,y);
    }
    PBJ::Vector2f operator()() {
        return PBJ::Vector2f(0,0);
    }
};
template <> class _PBJCast<PBJ::Vector2d> {
public:
    PBJ::Vector2d operator()(double x, double y) {
        return PBJ::Vector2d(x,y);
    }
    PBJ::Vector2d operator()() {
        return PBJ::Vector2d(0,0);
    }
};

template <> class _PBJCast<PBJ::Vector3f> {
public:
    PBJ::Vector3f operator()(float x, float y, float z) {
        return PBJ::Vector3f(x,y,z);
    }
    PBJ::Vector3f operator()() {
        return PBJ::Vector3f(0,0,0);
    }

};
template <> class _PBJCast<PBJ::Vector3d> {
public:
    PBJ::Vector3d operator()(double x, double y, double z) {
        return PBJ::Vector3d(x,y,z);
    }
    PBJ::Vector3d operator()() {
        return PBJ::Vector3d(0,0,0);
    }
};

template <> class _PBJCast<PBJ::Vector4f> {
public:
    PBJ::Vector4f operator()(float x, float y, float z, float w) {
        return PBJ::Vector4f(x,y,z,w);
    }
    PBJ::Vector4f operator()() {
        return PBJ::Vector4f(0,0,0,0);
    }
};
template <> class _PBJCast<PBJ::Vector4d> {
public:
    PBJ::Vector4d operator()(double x, double y, double z, float w) {
        return PBJ::Vector4d(x,y,z,w);
    }
    PBJ::Vector4d operator()() {
        return PBJ::Vector4d(0,0,0,0);
    }
};




template <typename Type> class _PBJValidate {
public:
    template <typename Input> bool operator()(const Input&ct) {
        return true;
    }
};
template <> class _PBJValidate<PBJ::utf8string> {
public:
    bool operator()(const std::string&ct) {
        return true;
    }
};
template <> class _PBJValidate<PBJ::angle> {
public:
    bool operator()(const float&ct) {
        return ct>=0&&ct<=2*3.1415926536;
    }
};
