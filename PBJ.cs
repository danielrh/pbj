
namespace PBJ {

public class Message {

}

public struct Vector2f {

    public float x;
    public float y;


    public Vector2f(float _x, float _y) {
        x = _x; y = _y;
    }

    public Vector2f(Vector2f cpy) {
        x = cpy.x; y = cpy.y;
    }


    public Vector2f Negate() {
        return new Vector2f(-x, -y);
    }

    public Vector2f Add(Vector2f rhs) {
        return new Vector2f(x + rhs.x, y + rhs.y);
    }

    public Vector2f Subtract(Vector2f rhs) {
        return new Vector2f(x - rhs.x, y - rhs.y);
    }

    public Vector2f Multiply(Vector2f rhs) {
        return new Vector2f(x * rhs.x, y * rhs.y);
    }

    public Vector2f Multiply(float s) {
        return new Vector2f(x * s, y * s);
    }

    public Vector2f Divide(Vector2f rhs) {
        return new Vector2f(x / rhs.x, y / rhs.y);
    }

    public Vector2f Divide(float s) {
        return new Vector2f(x / s, y / s);
    }

    public float Dot(Vector2f rhs) {
        return (x*rhs.x + y*rhs.y);
    }

    public void Normalize() {
        float len = Length;
        if (len != 0.0) {
            x /= len; y /= len;
        }
    }

    public Vector2f Normalized {
        get {
            Vector2f normed = new Vector2f(this);
            normed.Normalize();
            return normed;
        }
    }

    public float SquaredLength {
        get {
            return (x*x + y*y);
        }
    }
    public float Length {
        get {
            return Math.Sqrt(SquaredLength);
        }
    }


    public override string ToString() {
        return String.Format("<{0}, {1}>", x, y);
    }


    public static Vector2f operator-(Vector2f uo) {
        return uo.Negate();
    }

    public static Vector2f operator+(Vector2f lhs, Vector2f rhs) {
        return lhs.Add(rhs);
    }

    public static Vector2f operator-(Vector2f lhs, Vector2f rhs) {
        return lhs.Subtract(rhs);
    }

    public static Vector2f operator*(Vector2f lhs, Vector2f rhs) {
        return lhs.Multiply(rhs);
    }

    public static Vector2f operator*(Vector2f lhs, float rhs) {
        return lhs.Multiply(rhs);
    }
    public static Vector2f operator*(float lhs, Vector2f rhs) {
        return rhs.Multiply(lhs);
    }

    public static Vector2f operator/(Vector2f lhs, Vector2f rhs) {
        return lhs.Divide(rhs);
    }

    public static Vector2f operator/(Vector2f lhs, float rhs) {
        return lhs.Divide(rhs);
    }

} // struct Vector2f

public struct Vector2d {

    public double x;
    public double y;


    public Vector2d(double _x, double _y) {
        x = _x; y = _y;
    }

    public Vector2d(Vector2d cpy) {
        x = cpy.x; y = cpy.y;
    }


    public Vector2d Negate() {
        return new Vector2d(-x, -y);
    }

    public Vector2d Add(Vector2d rhs) {
        return new Vector2d(x + rhs.x, y + rhs.y);
    }

    public Vector2d Subtract(Vector2d rhs) {
        return new Vector2d(x - rhs.x, y - rhs.y);
    }

    public Vector2d Multiply(Vector2d rhs) {
        return new Vector2d(x * rhs.x, y * rhs.y);
    }

    public Vector2d Multiply(double s) {
        return new Vector2d(x * s, y * s);
    }

    public Vector2d Divide(Vector2d rhs) {
        return new Vector2d(x / rhs.x, y / rhs.y);
    }

    public Vector2d Divide(double s) {
        return new Vector2d(x / s, y / s);
    }

    public double Dot(Vector2d rhs) {
        return (x*rhs.x + y*rhs.y);
    }

    public void Normalize() {
        double len = Length;
        if (len != 0.0) {
            x /= len; y /= len;
        }
    }

    public Vector2d Normalized {
        get {
            Vector2d normed = new Vector2d(this);
            normed.Normalize();
            return normed;
        }
    }

    public double SquaredLength {
        get {
            return (x*x + y*y);
        }
    }
    public double Length {
        get {
            return Math.Sqrt(SquaredLength);
        }
    }


    public override string ToString() {
        return String.Format("<{0}, {1}>", x, y);
    }


    public static Vector2d operator-(Vector2d uo) {
        return uo.Negate();
    }

    public static Vector2d operator+(Vector2d lhs, Vector2d rhs) {
        return lhs.Add(rhs);
    }

    public static Vector2d operator-(Vector2d lhs, Vector2d rhs) {
        return lhs.Subtract(rhs);
    }

    public static Vector2d operator*(Vector2d lhs, Vector2d rhs) {
        return lhs.Multiply(rhs);
    }

    public static Vector2d operator*(Vector2d lhs, double rhs) {
        return lhs.Multiply(rhs);
    }
    public static Vector2d operator*(double lhs, Vector2d rhs) {
        return rhs.Multiply(lhs);
    }

    public static Vector2d operator/(Vector2d lhs, Vector2d rhs) {
        return lhs.Divide(rhs);
    }

    public static Vector2d operator/(Vector2d lhs, double rhs) {
        return lhs.Divide(rhs);
    }

} // struct Vector2d

public struct Vector3f {

    public float x;
    public float y;
    public float z;


    public Vector3f(float _x, float _y, float _z) {
        x = _x; y = _y; z = _z;
    }

    public Vector3f(Vector3f cpy) {
        x = cpy.x; y = cpy.y; z = cpy.z;
    }


    public Vector3f Negate() {
        return new Vector3f(-x, -y, -z);
    }

    public Vector3f Add(Vector3f rhs) {
        return new Vector3f(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    public Vector3f Subtract(Vector3f rhs) {
        return new Vector3f(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    public Vector3f Multiply(Vector3f rhs) {
        return new Vector3f(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    public Vector3f Multiply(float s) {
        return new Vector3f(x * s, y * s, z * s);
    }

    public Vector3f Divide(Vector3f rhs) {
        return new Vector3f(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    public Vector3f Divide(float s) {
        return new Vector3f(x / s, y / s, z / s);
    }

    public float Dot(Vector3f rhs) {
        return (x*rhs.x + y*rhs.y + z*rhs.z);
    }

    public Vector3f Cross(Vector3f rhs) {
        return new Vector3f( y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
    }

    public void Normalize() {
        float len = Length;
        if (len != 0.0) {
            x /= len; y /= len; z /= len;
        }
    }

    public Vector3f Normalized {
        get {
            Vector3f normed = new Vector3f(this);
            normed.Normalize();
            return normed;
        }
    }

    public float SquaredLength {
        get {
            return (x*x + y*y + z*z);
        }
    }
    public float Length {
        get {
            return Math.Sqrt(SquaredLength);
        }
    }


    public override string ToString() {
        return String.Format("<{0}, {1}, {2}>", x, y, z);
    }


    public static Vector3f operator-(Vector3f uo) {
        return uo.Negate();
    }

    public static Vector3f operator+(Vector3f lhs, Vector3f rhs) {
        return lhs.Add(rhs);
    }

    public static Vector3f operator-(Vector3f lhs, Vector3f rhs) {
        return lhs.Subtract(rhs);
    }

    public static Vector3f operator*(Vector3f lhs, Vector3f rhs) {
        return lhs.Multiply(rhs);
    }

    public static Vector3f operator*(Vector3f lhs, float rhs) {
        return lhs.Multiply(rhs);
    }
    public static Vector3f operator*(float lhs, Vector3f rhs) {
        return rhs.Multiply(lhs);
    }

    public static Vector3f operator/(Vector3f lhs, Vector3f rhs) {
        return lhs.Divide(rhs);
    }

    public static Vector3f operator/(Vector3f lhs, float rhs) {
        return lhs.Divide(rhs);
    }

} // struct Vector3f

public struct Vector3d {

    public double x;
    public double y;
    public double z;


    public Vector3d(double _x, double _y, double _z) {
        x = _x; y = _y; z = _z;
    }

    public Vector3d(Vector3d cpy) {
        x = cpy.x; y = cpy.y; z = cpy.z;
    }


    public Vector3d Negate() {
        return new Vector3d(-x, -y, -z);
    }

    public Vector3d Add(Vector3d rhs) {
        return new Vector3d(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    public Vector3d Subtract(Vector3d rhs) {
        return new Vector3d(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    public Vector3d Multiply(Vector3d rhs) {
        return new Vector3d(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    public Vector3d Multiply(double s) {
        return new Vector3d(x * s, y * s, z * s);
    }

    public Vector3d Divide(Vector3d rhs) {
        return new Vector3d(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    public Vector3d Divide(double s) {
        return new Vector3d(x / s, y / s, z / s);
    }

    public double Dot(Vector3d rhs) {
        return (x*rhs.x + y*rhs.y + z*rhs.z);
    }

    public Vector3d Cross(Vector3d rhs) {
        return new Vector3d( y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
    }

    public void Normalize() {
        double len = Length;
        if (len != 0.0) {
            x /= len; y /= len; z /= len;
        }
    }

    public Vector3d Normalized {
        get {
            Vector3d normed = new Vector3d(this);
            normed.Normalize();
            return normed;
        }
    }

    public double SquaredLength {
        get {
            return (x*x + y*y + z*z);
        }
    }
    public double Length {
        get {
            return Math.Sqrt(SquaredLength);
        }
    }


    public override string ToString() {
        return String.Format("<{0}, {1}, {2}>", x, y, z);
    }


    public static Vector3d operator-(Vector3d uo) {
        return uo.Negate();
    }

    public static Vector3d operator+(Vector3d lhs, Vector3d rhs) {
        return lhs.Add(rhs);
    }

    public static Vector3d operator-(Vector3d lhs, Vector3d rhs) {
        return lhs.Subtract(rhs);
    }

    public static Vector3d operator*(Vector3d lhs, Vector3d rhs) {
        return lhs.Multiply(rhs);
    }

    public static Vector3d operator*(Vector3d lhs, double rhs) {
        return lhs.Multiply(rhs);
    }
    public static Vector3d operator*(double lhs, Vector3d rhs) {
        return rhs.Multiply(lhs);
    }

    public static Vector3d operator/(Vector3d lhs, Vector3d rhs) {
        return lhs.Divide(rhs);
    }

    public static Vector3d operator/(Vector3d lhs, double rhs) {
        return lhs.Divide(rhs);
    }

} // struct Vector3d

public struct Quaternion {

    public float w;
    public float x;
    public float y;
    public float z;


    public Quaternion(float _w, float _x, float _y, float _z) {
        w = _w; x = _x; y = _y; z = _z;
    }

    public Quaternion(Quaternion cpy) {
        w = cpy.w; x = cpy.x; y = cpy.y; z = cpy.z;
    }

    public static readonly Quaternion Identity = new Quaternion(1.0, 0.0, 0.0, 0.0);

    public static Quaternion FromAxisAngle(Vector3f axis, float rads) {
	float halfAngle = rads * 0.5;
	float sinHalf = Math.Sin(halfAngle);
	float w = Math.Cos(halfAngle);
	float x = sinHalf*axis.x;
	float y = sinHalf*axis.y;
	float z = sinHalf*axis.z;
        return new Quaternion(w, x, y, z);
    }

    public static Quaternion FromAxisAngle(Vector3d axis, float rads) {
	float halfAngle = rads * 0.5;
	float sinHalf = Math.Sin(halfAngle);
	float w = Math.Cos(halfAngle);
	float x = sinHalf*axis.x;
	float y = sinHalf*axis.y;
	float z = sinHalf*axis.z;
        return new Quaternion(w, x, y, z);
    }


    public Quaternion Add(Quaternion rhs) {
        return new Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
    }

    public Quaternion Subtract(Quaternion rhs) {
        return new Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
    }

    public Quaternion Multiply(Quaternion rhs) {
	return new Quaternion(
            w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z,
            w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y,
            w*rhs.y + y*rhs.w + z*rhs.x - x*rhs.z,
            w*rhs.z + z*rhs.w + x*rhs.y - y*rhs.x
	);
    }

    public Vector3f Multiply(Vector3f rhs) {
	Vector3f qvec = new Vector3f(x, y, z);
	Vector3f uv = qvec.Cross(rhs);
	Vector3f uuv = qvec.Cross(uv);
	uv *= 2.0 * w;
	uuv *= 2.0;

	return rhs + uv + uuv;
    }

    public Vector3d Multiply(Vector3d rhs) {
	Vector3d qvec = new Vector3f(x, y, z);
	Vector3d uv = qvec.Cross(rhs);
	Vector3d uuv = qvec.Cross(uv);
	uv *= 2.0 * w;
	uuv *= 2.0;

	return rhs + uv + uuv;
    }

    public Quaternion Multiply(float rhs) {
        return new Quaternion(w*rhs, x*rhs, y*rhs, z*rhs);
    }

    public Quaternion Negate() {
        return new Quaternion(-w, -x, -y, -z);
    }

    public float Dot(Quaternion rhs) {
        return (w*rhs.w + x*rhs.x + y*rhs.y + z*rhs.z);
    }

    public float Norm {
        get {
            return Math.Sqrt(w*w + x*x + y*y + z*z);
        }
    }

    public float SquareNorm {
        get {
            return (w*w + x*x + y*y + z*z);
        }
    }

    public void Normalize() {
        float len = SquareNorm;
        if (len == 0.0) return;
        float factor = 1.0 / Math.Sqrt(len);
        this *= factor;
    }

    public Quaternion Normalized {
        get {
            Quaternion q = new Quaternion(this);
            q.Normalize();
            return q;
        }
    }

    public Quaternion Inverse {
        get {
            float norm = SquareNorm;
            if (norm > 0.0) {
                float invnorm = 1.0 / norm;
                return new Quaternion(w*invnorm, -x*invnorm, -y*invnorm, -z*invnorm);
            }
            else
                return new Quaternion(0.0, 0.0, 0.0, 0.0);
        }
    }


    public override string ToString() {
        return String.Format("<{0}, {1}, {2}, {3}>", w, x, y, z);
    }



    public static Quaternion operator-(Quaternion uo) {
        return uo.Negate();
    }

    public static Quaternion operator+(Quaternion lhs, Quaternion rhs) {
        return lhs.Add(rhs);
    }

    public static Quaternion operator-(Quaternion lhs, Quaternion rhs) {
        return lhs.Subtract(rhs);
    }

    public static Vector3f operator*(Quaternion lhs, Vector3f rhs) {
        return lhs.Multiply(rhs);
    }

    public static Vector3d operator*(Quaternion lhs, Vector3d rhs) {
        return lhs.Multiply(rhs);
    }

    public static Quaternion operator*(Quaternion lhs, Quaternion rhs) {
        return lhs.Multiply(rhs);
    }

    public static Quaternion operator*(Quaternion lhs, float rhs) {
        return lhs.Multiply(rhs);
    }

    public static Quaternion operator*(float lhs, Quaternion rhs) {
        return rhs.Multiply(lhs);
    }

} // struct Quaternion


public struct Vector4f {

    public float x;
    public float y;
    public float z;
    public float w;


    public Vector4f(float _x, float _y, float _z, float _w) {
        x = _x; y = _y; z = _z; w = _w;
    }

    public Vector4f(Vector4f cpy) {
        x = cpy.x; y = cpy.y; z = cpy.z; w = cpy.w;
    }


    public Vector4f Negate() {
        return new Vector4f(-x, -y, -z, -w);
    }

    public Vector4f Add(Vector4f rhs) {
        return new Vector4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    public Vector4f Subtract(Vector4f rhs) {
        return new Vector4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    public Vector4f Multiply(Vector4f rhs) {
        return new Vector4f(x * rhs.x, y * rhs.y, z * rhs.z, w*rhs.w);
    }

    public Vector4f Multiply(float s) {
        return new Vector4f(x * s, y * s, z * s, w * s);
    }

    public Vector4f Divide(Vector4f rhs) {
        return new Vector4f(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
    }

    public Vector4f Divide(float s) {
        return new Vector3f(x / s, y / s, z / s, w / s);
    }

    public float Dot(Vector4f rhs) {
        return (x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w);
    }

    public void Normalize() {
        float len = Length;
        if (len != 0.0) {
            x /= len; y /= len; z /= len; w /= len;
        }
    }

    public Vector4f Normalized {
        get {
            Vector4f normed = new Vector4f(this);
            normed.Normalize();
            return normed;
        }
    }

    public float SquaredLength {
        get {
            return (x*x + y*y + z*z + w*w);
        }
    }
    public float Length {
        get {
            return Math.Sqrt(SquaredLength);
        }
    }


    public override string ToString() {
        return String.Format("<{0}, {1}, {2}, {3}>", x, y, z, w);
    }


    public static Vector4f operator-(Vector4f uo) {
        return uo.Negate();
    }

    public static Vector4f operator+(Vector4f lhs, Vector4f rhs) {
        return lhs.Add(rhs);
    }

    public static Vector4f operator-(Vector4f lhs, Vector4f rhs) {
        return lhs.Subtract(rhs);
    }

    public static Vector4f operator*(Vector4f lhs, Vector4f rhs) {
        return lhs.Multiply(rhs);
    }

    public static Vector4f operator*(Vector4f lhs, float rhs) {
        return lhs.Multiply(rhs);
    }
    public static Vector4f operator*(float lhs, Vector4f rhs) {
        return rhs.Multiply(lhs);
    }

    public static Vector4f operator/(Vector4f lhs, Vector4f rhs) {
        return lhs.Divide(rhs);
    }

    public static Vector4f operator/(Vector4f lhs, float rhs) {
        return lhs.Divide(rhs);
    }

} // struct Vector4f



public struct Vector4d {

    public double x;
    public double y;
    public double z;
    public double w;


    public Vector4d(double _x, double _y, double _z, double _w) {
        x = _x; y = _y; z = _z; w = _w;
    }

    public Vector4d(Vector4d cpy) {
        x = cpy.x; y = cpy.y; z = cpy.z; w = cpy.w;
    }


    public Vector4d Negate() {
        return new Vector4d(-x, -y, -z, -w);
    }

    public Vector4d Add(Vector4d rhs) {
        return new Vector4d(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    public Vector4d Subtract(Vector4d rhs) {
        return new Vector4d(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    public Vector4d Multiply(Vector4d rhs) {
        return new Vector4d(x * rhs.x, y * rhs.y, z * rhs.z, w*rhs.w);
    }

    public Vector4d Multiply(double s) {
        return new Vector4d(x * s, y * s, z * s, w * s);
    }

    public Vector4d Divide(Vector4d rhs) {
        return new Vector4d(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
    }

    public Vector4d Divide(double s) {
        return new Vector3f(x / s, y / s, z / s, w / s);
    }

    public double Dot(Vector4d rhs) {
        return (x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w);
    }

    public void Normalize() {
        double len = Length;
        if (len != 0.0) {
            x /= len; y /= len; z /= len; w /= len;
        }
    }

    public Vector4d Normalized {
        get {
            Vector4d normed = new Vector4d(this);
            normed.Normalize();
            return normed;
        }
    }

    public double SquaredLength {
        get {
            return (x*x + y*y + z*z + w*w);
        }
    }
    public double Length {
        get {
            return Math.Sqrt(SquaredLength);
        }
    }


    public override string ToString() {
        return String.Format("<{0}, {1}, {2}, {3}>", x, y, z, w);
    }


    public static Vector4d operator-(Vector4d uo) {
        return uo.Negate();
    }

    public static Vector4d operator+(Vector4d lhs, Vector4d rhs) {
        return lhs.Add(rhs);
    }

    public static Vector4d operator-(Vector4d lhs, Vector4d rhs) {
        return lhs.Subtract(rhs);
    }

    public static Vector4d operator*(Vector4d lhs, Vector4d rhs) {
        return lhs.Multiply(rhs);
    }

    public static Vector4d operator*(Vector4d lhs, double rhs) {
        return lhs.Multiply(rhs);
    }
    public static Vector4d operator*(double lhs, Vector4d rhs) {
        return rhs.Multiply(lhs);
    }

    public static Vector4d operator/(Vector4d lhs, Vector4d rhs) {
        return lhs.Divide(rhs);
    }

    public static Vector4d operator/(Vector4d lhs, double rhs) {
        return lhs.Divide(rhs);
    }

} // struct Vector4d



public struct BoundingBox3f3f {
    Vector3f mMin;
    Vector3f mDiag;
    public BoundingBox3f3f(float minx, float miny, float minz,float diagx,float diagy,float diagz) {
        mMin.x=minx;
        mMin.y=miny;
        mMin.z=minz;

        mDiag.x=diagx;
        mDiag.y=diagy;
        mDiag.z=diagz;
    }
    public BoundingBox3f3f(Vector3f min, Vector3f max) {
        mMin.x=min.x;
        mMin.y=min.y;
        mMin.sz=min.z;

        mDiag.x=max.x-min.x;
        mDiag.y=max.y-min.y;
        mDiag.z=max.z-min.z;
    }
    public BoundingBox3f3f(BoundingBox3f3f cpy, Vector3f scale) {
        mMin.x=(float)(cpy.mMinx*scale.x);
        mMin.y=(float)(cpy.mMiny*scale.y);
        mMin.z=(float)(cpy.mMinz*scale.z);
        
        mDiag.x=(float)(cpy.mDiag.x*scale.x);
        mDiag.y=(float)(cpy.mDiag.y*scale.y);
        mDiag.z=(float)(cpy.mDiag.z*scale.z);
    }
    public Vector3f Min {
        get {
            return new Vector3f(mMin.x,mMin.y,mMin.z);
        }
    }
    public Vector3f Max {
        get {
            return new Vector3f(mMin.x+mDiag.x,mMin.y+mDiag.y,mMin.z+mDiag.z);
        }
    }

    public Vector3f Diag {
        get {
            return new Vector3f(mDiag.x,mDiag.y,mDiag.z);
        }
    }


    public override string ToString() {
        return "["+this.Min.ToString()+" - "+this.Max.ToString()+"]";
    }
    public BoundingBox3f3f Merge(BoundingBox3f3f other) {
        Vector3 thisMax=Max;
        Vector3 otherMax=other.Max;
        bool xless=other.mMin.x>mMin.x;
        bool yless=other.mMin.y>mMin.y;
        bool zless=other.mMin.z>mMin.z;

        bool xmore=other.mMax.x<mMax.x;
        bool ymore=other.mMax.y<mMax.y;
        bool zmore=other.mMax.z<mMax.z;
        return new BoundingBox3f3f(xless?mMin.x:other.mMin.x,
                               yless?mMin.y:other.mMin.y,
                               zless?mMin.z:other.mMin.z,
                               xmore?(xless?mDiag.x:otherMax.x-mMin.x):(xless?thisMax.x-other.mMin.x:other.mDiag.x),
                               ymore?(yless?mDiag.y:otherMax.y-mMin.y):(yless?thisMax.y-other.mMin.y:other.mDiag.y),
                               zmore?(zless?mDiag.z:otherMax.z-mMin.z):(zless?thisMax.z-other.mMin.z:other.mDiag.z));
    }

} // struct BoundingBox

public struct BoundingBox3d3f {
    Vector3d mMin;
    Vector3f mDiag;
    public BoundingBox3d3f(double minx, double miny, double minz,float diagx,float diagy,float diagz) {
        mMin.x=minx;
        mMin.y=miny;
        mMin.z=minz;

        mDiag.x=diagx;
        mDiag.y=diagy;
        mDiag.z=diagz;
    }
    public BoundingBox3d3f(Vector3d min, Vector3f max) {
        mMin.x=min.x;
        mMin.y=min.y;
        mMin.sz=min.z;

        mDiag.x=max.x-min.x;
        mDiag.y=max.y-min.y;
        mDiag.z=max.z-min.z;
    }
    public BoundingBox3d3f(BoundingBox3d3f cpy, Vector3d scale) {
        mMin.x=(double)(cpy.mMinx*scale.x);
        mMin.y=(double)(cpy.mMiny*scale.y);
        mMin.z=(double)(cpy.mMinz*scale.z);
        
        mDiag.x=(float)(cpy.mDiag.x*scale.x);
        mDiag.y=(float)(cpy.mDiag.y*scale.y);
        mDiag.z=(float)(cpy.mDiag.z*scale.z);
    }
    public Vector3d Min {
        get {
            return new Vector3d(mMin.x,mMin.y,mMin.z);
        }
    }
    public Vector3d Max {
        get {
            return new Vector3d(mMin.x+mDiag.x,mMin.y+mDiag.y,mMin.z+mDiag.z);
        }
    }

    public Vector3d Diag {
        get {
            return new Vector3d(mDiag.x,mDiag.y,mDiag.z);
        }
    }


    public override string ToString() {
        return "["+this.Min.ToString()+" - "+this.Max.ToString()+"]";
    }
    public BoundingBox3d3f Merge(BoundingBox3d3f other) {
        Vector3d thisMax=Max;
        Vector3d otherMax=other.Max;
        bool xless=other.mMin.x>mMin.x;
        bool yless=other.mMin.y>mMin.y;
        bool zless=other.mMin.z>mMin.z;

        bool xmore=other.mMax.x<mMax.x;
        bool ymore=other.mMax.y<mMax.y;
        bool zmore=other.mMax.z<mMax.z;
        return new BoundingBox3d3f(xless?mMin.x:other.mMin.x,
                               yless?mMin.y:other.mMin.y,
                               zless?mMin.z:other.mMin.z,
                               xmore?(xless?mDiag.x:otherMax.x-mMin.x):(xless?thisMax.x-other.mMin.x:other.mDiag.x),
                               ymore?(yless?mDiag.y:otherMax.y-mMin.y):(yless?thisMax.y-other.mMin.y:other.mDiag.y),
                               zmore?(zless?mDiag.z:otherMax.z-mMin.z):(zless?thisMax.z-other.mMin.z:other.mDiag.z));
    }

} // struct BoundingBox




public struct BoundingSphere3f {
    Vector3f mCenter;
    float mRadius;
    public BoundingSphere3f(float x,float y,float z, float r) {
        mCenter.x=x;
        mCenter.y=y;
        mCenter.z=z;
        mRadius=r;
    }
    public BoundingSphere3f(Vector3f center, float radius) {
        mCenter.x=center.x;
        mCenter.y=center.y;
        mCenter.x=center.z;
        mRadius=radius;
    }
    public BoundingSphere3f(BoundingSphere3f cpy, float scale) {
        mCenter.x=cpy.mCenter.x;
        mCenter.y=cpy.mCenter.y;
        mCenter.z=cpy.mCenter.z;
        mRadius=cpy.mRadius*scale;
    }
    public Vector3f Center {
        get {
            return new Vector3d(mCenter.x,mCenter.y,mCenter.z);
        }
    }
    public float Radius {
        get {
            return mRadius;
        }
    }

    public override string ToString() {
        return "["+this.Center.ToString()+" : "+this.Radius.ToString()+"]";
    }
} // struct BoundingSphere3f



public struct BoundingSphere3d {
    Vector3d mCenter;
    float mRadius;
    public BoundingSphere3d(double x,double y,double z, float r) {
        mCenter.x=x;
        mCenter.y=y;
        mCenter.z=z;
        mRadius=r;
    }
    public BoundingSphere3d(Vector3d center, float radius) {
        mCenter.x=center.x;
        mCenter.y=center.y;
        mCenter.x=center.z;
        mRadius=radius;
    }
    public BoundingSphere3d(BoundingSphere3d cpy, float scale) {
        mCenter.x=cpy.mCenter.x;
        mCenter.y=cpy.mCenter.y;
        mCenter.z=cpy.mCenter.z;
        mRadius=cpy.mRadius*scale;
    }
    public Vector3d Center {
        get {
            return new Vector3d(mCenter.x,mCenter.y,mCenter.z);
        }
    }
    public float Radius {
        get {
            return mRadius;
        }
    }

    public override string ToString() {
        return "["+this.Center.ToString()+" : "+this.Radius.ToString()+"]";
    }
} // struct BoundingSphere3f

public struct UUID {
    ulong mLowOrderBytes;
    ulong mHighOrderBytes;
    public bool SetUUID(byte[]data) {
        if (data.Length==16) {
            mLowOrderBytes=0;
            mHighOrderBytes=0;
            int shiftVal=0;
            for (int i=0;i<8;++i) {
                ulong temp=mData[i];
                mLowOrderBytes|=(temp<<shiftVal);
                shiftVal+=8;
            }
            shiftVal=0;
            for (int i=8;i<16;++i) {
                ulong temp=mData[i];
                mHighOrderBytes|=(temp<<shiftVal);
                shiftVal+=8;                
            }
        }else {
            return false;
        }
    }
    public byte[] GetUUID() {
        byte[] data= new byte[16];
        int shiftVal=0;
        for (int i=0;i<8;++i) {
            ulong temp=0xff;
            temp=(mLowOrderBytes&(temp<<shiftVal));
            temp=(temp>>shiftVal);
            data[i]=(byte)temp;
            shiftVal+=8;
        }
        shiftVal=0;
        for (int i=8;i<16;++i) {
            ulong temp=0xff;
            temp=(mHighOrderBytes&(temp<<shiftVal));
            temp=(temp>>shiftVal);
            data[i]=(byte)temp;
            shiftVal+=8;
        }
    }


    public UUID(byte []data) {
        if (!SetUUID(mData) ) {
            throw new System.ArgumentException("UUIDs must be provided 16 bytes");
        }
    }
}

public class Time {
    ulong usec;
    public Time(ulong usec_since_epoch){
        usec=usec_since_epoch;
    }
}
public class Duration {
    ulong usec;
    public Duration(ulong time_since){
        usec=usec_since_epoch;
    }
}


}
