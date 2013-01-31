#include "Selection.h"
#include "DrawFunc.h"

void Selection::calculateSelection(int mouseX, int mouseY)
{
	if (Transformer::selected != -1)
		return;

	std::map<int, Actor *> *actors = SceneManager::getInstance()->getActors();

	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

	float white[] = {1.0,1.0,1.0};
	float black[] = {0,0,0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_EMISSION,black);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float color[3];
	int currentID = 1;
	for (std::map<int, Actor *>::iterator it = actors->begin(); it != actors->end(); ++it){
		currentID = it->first;
		Actor *actor = it->second;
		if ((!SceneManager::getInstance()->isPhysicsObject((*it).first) && SceneManager::getInstance()->getEditMode() == 0) || 
			(SceneManager::getInstance()->isPhysicsObject((*it).first) && SceneManager::getInstance()->getEditMode() == 1)) {
			glPushMatrix();
			MatrixManager::getInstance()->pushMatrix4(MODELVIEW);
				MatrixManager::getInstance()->putMatrix4(MODELVIEW, actor->transformToMatrix(MatrixManager::getInstance()->getMatrix4(MODELVIEW)));
				MatrixManager::getInstance()->putMatrix3(NORMAL, actor->transformToMatrix(MatrixManager::getInstance()->getMatrix3(NORMAL)));
				getColorFromID(currentID,color);
				glColor3f(color[0],color[1],color[2]);
				actor->transform();
				glPushMatrix();
				transformNoShaders();
				ModelManager::getInstance()->drawModelGeometry(*actor->getModel());
			glPopMatrix();
			MatrixManager::getInstance()->popMatrix4(MODELVIEW);
		}
	}

	unsigned char readcolor[3];
	GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(mouseX, viewport[3] - mouseY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, readcolor);
	int selectID = getIDFromColor(readcolor[0], readcolor[1], readcolor[2]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

	if (actors->find(selectID) != actors->end()) {
		SceneManager::getInstance()->setSelectedActor(selectID);
	}
}

void Selection::calculateSelectedTransformer(int mouseX, int mouseY)
{
	Transformer::selected=-1;
	Transformer::lastAngle=0;
	Transformer::dragPoint=Vector3(0,0,0);

	Actor *selectedActor = SceneManager::getInstance()->getSelectedActor();
	if (selectedActor == 0)
		return;
	
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	transformNoShaders();
	glTranslatef(selectedActor->getTranslate()[0], selectedActor->getTranslate()[1], selectedActor->getTranslate()[2]);
	Transformer::draw(true);
	glPopMatrix();

	unsigned char color[3];
	GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(mouseX, viewport[3] - mouseY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
	int selectID = getIDFromColor(color[0], color[1], color[2]);

	if ( (selectID>=100000) && (selectID<100006) ) {
		Transformer::selected=selectID-100000;
	}
}