#include "output.pbj.hpp"
using namespace Sirikata::PB;
int main (int argc, char**argv) {
    TestMessage tm;
    tm.set_xxfr(1.25);
    tm.set_v3f(PBJ::Vector3f(1,2,3));
    std::string s;
    bool retval=tm.SerializeToString(&s);
    assert(retval);
    std::cout<<s.length();
    TestMessage ti;
    retval=ti.ParseFromString(s);
    assert(retval);
    assert(ti.v3f().x==1);
    assert(ti.v3f().y==2);
    assert(ti.v3f().z==3);
    assert(ti.xxfr()==1.25);
    return 0;
}
