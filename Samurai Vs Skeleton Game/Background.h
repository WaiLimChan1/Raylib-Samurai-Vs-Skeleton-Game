#pragma once
#include "Header.h"

class Background
{
private:
	Texture2D texture;
	Vector2 pos;
	Vector2 posOffSet;
	float scale;

	enum Mode {NONE, ABOVE_GROUND, BELOW_GROUND};
	Mode mode;

public:
	Background();
	Background(Texture2D texture, Vector2 pos, Vector2 posOffSet, float scale);
	
	Vector2 getScaledSize();
	void setPositionToAboveGround(); //Ground being the initial bottom of the SCREEN
	void setPositionToBelowGround(); 

	void draw(Vector2 cameraPos, int backgroundPadding, float distanceRatio, Color tintColor = WHITE);
	void unloadTexture();
};