#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <QGLWidget>
#include "Actor.h"
#include "SceneManager.h"
#include "Matrix.h"
#include "Unproject.h"
#include "Selection.h"

class Transformer {
public:
	static Vector3 dragPoint;
	static float lastAngle;
	static int selected;

	static void drawBoundingBox();
	static void drawTranslator(float *xColor,float *yColor, float *zColor);
	static void drawRotator(float *xColor,float *yColor, float *zColor);
	static void draw(bool selection);
	static void calculateTransform(int mouseX, int mouseY, bool shift, bool ctrl);
};

#endif