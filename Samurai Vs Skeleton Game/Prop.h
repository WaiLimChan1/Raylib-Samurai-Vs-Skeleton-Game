#pragma once
#include "Header.h"

class Prop
{
private:
	Texture2D texture;
	Vector2 pos;
	Vector2 posOffSet;
	bool facingLeft;
	float scale;

public:
	Prop();
	Prop(Texture2D texture, Vector2 pos, Vector2 posOffSet, bool facingLeft, float scale);

	Vector2 getPos();

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	void draw(Vector2 cameraPos, Color colorTint = WHITE);
};