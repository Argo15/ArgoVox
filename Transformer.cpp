#include "Transformer.h"
#include "DrawFunc.h"

int Transformer::selected = -1;
float Transformer::lastAngle = 0;
Vector3 Transformer::dragPoint = Vector3(0,0,0);

void Transformer::drawBoundingBox()
{
	Actor *actor = SceneManager::getInstance()->getSelectedActor();
	float radius = actor->getScaledRadius();
	glColor3f(0.0,0.0,0.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
		glVertex3f(radius,radius,radius);
	glEnd();
}

void Transformer::drawTranslator(float *xColor,float *yColor, float *zColor)
{
	Actor *actor = SceneManager::getInstance()->getSelectedActor();

	glColor3f(0.0,0.0,0.0);
	glLineWidth(8.0);
	float radius = actor->getScaledRadius();

	glBegin(GL_LINES);
		glColor3f(xColor[0],xColor[1],xColor[2]);
		glVertex3f(radius,0,0);
		glVertex3f(radius+max(0.2f,radius),0,0);
		glColor3f(yColor[0],yColor[1],yColor[2]);
		glVertex3f(0,radius,0);
		glVertex3f(0,radius+max(0.2f,radius),0);
		glColor3f(zColor[0],zColor[1],zColor[2]);
		glVertex3f(0,0,radius);
		glVertex3f(0,0,radius+max(0.2f,radius));
	glEnd();
}

void Transformer::drawRotator(float *xColor, float *yColor, float *zColor)
{
	Actor *actor = SceneManager::getInstance()->getSelectedActor();
	glLineWidth(10.0);
	float radius = actor->getScaledRadius();

	glBegin(GL_LINE_STRIP);
		glColor3f(xColor[0],xColor[1],xColor[2]);
		for(int i=0;i<100;i++){
			glVertex3f(0,radius*sinf(i/50.0*PI),radius*cosf(i/50.0*PI));
		}
		glVertex3f(radius*sinf(0),0,radius*cosf(0));
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor3f(yColor[0],yColor[1],yColor[2]);
		for(int i=0;i<100;i++){
			glVertex3f(radius*sinf(i/50.0*PI),0,radius*cosf(i/50.0*PI));
		}
		glVertex3f(0,radius*sinf(0),radius*cosf(0));
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor3f(zColor[0],zColor[1],zColor[2]);
		for(int i=0;i<100;i++){
			glVertex3f(radius*cosf(i/50.0*PI),radius*sinf(i/50.0*PI),0);
		}
		glVertex3f(radius*cosf(0),radius*sinf(0),0);
	glEnd();
}

void Transformer::draw(bool selection)
{
	glDisable(GL_LIGHTING);
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE4);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	float white[] = {1.0,1.0,1.0};
	float black[] = {0,0,0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_EMISSION,black);

	float xColor[3];
	float yColor[3];
	float zColor[3];
	if (!selection) {
		xColor[0]=1.0;xColor[1]=0.0;xColor[2]=0.0;
		yColor[0]=0.0;yColor[1]=1.0;yColor[2]=0.0;
		zColor[0]=0.0;zColor[1]=0.0;zColor[2]=1.0;
		switch (selected) {
			case 0: xColor[1]=1.0; break;
			case 1: yColor[0]=1.0; break;
			case 2: zColor[0]=1.0; zColor[1]=1.0; zColor[2]=0.0; break;
		}
	} else {
		getColorFromID(100000,xColor);
		getColorFromID(100001,yColor);
		getColorFromID(100002,zColor);
	}
	drawTranslator(xColor,yColor,zColor);
	drawBoundingBox();
	if (!selection) {
		xColor[0]=1.0;xColor[1]=0.0;xColor[2]=0.0;
		yColor[0]=0.0;yColor[1]=1.0;yColor[2]=0.0;
		zColor[0]=0.0;zColor[1]=0.0;zColor[2]=1.0;
		switch (selected) {
			case 3: xColor[1]=1.0; break;
			case 4: yColor[0]=1.0; break;
			case 5: zColor[0]=1.0; zColor[1]=1.0; zColor[2]=0.0; break;
		}
	} else {
		getColorFromID(100003,xColor);
		getColorFromID(100004,yColor);
		getColorFromID(100005,zColor);
	}
	drawRotator(xColor,yColor,zColor);
}

void Transformer::calculateTransform(int mouseX, int mouseY, bool shift, bool ctrl)
{
	Actor *actor = SceneManager::getInstance()->getSelectedActor();
	if (actor == 0 || selected == -1) {
		dragPoint = Vector3(0,0,0);
		return;
	}

	Vector3 unProjLine[2];
	Vector3 dragLine[2];

	float *center = actor->getTranslate();
	
	unProjLine[0] = getUnproject(0,mouseX,mouseY);
	unProjLine[1] = getUnproject(1,mouseX,mouseY)-unProjLine[0];
	dragLine[0] = Vector3(center[0],center[1],center[2]);

	if (selected<3) {
		switch (selected) {
			case 0: dragLine[1] = Vector3(1.0,0.0,0.0); break;
			case 1: dragLine[1] = Vector3(0.0,1.0,0.0); break;
			case 2: dragLine[1] = Vector3(0.0,0.0,1.0); break;
			default: return;
		}

		Vector3 newDragPoint = getDragPoint(dragLine,unProjLine);
		Vector3 translate = newDragPoint-dragPoint;

		if (dragPoint.length() > 0.0001){
			if (ctrl) {
				float scale = (translate[0]+translate[1]+translate[2])/2.0;
				actor->scale(scale,scale,scale);
			} else if (shift) {
				translate = translate * 0.5;
				actor->scale(translate[0],translate[1],translate[2]);
			} else {
				actor->translate(translate[0],translate[1],translate[2]);
			}
		}
		dragPoint=newDragPoint;
	} else {
		float newAngle;
		float angleChange;
		Vector3 normal;
		Vector3 tangent;

		switch (selected) {
			case 3: normal = Vector3(1.0,0.0,0.0); tangent = Vector3(0,0,1); break;
			case 4: normal = Vector3(0.0,1.0,0.0); tangent = Vector3(0,0,1); break;
			case 5: normal = Vector3(0.0,0.0,1.0); tangent = Vector3(1,0,0); break;
			default: return;
		}
		
		Vector3 vcenter=Vector3(center[0],center[1],center[2]);
		unProjLine[0]-=vcenter;
		newAngle = getRotAngle(unProjLine,&normal,&tangent);

		angleChange=newAngle-lastAngle;
		if (lastAngle==0){
			angleChange=0;
		}
		lastAngle = newAngle;

		Quaternion quat;
		quat.createQuaternion(angleChange,normal[0],normal[1],normal[2]);
		actor->rotate(quat);
	}
}