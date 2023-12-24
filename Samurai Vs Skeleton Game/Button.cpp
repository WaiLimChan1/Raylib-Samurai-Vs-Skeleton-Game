#include "Button.h"

Texture2D Button::StaticSamuraiButtonIcon;
Texture2D Button::StaticSamuraiCommanderButtonIcon;
Texture2D Button::StaticSamuraiArcherButtonIcon;
Texture2D Button::StaticSkeletonWarriorButtonIcon;
Texture2D Button::StaticSkeletonSpearmanButtonIcon;
Texture2D Button::StaticSkeletonArcherButtonIcon;

Button::Button() {}
Button::Button(CharacterENUM characterEnum, const char* name, Vector2 pos, bool facingLeft, float scale)
	: characterEnum(characterEnum), name(name), pos(pos), facingLeft(facingLeft), scale(scale)
{
	if (characterEnum == SAMURAI) icon = StaticSamuraiButtonIcon;
	else if (characterEnum == SAMURAI_COMMANDER) icon = StaticSamuraiCommanderButtonIcon;
	else if (characterEnum == SAMURAI_ARCHER) icon = StaticSamuraiArcherButtonIcon;
	else if (characterEnum == SKELETON_WARRIOR) icon = StaticSkeletonWarriorButtonIcon;
	else if (characterEnum == SKELETON_SPEARMAN) icon = StaticSkeletonSpearmanButtonIcon;
	else if (characterEnum == SKELETON_ARCHER) icon = StaticSkeletonArcherButtonIcon;
}

CharacterENUM Button::getCharacterENUM() { return characterEnum; }
const char* Button::getName() { return name; }
float Button::getWidth() { return icon.width * scale; }
float Button::getHeight() { return icon.height * scale; }
void Button::setPos(Vector2 pos) { this->pos = pos; }
float Button::getScale() { return scale; }

Rectangle Button::getHitBox(Vector2 cameraPos)
{
	float hitBoxWidth = (icon.width * scale) * 3 / 4;
	float hitBoxHeight = (icon.height * scale) * 2 / 3;
	float hitBoxPosX = pos.x - cameraPos.x + hitBoxWidth * 1/6;
	float hitBoxPosY = pos.y - cameraPos.y + hitBoxHeight / 2;
	return Rectangle{ hitBoxPosX, hitBoxPosY, hitBoxWidth, hitBoxHeight };
}
void Button::drawHitBox(Vector2 cameraPos, Color color)
{
	DrawRectangleRec(getHitBox(cameraPos), color);
}

bool Button::isClicked(Vector2 cameraPos)
{
	return IsMouseButtonPressed(0) && CheckCollisionPointRec(GetMousePosition(), getHitBox(cameraPos));
}
bool Button::isHeldWithShift(Vector2 cameraPos)
{
	return IsMouseButtonDown(0) && IsKeyDown(KEY_LEFT_SHIFT) && CheckCollisionPointRec(GetMousePosition(), getHitBox(cameraPos));
}

void Button::draw(Vector2 cameraPos, float borderSize, Color borderColor, Color backgroundColor, Color imageColor)
{
	//Draw border
	Rectangle hitBox = getHitBox(cameraPos);
	DrawRectangle(hitBox.x - borderSize, hitBox.y - borderSize, hitBox.width + borderSize * 2, hitBox.height + borderSize * 2, borderColor);

	//Draw background
	DrawRectangleRec(hitBox, backgroundColor);

	//Draw image
	Rectangle tempSource = { 0,0,icon.width,icon.height };
	if (facingLeft) tempSource.width *= -1;
	DrawTexturePro(icon, tempSource, { pos.x - cameraPos.x, pos.y - cameraPos.y, icon.width * scale, icon.height * scale }, { 0,0 }, 0, imageColor);
}