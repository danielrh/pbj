class Test {
    static void assert(bool exp) {
        if (!exp) {
            System.Console.Write("Bad things\n");
        }
    }
    static void Main() {

        Sirikata.PB.TestMessage.Builder tm=Sirikata.PB.TestMessage.CreateBuilder();
        tm.Xxfr=1.25f;
        tm.V3F=new PBJ.Vector3f(1,2,3);
        Sirikata.PB.TestMessage xti=tm.BuildPartial();
        byte[] s=xti.ToByteArray();
        System.Console.Write(s.Length);
        Sirikata.PB.TestMessage ti;
        ti=Sirikata.PB.TestMessage.ParseFrom(s);
        assert(ti!=null);
        assert(ti.V3F.x==1);
        assert(ti.V3F.y==2);
        assert(ti.V3F.z==3);
        assert(ti.Xxfr==1.25);


    }
}