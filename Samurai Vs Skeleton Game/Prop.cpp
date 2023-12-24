#include "Prop.h"

Prop::Prop() {}
Prop::Prop(Texture2D texture, Vector2 pos, Vector2 posOffSet, bool facingLeft, float scale)
	: texture(texture), pos(pos), posOffSet(posOffSet), facingLeft(facingLeft), scale(scale)
{
	//Set it to the ground
	this->pos.y = SCREEN_HEIGHT - texture.height * scale;
	this->pos.y += posOffSet.y;
}

Vector2 Prop::getPos() { return pos; }

Rectangle Prop::getHitBox(Vector2 cameraPos)
{
	return Rectangle{ pos.x - cameraPos.x, pos.y - cameraPos.y, texture.width * scale, texture.height * scale};
}
void Prop::drawHitBox(Vector2 cameraPos, Color color) { DrawRectangleRec(getHitBox(cameraPos), color); }

void Prop::draw(Vector2 cameraPos, Color colorTint)
{
	Rectangle tempSource = { 0,0,texture.width,texture.height };
	if (facingLeft) tempSource.width *= -1;
	DrawTexturePro(texture, tempSource, { pos.x - cameraPos.x, pos.y - cameraPos.y, texture.width * scale, texture.height * scale }, { 0,0 }, 0, colorTint);
}