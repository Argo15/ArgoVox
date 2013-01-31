#ifndef AOBJMODEL_H
#define AOBJMODEL_H

#include "StaticModel.h"
#include <GL/gl.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include <iostream>
#include <fstream>
using namespace std;

class AObjModel : public StaticModel
{
public:
	bool load(string sFileName);
};

#endif