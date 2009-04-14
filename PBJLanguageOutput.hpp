extern "C" struct LanguageOutputStruct {
    std::ostream* cpp;
    std::ostream* cs;
};

extern "C" struct CsStreams {
    std::stringstream* csType;
    std::stringstream* csBuild;
    std::stringstream* csMembers;
};

