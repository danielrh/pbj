#include <util/Platform.hpp>
#include "output.pbj.hpp"
using namespace Sirikata;
using namespace Sirikata::PB;
int main (int argc, char**argv) {
    TestMessage tm;
    tm.set_xxfr(1.25);
    tm.set_v3f(PBJ::Vector3f(1,2,3));
    tm.add_submessers().set_subvector(PBJ::Vector3d(2,3,4));
    tm.mutable_submes().set_subvector(PBJ::Vector3d(5,6,7));
    tm.mutable_extmesser().mutable_sub_mes().set_subvector(PBJ::Vector3d(8,9,10));
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
    assert(ti.submessers(0).subvector().x==2);
    assert(ti.submessers(0).subvector().y==3);
    assert(ti.submessers(0).subvector().z==4);
    assert(ti.submes().subvector().x==5);
    assert(ti.submes().subvector().y==6);
    assert(ti.submes().subvector().z==7);
    assert(ti.extmesser().sub_mes().subvector().x==8);
    assert(ti.extmesser().sub_mes().subvector().y==9);
    assert(ti.extmesser().sub_mes().subvector().z==10);
    Content c;
    c.mutable_c().mutable_vec().set_p(Sirikata::Vector3f(1,2,3));
    c.mutable_c().mutable_vec().set_t(Sirikata::Time::epoch());
    std::string cstr;
    c.SerializeToString(&cstr);
    Content cc;
    cc.ParseFromString(cstr);
    Stuff ss=cc.c();
    TimedVector itv=ss.vec();
    return 0;
}

