//
// Created by mecha on 08.01.2022.
//

#include "ScriptCompiler.h"

namespace engine {

//#include "baseclass.h"
//#include <cstdlib>      // EXIT_FAILURE, etc
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <dlfcn.h>      // dynamic library loading, dlopen() etc
//#include <memory>       // std::shared_ptr
//
//// compile code, instantiate class and return pointer to base class
//// https://www.linuxjournal.com/article/3687
//// http://www.tldp.org/HOWTO/C++-dlopen/thesolution.html
//// https://stackoverflow.com/questions/11016078/
//// https://stackoverflow.com/questions/10564670/
//    std::shared_ptr<A> compile(const std::string& code)
//    {
//        // temporary cpp/library output files
//        std::string outpath="/tmp";
//        std::string headerfile="baseclass.h";
//        std::string cppfile=outpath+"/runtimecode.cpp";
//        std::string libfile=outpath+"/runtimecode.so";
//        std::string logfile=outpath+"/runtimecode.log";
//        std::ofstream out(cppfile.c_str(), std::ofstream::out);
//
//        // copy required header file to outpath
//        std::string cp_cmd="cp " + headerfile + " " + outpath;
//        system(cp_cmd.c_str());
//
//        // add necessary header to the code
//        std::string newcode =   "#include \"" + headerfile + "\"\n\n"
//                                + code + "\n\n"
//                                         "extern \"C\" {\n"
//                                         "A* maker()\n"
//                                         "{\n"
//                                         "    return (A*) new B(); \n"
//                                         "}\n"
//                                         "} // extern C\n";
//
//        // output code to file
//        if(out.bad()) {
//            std::cout << "cannot open " << cppfile << std::endl;
//            exit(EXIT_FAILURE);
//        }
//        out << newcode;
//        out.flush();
//        out.close();
//
//        // compile the code
//        std::string cmd = "g++ -Wall -Wextra " + cppfile + " -o " + libfile
//                          + " -O2 -shared -fPIC &> " + logfile;
//        int ret = system(cmd.c_str());
//        if(WEXITSTATUS(ret) != EXIT_SUCCESS) {
//            std::cout << "compilation failed, see " << logfile << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        // load dynamic library
//        void* dynlib = dlopen (libfile.c_str(), RTLD_LAZY);
//        if(!dynlib) {
//            std::cerr << "error loading library:\n" << dlerror() << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        // loading symbol from library and assign to pointer
//        // (to be cast to function pointer later)
//        void* create = dlsym(dynlib, "maker");
//        const char* dlsym_error=dlerror();
//        if(dlsym_error != NULL)  {
//            std::cerr << "error loading symbol:\n" << dlsym_error << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        // execute "create" function
//        // (casting to function pointer first)
//        // https://stackoverflow.com/questions/8245880/
//        A* a = reinterpret_cast<A*(*)()> (create)();
//
//        // cannot close dynamic lib here, because all functions of the class
//        // object will still refer to the library code
//        // dlclose(dynlib);
//
//        return std::shared_ptr<A>(a);
//    }
//
//
//    int main(int argc, char** argv)
//    {
//        double input=2.0;
//        double x=5.1;
//        // code to be compiled at run-time
//        // class needs to be called B and derived from A
//        std::string code =  "class B : public A {\n"
//                            "    double f(double x) const \n"
//                            "    {\n"
//                            "        return m_input*x;\n"
//                            "    }\n"
//                            "};";
//
//        std::cout << "compiling.." << std::endl;
//        std::shared_ptr<A> a = compile(code);
//        a->init(input);
//        std::cout << "f(" << x << ") = " << a->f(x) << std::endl;
//
//        return EXIT_SUCCESS;
//    }#include "baseclass.h"
//#include <cstdlib>      // EXIT_FAILURE, etc
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <dlfcn.h>      // dynamic library loading, dlopen() etc
//#include <memory>       // std::shared_ptr
//
//// compile code, instantiate class and return pointer to base class
//// https://www.linuxjournal.com/article/3687
//// http://www.tldp.org/HOWTO/C++-dlopen/thesolution.html
//// https://stackoverflow.com/questions/11016078/
//// https://stackoverflow.com/questions/10564670/
//    std::shared_ptr<A> compile(const std::string& code)
//    {
//        // temporary cpp/library output files
//        std::string outpath="/tmp";
//        std::string headerfile="baseclass.h";
//        std::string cppfile=outpath+"/runtimecode.cpp";
//        std::string libfile=outpath+"/runtimecode.so";
//        std::string logfile=outpath+"/runtimecode.log";
//        std::ofstream out(cppfile.c_str(), std::ofstream::out);
//
//        // copy required header file to outpath
//        std::string cp_cmd="cp " + headerfile + " " + outpath;
//        system(cp_cmd.c_str());
//
//        // add necessary header to the code
//        std::string newcode =   "#include \"" + headerfile + "\"\n\n"
//                                + code + "\n\n"
//                                         "extern \"C\" {\n"
//                                         "A* maker()\n"
//                                         "{\n"
//                                         "    return (A*) new B(); \n"
//                                         "}\n"
//                                         "} // extern C\n";
//
//        // output code to file
//        if(out.bad()) {
//            std::cout << "cannot open " << cppfile << std::endl;
//            exit(EXIT_FAILURE);
//        }
//        out << newcode;
//        out.flush();
//        out.close();
//
//        // compile the code
//        std::string cmd = "g++ -Wall -Wextra " + cppfile + " -o " + libfile
//                          + " -O2 -shared -fPIC &> " + logfile;
//        int ret = system(cmd.c_str());
//        if(WEXITSTATUS(ret) != EXIT_SUCCESS) {
//            std::cout << "compilation failed, see " << logfile << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        // load dynamic library
//        void* dynlib = dlopen (libfile.c_str(), RTLD_LAZY);
//        if(!dynlib) {
//            std::cerr << "error loading library:\n" << dlerror() << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        // loading symbol from library and assign to pointer
//        // (to be cast to function pointer later)
//        void* create = dlsym(dynlib, "maker");
//        const char* dlsym_error=dlerror();
//        if(dlsym_error != NULL)  {
//            std::cerr << "error loading symbol:\n" << dlsym_error << std::endl;
//            exit(EXIT_FAILURE);
//        }
//
//        // execute "create" function
//        // (casting to function pointer first)
//        // https://stackoverflow.com/questions/8245880/
//        A* a = reinterpret_cast<A*(*)()> (create)();
//
//        // cannot close dynamic lib here, because all functions of the class
//        // object will still refer to the library code
//        // dlclose(dynlib);
//
//        return std::shared_ptr<A>(a);
//    }
//
//
//    int main(int argc, char** argv)
//    {
//        double input=2.0;
//        double x=5.1;
//        // code to be compiled at run-time
//        // class needs to be called B and derived from A
//        std::string code =  "class B : public A {\n"
//                            "    double f(double x) const \n"
//                            "    {\n"
//                            "        return m_input*x;\n"
//                            "    }\n"
//                            "};";
//
//        std::cout << "compiling.." << std::endl;
//        std::shared_ptr<A> a = compile(code);
//        a->init(input);
//        std::cout << "f(" << x << ") = " << a->f(x) << std::endl;
//
//        return EXIT_SUCCESS;
//    }

}