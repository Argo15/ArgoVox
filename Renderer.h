#ifndef RENDERER_H
#define RENDERER_H

class Renderer 
{
public:
	virtual void init() = 0;
	virtual void resize(int nWidth, int nHeight) = 0;
	virtual void render() = 0;
};

#endif