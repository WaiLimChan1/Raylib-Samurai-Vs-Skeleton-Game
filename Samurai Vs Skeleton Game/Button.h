#pragma once
#include "Header.h"

class Button
{
private:
	static Texture2D StaticSamuraiButtonIcon;
	static Texture2D StaticSamuraiCommanderButtonIcon;
	static Texture2D StaticSamuraiArcherButtonIcon;
	static Texture2D StaticSkeletonWarriorButtonIcon;
	static Texture2D StaticSkeletonSpearmanButtonIcon;
	static Texture2D StaticSkeletonArcherButtonIcon;

	CharacterENUM characterEnum;
	const char* name;
	Texture2D icon;
	Vector2 pos;
	bool facingLeft;
	float scale;

public:
	Button();
	Button(CharacterENUM characterEnum, const char* name, Vector2 pos, bool facingLeft, float scale);

	static void LoadTextures()
	{
		StaticSamuraiButtonIcon = LoadTexture("textures/Samurai/Icon.png");
		StaticSamuraiCommanderButtonIcon = LoadTexture("textures/Samurai_Commander/Icon.png");
		StaticSamuraiArcherButtonIcon = LoadTexture("textures/Samurai_Archer/Icon.png");
		StaticSkeletonWarriorButtonIcon = LoadTexture("textures/Skeleton_Warrior/Icon.png");
		StaticSkeletonSpearmanButtonIcon = LoadTexture("textures/Skeleton_Spearman/Icon.png");
		StaticSkeletonArcherButtonIcon = LoadTexture("textures/Skeleton_Archer/Icon.png");
	}
	
	CharacterENUM getCharacterENUM();
	const char* getName();
	float getWidth();
	float getHeight();
	void setPos(Vector2 pos);
	float getScale();

	Rectangle getHitBox(Vector2 cameraPos);
	void drawHitBox(Vector2 cameraPos, Color color = { 230, 41, 55, 150 });

	bool isClicked(Vector2 cameraPos);
	bool isHeldWithShift(Vector2 cameraPos);

	void draw(Vector2 cameraPos, float borderSize = 6, Color borderColor = BLACK, Color backgroundColor = WHITE, Color imageColor = WHITE);
};