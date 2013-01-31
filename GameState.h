#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
public:
	static GameState *GAMESTATE;

public:
	GameState();

	virtual void resize(int nWidth, int nHeight) = 0;
	virtual void tick(int nFps) = 0;

	virtual bool mouseHidden() = 0;
};

#endif