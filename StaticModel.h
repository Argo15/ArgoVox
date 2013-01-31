#ifndef STATICMODEL_H
#define STATICMODEL_H

#include "Model.h"
#include <string>
using namespace std;

class StaticModel : public Model
{
protected:
	GLuint m_nBufferID;
public:
	virtual bool load(string sFilename) = 0;
	void remove();
	void draw();
	void drawGeometry();
	void drawSelection();
};

#endif