#include "Background.h"

Background::Background() {}
Background::Background(Texture2D texture, Vector2 pos, Vector2 posOffSet, float scale)
	: texture(texture), pos(pos), posOffSet(posOffSet), scale(scale) 
{
	this->pos = Vector2Add(pos, posOffSet);
	mode = NONE;
}

Vector2 Background::getScaledSize() { return { texture.width * scale, texture.height * scale }; }
void Background::setPositionToAboveGround() 
{ 
	mode = ABOVE_GROUND;
	pos = { pos.x, SCREEN_HEIGHT - getScaledSize().y }; 
	pos = Vector2Add(pos, posOffSet);
}

void Background::setPositionToBelowGround()
{
	mode = BELOW_GROUND;
	pos = { pos.x, SCREEN_HEIGHT };
	pos = Vector2Add(pos, posOffSet);
}

//Several of the same background texture is used to create the illusion that the background
//spans infinitely. backgroundPadding determines the number of background textures before and after the main background texture.
//The bigger the distance ratio, the further away
void Background::draw(Vector2 cameraPos, int backgroundPadding, float distanceRatio, Color tintColor)
{
	//Recalculate the camera pos based on the distance ratio.
	Vector2 distancedCameraPos = { cameraPos.x / distanceRatio, cameraPos.y / distanceRatio };

	//If camera position touches the right end of the original background, reset background
	if (distancedCameraPos.x >= texture.width * scale + pos.x)
	{
		pos = { distancedCameraPos.x, pos.y };
		if (mode == ABOVE_GROUND) setPositionToAboveGround();
		if (mode == BELOW_GROUND) setPositionToBelowGround();
	}

	//If camera position touches the left end of the background that is immediately left of the original background, reset background
	if (distancedCameraPos.x <= -texture.width * scale + pos.x)
	{
		pos = { distancedCameraPos.x, pos.y };
		if (mode == ABOVE_GROUND) setPositionToAboveGround();
		if (mode == BELOW_GROUND) setPositionToBelowGround();
	}

	//Draw consecutive background textures starting with the leftmost one. 
	Vector2 temp{ pos.x - backgroundPadding * texture.width * scale, pos.y };
	for (int i = 0; i < backgroundPadding * 2 + 1; i++)
	{
		DrawTextureEx(texture, { temp.x - (int) distancedCameraPos.x, temp.y - (int) distancedCameraPos.y }, 0, scale, tintColor);
		temp.x += texture.width * scale;
	}
}

void Background::unloadTexture() { UnloadTexture(texture); }