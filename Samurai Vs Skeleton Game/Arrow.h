#pragma once
#include "Header.h"

class Arrow
{
private:
	Texture texture;
	float scale;

	Vector2 pos;

	bool facingLeft;

	float damage;

	float speed;
	float traveledDistance;
	float totalRange;

public:
	Arrow();
	Arrow(Texture texture, float scale, Vector2 pos, bool facingLeft, float damage, float speed, float totalRange);

	bool getFacingLeft();
	float getDamage();

	Rectangle getAttackBox(Vector2 cameraPos);
	void drawAttackbox(Vector2 cameraPos, Color color = { 255, 161, 0, 150 });

	void draw(Vector2 cameraPos, Color color = WHITE);

	bool reachedMaxRange();
	void updatePosition();
	void checkBound();
};