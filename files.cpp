#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <GL/gl.h>
#include "shader.h"
#include "files.h"

using namespace boost::filesystem;

extern std::vector<Shader*> vertex_list;
extern std::vector<Shader*> fragment_list;

void
clearLoadedShaders()
{
    typedef std::vector<Shader*>::iterator sit;
    for(sit i(vertex_list.begin()); i != vertex_list.end();++i) {
        (*i)->clean();
        delete (*i);
    }
    vertex_list.clear();
    for(sit i(fragment_list.begin()); i != fragment_list.end();++i) {
        (*i)->clean();
        delete (*i);
    }
    fragment_list.clear();
}

int
vertex_count()
{
    return vertex_list.size();
}

int
fragment_count()
{
    return fragment_list.size();
}

void
ShaderSourceLoader()
{
    clearLoadedShaders();
    path p("./shaders/");
    
    if(exists(p)) {
        typedef std::vector<path> vec;
        vec v;
        copy(directory_iterator(p), directory_iterator(), back_inserter(v));
        sort(v.begin(), v.end());
        for(vec::const_iterator it (v.begin()); it != v.end(); ++it)
        {
            Shader *sh = 0;
            path h = *it;
            std::cout << h.native() << std::endl;
            if(h.extension().string() == std::string(".vs")) {
                sh = loadShader(h.native(), GL_VERTEX_SHADER);
                if(sh) vertex_list.push_back(sh);
                sh = 0;
            }
            
            if(h.extension().string() == std::string(".ps")) {
                sh = loadShader(h.native(), GL_FRAGMENT_SHADER);
                if(sh) fragment_list.push_back(sh);
                sh = 0;
            }
        }
    }
    std::cout << vertex_list.size() << "# vertex shaders" << std::endl;
    std::cout << fragment_list.size() << "# fragment shaders" << std::endl;
}

