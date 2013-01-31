#ifndef SELECTION_H
#define SELECTION_H

#include "SceneManager.h"
#include "Transformer.h"
#include "Camera.h"

static void getColorFromID(int id, float *color)
{
	color[0]=(float)(id%255)/255.0;
	color[1]=(float)((id/255)%255)/255.0;
	color[2]=(float)((id/(255*255))%255)/255.0;
}

static int getIDFromColor(float r, float g, float b)
{
	int iR = (int)(r);
	int iG = (int)(g);
	int ib = (int)(b);
	int selectID = iR+iG*255+ib*255*255;
	return selectID;
}

class Selection {
public:
	static void calculateSelection(int mouseX, int mouseY);
	static void calculateSelectedTransformer(int mouseX, int mouseY);
};

#endif