#ifndef NOSTATE_H
#define NOSTATE_H

#include "GameState.h"

class NoState : public GameState
{
public:
	NoState() {}
	~NoState() {}

	virtual void resize(int w, int h) {}
	virtual void tick(int fps) {}
	virtual bool mouseHidden() {return false;}
};
#endif