#include "Arrow.h"

Arrow::Arrow() {}
Arrow::Arrow(Texture texture, float scale, Vector2 pos, bool facingLeft, float damage, float speed, float totalRange)
	: texture(texture), scale(scale), pos(pos), facingLeft(facingLeft), damage(damage), speed(speed), totalRange(totalRange)
{
	traveledDistance = 0;
}

bool Arrow::getFacingLeft() { return facingLeft; }
float Arrow::getDamage() { return damage; }

Rectangle Arrow::getAttackBox(Vector2 cameraPos)
{
	float attackBoxWidth = texture.width * scale / 3;
	float attackBoxHeight = texture.height * scale / 3;
	float attackBoxPosX = pos.x - cameraPos.x;
	float attackBoxPosY = pos.y - cameraPos.y + attackBoxHeight;
	if (!facingLeft) attackBoxPosX += attackBoxWidth * 2;

	return Rectangle{ attackBoxPosX, attackBoxPosY, attackBoxWidth, attackBoxHeight };
}
void Arrow::drawAttackbox(Vector2 cameraPos, Color color) { DrawRectangleRec(getAttackBox(cameraPos), color); }

void Arrow::draw(Vector2 cameraPos, Color color)
{
	Rectangle tempSource = {0,0,texture.width,texture.height};
	if (facingLeft) tempSource.width *= -1;
	DrawTexturePro(texture, tempSource, { pos.x - cameraPos.x, pos.y - cameraPos.y, texture.width * scale, texture.height * scale}, { 0,0 }, 0, color);
}

bool Arrow::reachedMaxRange()
{
	return traveledDistance >= totalRange;
}

void Arrow::updatePosition()
{
	if (!reachedMaxRange())
	{
		float dt = GetFrameTime();
		if (facingLeft) pos.x -= speed * dt;
		else pos.x += speed * dt;
		traveledDistance += speed * dt;
	}
}
void Arrow::checkBound()
{

}