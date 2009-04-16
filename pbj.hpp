#include "util/Platform.hpp"
#include "util/UUID.hpp"
#include "util/BoundingSphere.hpp"
#include "util/BoundingBox.hpp"
#include "util/Vector2.hpp"
#include "util/Time.hpp"
#include "util/Sha256.hpp"

#include <google/protobuf/message.h>
namespace PBJ {

class HasFields {
public:
    bool evaluateInput(bool inp)const {
        return inp;
    }
    bool evaluateOutput(bool outp)const {
        return true;
    }
};

template <class HasAllPBJFieldsAble> class Message {
    ::google::protobuf::Message * _mMessage;

protected:
    Message(::google::protobuf::Message *msg){
        _mMessage=msg;
    }
    void setMessageRepresentation(::google::protobuf::Message * msg) {
        _mMessage=msg;
    }
public:
    void CopyFrom(const Message&from) {
        _mMessage->CopyFrom(from._mMessage);
    }
    void MergeFrom(const Message&from) {
        _mMessage->MergeFrom(from._mMessage);
    }
    void Clear() {
        _mMessage->Clear();
    }
    void DiscardUnknownFields() {
        _mMessage->DiscardUnknownFields();
    }
    bool InputOK() const{
        return static_cast<const HasAllPBJFieldsAble*>(this)->HasAllPBJFieldsAble::_HasAllPBJFields();
    }
    bool OutputOK() const{
        assert(static_cast<const HasAllPBJFieldsAble*>(this)->HasAllPBJFieldsAble::_HasAllPBJFields());
        return true;
    }
    bool IsInitialized() const {
        return _mMessage->IsInitialized()&&static_cast<const HasAllPBJFieldsAble*>(this)->HasAllPBJFieldsAble::_HasAllPBJFields();
    }
    bool ParseFromCodedStream(::google::protobuf::io::CodedInputStream* input){
        return _mMessage->ParseFromCodedStream(input)&&InputOK();
    }
    // Like ParseFromCodedStream(), but accepts messages that are missing
    // required fields.
    bool ParsePartialFromCodedStream(::google::protobuf::io::CodedInputStream* input){
        return _mMessage->ParsePartialFromCodedStream(input);
    }
    // Read a protocol buffer from the given zero-copy input stream.  If
    // successful, the entire input will be consumed.
    bool ParseFromZeroCopyStream(::google::protobuf::io::ZeroCopyInputStream* input){
        return _mMessage->ParseFromZeroCopyStream(input)&&InputOK();
    }
    // Like ParseFromZeroCopyStream(), but accepts messages that are missing
    // required fields.
    bool ParsePartialFromZeroCopyStream(::google::protobuf::io::ZeroCopyInputStream* input){
        return _mMessage->ParsePartialFromZeroCopyStream(input);
    }
  // Parse a protocol buffer contained in a std::string.
    bool ParseFromString(const std::string& data){
        return _mMessage->ParseFromString(data)&&InputOK();
    }
  // Like ParseFromString(), but accepts messages that are missing
  // required fields.
    bool ParsePartialFromString(const std::string& data){
        return _mMessage->ParsePartialFromString(data);
    }
  // Parse a protocol buffer contained in an array of bytes.
    bool ParseFromArray(const void* data, int size){
        return _mMessage->ParseFromArray(data,size)&&InputOK();
    }
  // Like ParseFromArray(), but accepts messages that are missing
  // required fields.
    bool ParsePartialFromArray(const void* data, int size){
        return _mMessage->ParsePartialFromArray(data,size);
    }

  // Parse a protocol buffer from a file descriptor.  If successful, the entire
  // input will be consumed.
    bool ParseFromFileDescriptor(int file_descriptor){
        return _mMessage->ParseFromFileDescriptor(file_descriptor)&&InputOK();
    }
  // Like ParseFromFileDescriptor(), but accepts messages that are missing
  // required fields.
    bool ParsePartialFromFileDescriptor(int file_descriptor){
        return _mMessage->ParsePartialFromFileDescriptor(file_descriptor);
    }
  // Parse a protocol buffer from a C++ istream.  If successful, the entire
  // input will be consumed.
    bool ParseFromIstream(std::istream* input){
        return _mMessage->ParseFromIstream(input)&&InputOK();
    }
  // Like ParseFromIstream(), but accepts messages that are missing
  // required fields.
    bool ParsePartialFromIstream(std::istream* input){
        return _mMessage->ParsePartialFromIstream(input);
    }


  // Reads a protocol buffer from the stream and merges it into this
  // Message.  Singular fields read from the input overwrite what is
  // already in the Message and repeated fields are appended to those
  // already present.
  //
  // It is the responsibility of the caller to call input->LastTagWas()
  // (for groups) or input->ConsumedEntireMessage() (for non-groups) after
  // this returns to verify that the message's end was delimited correctly.
  //
  // ParsefromCodedStream() is implemented as Clear() followed by
  // MergeFromCodedStream().
    bool MergeFromCodedStream(::google::protobuf::io::CodedInputStream* input){
        return _mMessage->MergeFromCodedStream(input)&&InputOK();
    }

  // Like MergeFromCodedStream(), but succeeds even if required fields are
  // missing in the input.
  //
  // MergeFromCodedStream() is just implemented as MergePartialFromCodedStream()
  // followed by IsInitialized().
    bool MergePartialFromCodedStream(::google::protobuf::io::CodedInputStream* input){
        return _mMessage->MergePartialFromCodedStream(input);
    }

  // Serialization ---------------------------------------------------
  // Methods for serializing in protocol buffer format.  Most of these
  // are just simple wrappers around ByteSize() and SerializeWithCachedSizes().

  // Write a protocol buffer of this message to the given output.  Returns
  // false on a write error.  If the message is missing required fields,
  // this may GOOGLE_CHECK-fail.
    bool SerializeToCodedStream(::google::protobuf::io::CodedOutputStream* output) const{
        return _mMessage->SerializeToCodedStream(output)&&OutputOK();
    }
  // Like SerializeToCodedStream(), but allows missing required fields.
    bool SerializePartialToCodedStream(::google::protobuf::io::CodedOutputStream* output) const{
        return _mMessage->SerializePartialToCodedStream(output);
    }
  // Write the message to the given zero-copy output stream.  All required
  // fields must be set.
    bool SerializeToZeroCopyStream(::google::protobuf::io::ZeroCopyOutputStream* output) const{
        return _mMessage->SerializeToZeroCopyStream(output)&&OutputOK();
    }
  // Like SerializeToZeroCopyStream(), but allows missing required fields.
    bool SerializePartialToZeroCopyStream(::google::protobuf::io::ZeroCopyOutputStream* output) const{
        return _mMessage->SerializePartialToZeroCopyStream(output);
    }
  // Serialize the message and store it in the given std::string.  All required
  // fields must be set.
    bool SerializeToString(std::string* output) const{
        return _mMessage->SerializeToString(output)&&OutputOK();
    }
  // Like SerializeToString(), but allows missing required fields.
    bool SerializePartialToString(std::string* output) const{
        return _mMessage->SerializePartialToString(output);
    }
  // Serialize the message and store it in the given byte array.  All required
  // fields must be set.
    bool SerializeToArray(void* data, int size) const{
        return _mMessage->SerializeToArray(data,size)&&OutputOK();
    }
  // Like SerializeToArray(), but allows missing required fields.
    bool SerializePartialToArray(void* data, int size) const{
        return _mMessage->SerializePartialToArray(data,size);
    }

  // Serialize the message and write it to the given file descriptor.  All
  // required fields must be set.
    bool SerializeToFileDescriptor(int file_descriptor) const{
        return _mMessage->SerializeToFileDescriptor(file_descriptor)&&OutputOK();
    }
  // Like SerializeToFileDescriptor(), but allows missing required fields.
    bool SerializePartialToFileDescriptor(int file_descriptor) const{
        return _mMessage->SerializePartialToFileDescriptor(file_descriptor);
    }
  // Serialize the message and write it to the given C++ ostream.  All
  // required fields must be set.
    bool SerializeToOstream(std::ostream* output) const{
        return _mMessage->SerializeToOstream(output)&&OutputOK();
    }
  // Like SerializeToOstream(), but allows missing required fields.
    bool SerializePartialToOstream(std::ostream* output) const{
        return _mMessage->SerializePartialToOstream(output);
    }


  // Like SerializeToString(), but appends to the data to the std::string's existing
  // contents.  All required fields must be set.
    bool AppendToString(std::string* output) const{
        return _mMessage->AppendToString(output)&&OutputOK();
    }
  // Like AppendToString(), but allows missing required fields.
    bool AppendPartialToString(std::string* output) const{
        return _mMessage->AppendPartialToString(output)&&OutputOK();
    }

  // Computes the serialized size of the message.  This recursively calls
  // ByteSize() on all embedded messages.  If a subclass does not override
  // this, it MUST override SetCachedSize().
    virtual int ByteSize() const{
        return _mMessage->ByteSize();
    }

  // Serializes the message without recomputing the size.  The message must
  // not have changed since the last call to ByteSize(); if it has, the results
  // are undefined.
    bool SerializeWithCachedSizes(::google::protobuf::io::CodedOutputStream* output) const{
        return _mMessage->SerializeWithCachedSizes(output)&&OutputOK();
    }

  // Returns the result of the last call to ByteSize().  An embedded message's
  // size is needed both to serialize it (because embedded messages are
  // length-delimited) and to compute the outer message's size.  Caching
  // the size avoids computing it multiple times.
  //
  // ByteSize() does not automatically use the cached size when available
  // because this would require invalidating it every time the message was
  // modified, which would be too hard and expensive.  (E.g. if a deeply-nested
  // sub-message is changed, all of its parents' cached sizes would need to be
  // invalidated, which is too much work for an otherwise inlined setter
  // method.)
    virtual int GetCachedSize() const {
        return _mMessage->GetCachedSize();
    }
    
};

typedef Sirikata::SHA256 SHA256;
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

typedef Sirikata::Array<float,2> Array2f;
typedef Sirikata::Array<double,2> Array2d;
typedef Sirikata::Array<float,3> Array3f;
typedef Sirikata::Array<double,3> Array3d;
typedef Sirikata::Array<float,4> Array4f;
typedef Sirikata::Array<double,4> Array4d;
typedef Sirikata::Array<float,6> Array6f;
typedef Sirikata::Array<double,6> Array6d;

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
template <> class _PBJConstruct<PBJ::SHA256> {
public:
    std::string operator()(const PBJ::SHA256&ct) {
        return std::string((const char*)ct.rawData().begin(),(size_t)PBJ::SHA256::static_size);
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

template <> class _PBJConstruct<PBJ::Vector2f> {
public:
    typedef PBJ::Array2f ArrayType;
    PBJ::Array2f operator()(const PBJ::Vector2f&ct) {
        float data[2]={ct.x,ct.y};
        return PBJ::Array2f::construct(data);
    }
};
template <> class _PBJConstruct<PBJ::Vector2d> {
public:
    typedef PBJ::Array2d ArrayType;
    PBJ::Array2d operator()(const PBJ::Vector2d&ct) {
        double data[2]={ct.x,ct.y};
        return PBJ::Array2d::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::normal> {
public:
    typedef PBJ::Array2f ArrayType;
    PBJ::Array2f operator()(const PBJ::Vector3f&n) {
        PBJ::Vector3f ct=n/n.length();
        float data[2]={ct.x+(ct.z<0.0f?3.0f:0.0f),ct.y};
        return PBJ::Array2f::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::Vector3f> {
public:
    typedef PBJ::Array3f ArrayType;
    PBJ::Array3f operator()(const PBJ::Vector3f&ct) {
        float data[3]={ct.x,ct.y,ct.z};
        return PBJ::Array3f::construct(data);
    }
};
template <> class _PBJConstruct<PBJ::Vector3d> {
public:
    typedef PBJ::Array3d ArrayType;
    PBJ::Array3d operator()(const PBJ::Vector3d&ct) {
        double data[3]={ct.x,ct.y,ct.z};
        return PBJ::Array3d::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::Vector4f> {
public:
    typedef PBJ::Array4f ArrayType;
    PBJ::Array4f operator()(const PBJ::Vector4f&ct) {
        float data[4]={ct.x,ct.y,ct.z,ct.w};
        return PBJ::Array4f::construct(data);
    }
};
template <> class _PBJConstruct<PBJ::Vector4d> {
public:
    typedef PBJ::Array4d ArrayType;
    PBJ::Array4d operator()(const PBJ::Vector4d&ct) {
        double data[4]={ct.x,ct.y,ct.z,ct.w};
        return PBJ::Array4d::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::BoundingSphere3f> {
public:
    typedef PBJ::Array4f ArrayType;
    PBJ::Array4f operator()(const PBJ::BoundingSphere3f&ct) {
        float data[4]={ct.center().x,ct.center().y,ct.center().z,ct.radius()};
        return PBJ::Array4f::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::BoundingSphere3d> {
public:
    typedef PBJ::Array4d ArrayType;
    PBJ::Array4d operator()(const PBJ::BoundingSphere3d&ct) {
        double data[4]={ct.center().x,ct.center().y,ct.center().z,ct.radius()};
        return PBJ::Array4d::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::Quaternion> {
public:
    typedef PBJ::Array3f ArrayType;
    PBJ::Array3f operator()(const PBJ::Quaternion&q) {
        PBJ::Quaternion ct=q/q.length();
        
        float data[3]={ct.x+(ct.w<0?3.0:0.0),ct.y,ct.z};
        return PBJ::Array3f::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::BoundingBox3f3f> {
public:
    typedef PBJ::Array6f ArrayType;
    PBJ::Array6f operator()(const PBJ::BoundingBox3f3f&ct) {
        float data[6]={ct.center().x,ct.center().y,ct.center().z,ct.across().x,ct.across().y,ct.across().z};
        return PBJ::Array6f::construct(data);
    }
};

template <> class _PBJConstruct<PBJ::BoundingBox3d3f> {
public:
    typedef PBJ::Array6d ArrayType;
    PBJ::Array6d operator()(const PBJ::BoundingBox3d3f&ct) {
        double data[6]={ct.center().x,ct.center().y,ct.center().z,ct.across().x,ct.across().y,ct.across().z};
        return PBJ::Array6d::construct(data);
    }
};


template <typename convertFrom, typename convertTo> class _PBJCastMessage {
public:
    template <typename Type> const Type& operator()(const Type&ct) {
        return ct;
    }
    const convertTo& operator()() {
        static convertFrom retval;
        return retval;;
    }
};
template <typename convertTo> class _PBJCast {
public:
    template <typename Type> const Type& operator()(const Type&ct) {
        return ct;
    }
    convertTo operator()() {
        return (convertTo)0;
    }
};

template <> class _PBJCast<PBJ::utf8string> {
public:
    const std::string& operator()(const std::string& s) {
        return s;
    }
    const std::string operator()() {
        return std::string();
    }
};
template <> class _PBJCast<PBJ::bytes> {
public:
    const std::string& operator()(const std::string& s) {
        return s;
    }
    const std::string operator()() {
        return std::string();
    }
};


template <> class _PBJCast<PBJ::normal> {
public:
    PBJ::Vector3f operator()() {
        return PBJ::Vector3f(0,0,1);
    }
    PBJ::Vector3f operator()(float x, float y) {
        float neg=(x>1.5||y>1.5)?-1.0:1.0;
        if (x>1.5)
            x-=3;
        if (y>1.5)
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
template <> class _PBJCast<PBJ::SHA256> {
public:
    PBJ::SHA256 operator()(const std::string bytes) {
        return PBJ::SHA256::convertFromBinary(bytes.data());
    }
    PBJ::SHA256 operator()() {
        return PBJ::SHA256::null();
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
        float neg=(x>1.5||y>1.5||z>1.5)?-1.0:1.0;        
        if (x>1.5) x-=3;
        if (y>1.5) y-=3;
        if (z>1.5) z-=3;
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


template <> class _PBJCast<PBJ::BoundingSphere3f> {
public:
    PBJ::BoundingSphere3f operator()(double x, double y, double z, float r) {
        return PBJ::BoundingSphere3f(PBJ::Vector3f(x,y,z),r);
    }
    PBJ::BoundingSphere3f operator()() {
        return PBJ::BoundingSphere3f(PBJ::Vector3f(0,0,0),0);
    }
};
template <> class _PBJCast<PBJ::BoundingSphere3d> {
public:
    PBJ::BoundingSphere3d operator()(double x, double y, double z, float r) {
        return PBJ::BoundingSphere3d(PBJ::Vector3d(x,y,z),r);
    }
    PBJ::BoundingSphere3d operator()() {
        return PBJ::BoundingSphere3d(PBJ::Vector3d(0,0,0),0);
    }
};


template <> class _PBJCast<PBJ::BoundingBox3f3f> {
public:
    PBJ::BoundingBox3f3f operator()(double x, double y, double z, float w, float h, float d) {
        return PBJ::BoundingBox3f3f(PBJ::Vector3f(x,y,z),PBJ::Vector3f(x+w,y+h,z+d));
    }
    PBJ::BoundingBox3f3f operator()() {
        return PBJ::BoundingBox3f3f(PBJ::Vector3f(0,0,0),PBJ::Vector3f(0,0,0));
    }
};
template <> class _PBJCast<PBJ::BoundingBox3d3f> {
public:
    PBJ::BoundingBox3d3f operator()(double x, double y, double z, double w, double h, double d) {
        return PBJ::BoundingBox3d3f(PBJ::Vector3d(x,y,z),PBJ::Vector3d(x+w,y+h,z+d));
    }
    PBJ::BoundingBox3d3f operator()() {
        return PBJ::BoundingBox3d3f(PBJ::Vector3d(0,0,0),0);
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
template <typename Type> class _PBJValidateFlags {
public:
    bool operator() (Type input, Type allFlagsOn) {
        return (input|allFlagsOn)==allFlagsOn;
    }
};
template <typename Type> class _PBJCastFlags {
public:
    Type operator() (Type input, Type allFlagsOn) {
        return input&allFlagsOn;
    }
};
