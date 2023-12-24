#include "Header.h"

#include "MyCamera.h"
#include "Background.h"
#include "Prop.h"
#include "Effect.h"
#include "Portal.h"
#include "Button.h"

#include "Samurai.h"
#include "SamuraiCommander.h"
#include "SamuraiArcher.h"

#include "SkeletonWarrior.h"
#include "SkeletonSpearman.h"
#include "SkeletonArcher.h"

//Global Variables
//-----------------------------------------------------------------------------
Vector2 mapVerticalBound; // x - low, y - high
Vector2 mapHorizontalBound = { 0, 5000 }; // x - left, y - right

MyCamera camera;

Background background;
Background middleGround;
Background ground;

std::vector<Prop*> houseProps;
std::vector<Prop*> smallProps;


std::vector<Button*> spawnButtons;
std::vector<Button*> upgradeButtons;


Portal* allyPortal;
int allyMoney;
std::vector<Character*> allyCharacters;
std::vector<Character*> allyDeadCharacters;
std::vector<Arrow*> allyArrows;

Portal* enemyPortal;
int enemyMoney;
std::vector<Character*> enemyCharacters;
std::vector<Character*> enemyDeadCharacters;
std::vector<Arrow*> enemyArrows;

int roundLevel;

float passiveIncomeRate;
float timeSinceLastPassiveIncome;

float enemySpawnRate;
float enemySpawnRateSpeedUp;
float enemyTimeSinceLastSpawn;

CharacterStats* bossStats;
float enemyBossStatsMultipler;
float maxBossScale;



int maxDeadBodiesAmount;
std::vector<Effect*> specialEffects;

SelectionModeENUM selectionMode;
Vector2 firstSelectionRecPos, secondSelectionRecPos;
Rectangle selectionRectangle;
std::vector<int> targetIndices;
int targetIndex;


float animationSpeed = 1.0f / 12.0f;
bool drawHitBoxes;
bool drawAttackBoxes;
//-----------------------------------------------------------------------------

void SetUpLoadTextures()
{
	Samurai::LoadTextures();
	SamuraiCommander::LoadTextures();
	SamuraiArcher::LoadTextures();

	SkeletonWarrior::LoadTextures();
	SkeletonSpearman::LoadTextures();
	SkeletonArcher::LoadTextures();

	Button::LoadTextures();
}

bool collideWithProps(Prop* newProp, std::vector<Prop*>& props)
{
	for (auto& prop : props)
	{
		if (CheckCollisionRecs(newProp->getHitBox(camera.getPos()), prop->getHitBox(camera.getPos())))
			return true;
	}
	return false;
}

void createProps(std::vector<Texture2D>& possibleTextures, std::vector<Prop*>& props, int buffer, int onePerPixel, Vector2 propPosOffSet, bool facingLeft, float scale)
{
	Vector2 bound = { mapHorizontalBound.x - buffer, mapHorizontalBound.y + buffer };
	int numOfProps = (bound.y - bound.x) / 300;

	//Create the small props
	for (int i = 0; i < numOfProps; i++)
	{
		Prop* prop;
		int counter = 0;
		do
		{
			Vector2 propPos = { (float)(rand() % (int)(bound.y - bound.x) + bound.x), 0 };
			prop = new Prop(possibleTextures.at(rand() % possibleTextures.size()), propPos, propPosOffSet, facingLeft, scale);

			counter++;
			if (counter > 10) break;
		} while (collideWithProps(prop, props));

		if (counter <= 10)
			props.push_back(prop);
	}
}

void sortProps(std::vector<Prop*>& props)
{
	//Sort the small props. Leftmost props are drawn first
	for (int i = 0; i < props.size() - 1; i++)
	{
		int smallestIndex = i;
		float smallestPosX = props.at(i)->getPos().x;
		for (int j = i + 1; j < props.size(); j++)
		{
			if (props.at(j)->getPos().x < smallestPosX)
			{
				smallestIndex = j;
				smallestPosX = props.at(j)->getPos().x;
			}
		}

		Prop* temp = props.at(i);
		props.at(i) = props.at(smallestIndex);
		props.at(smallestIndex) = temp;
	}
}

void SetUpProps()
{
	bool propFacingLeft = false;
	float propScale = 3.0;


	std::vector<Texture2D> possibleSmallPropTextures;
	possibleSmallPropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/barrel.png"));
	possibleSmallPropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/crate.png"));
	possibleSmallPropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/crate-stack.png"));
	possibleSmallPropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/street-lamp.png"));
	possibleSmallPropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/wagon.png"));
	possibleSmallPropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/well.png"));
	//possibleTexture.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/sign.png"));

	createProps(possibleSmallPropTextures, smallProps, 1000, 300, { 0, -5 }, propFacingLeft, propScale);
	sortProps(smallProps);


	std::vector<Texture2D> possibleHousePropTextures;
	possibleHousePropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/house-a.png"));
	possibleHousePropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/house-b.png"));
	possibleHousePropTextures.push_back(LoadTexture("textures/Backgrounds/GothicVania Environment/props-sliced/house-c.png"));

	createProps(possibleHousePropTextures, houseProps, 1000, 300, { 0, -10 }, propFacingLeft, propScale);
	sortProps(houseProps);
}

void SetUpBackground()
{
	float speed = 800.0f;
	bool locked = true;
	camera = MyCamera({ 0,0 }, speed, locked);

	background = Background(LoadTexture("textures/Backgrounds/GothicVania Environment/layers/background.png"), { 0, 0 }, { 0,-150 }, 3.3f);
	background.setPositionToAboveGround();

	middleGround = Background(LoadTexture("textures/Backgrounds/GothicVania Environment/layers/middleground.png"), { 0,0 }, { 0, 0 }, 2.8f);
	middleGround.setPositionToAboveGround();

	ground = Background(LoadTexture("textures/Backgrounds/GothicVania Environment/layers/floor.png"), { 0,0 }, { 0, -40 }, 2.8f);
	ground.setPositionToBelowGround();

	mapVerticalBound = { (SCREEN_HEIGHT - background.getScaledSize().y) * 3, 200 };

	SetUpProps();
}

void SetUpPortals()
{
	float portalHealth = 500;
	float portalScale = 8.0f;
	allyPortal = new Portal(portalHealth, { 0,0 }, false, portalScale);
	allyPortal->getPortalEffect().setPos({ mapHorizontalBound.x, SCREEN_HEIGHT - allyPortal->getPortalEffect().getHeight() });

	enemyPortal = new Portal(portalHealth, { 0,0 }, true, portalScale);
	enemyPortal->getPortalEffect().setPos({ mapHorizontalBound.y - enemyPortal->getPortalEffect().getWidth(), SCREEN_HEIGHT - enemyPortal->getPortalEffect().getHeight() });
}

void SetUpSpawnButtonsAndUpgradeButtons()
{
	Vector2 pos;
	pos.x = 0;
	pos.y = SCREEN_HEIGHT;

	bool facingLeft = false;
	float scale = 0.72f;

	spawnButtons.push_back(new Button(SAMURAI, "Samurai", { 0, 0 }, facingLeft, scale));
	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth() / 2;
	pos.y -= spawnButtons.at(spawnButtons.size() - 1)->getHeight() * 5 / 4;
	spawnButtons.at(spawnButtons.size() - 1)->setPos(pos);
	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth();
	upgradeButtons.push_back(new Button(SAMURAI, "Samurai", pos, facingLeft, scale));

	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth() * 3 / 2;
	spawnButtons.push_back(new Button(SAMURAI_COMMANDER, "Samurai Commander", pos, facingLeft, scale));
	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth();
	upgradeButtons.push_back(new Button(SAMURAI_COMMANDER, "Samurai Commander", pos, facingLeft, scale));

	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth() * 3 / 2;
	spawnButtons.push_back(new Button(SAMURAI_ARCHER, "Samurai Archer", pos, facingLeft, scale));
	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth();
	upgradeButtons.push_back(new Button(SAMURAI_ARCHER, "Samurai Archer", pos, facingLeft, scale));

	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth() * 3 / 2;
	spawnButtons.push_back(new Button(SKELETON_WARRIOR, "Skeleton Warrior", pos, facingLeft, scale));
	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth();
	upgradeButtons.push_back(new Button(SKELETON_WARRIOR, "Skeleton Warrior", pos, facingLeft, scale));

	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth() * 3 / 2;
	spawnButtons.push_back(new Button(SKELETON_SPEARMAN, "Skeleton Spearman", pos, facingLeft, scale));
	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth();
	upgradeButtons.push_back(new Button(SKELETON_SPEARMAN, "Skeleton Spearman", pos, facingLeft, scale));

	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth() * 3 / 2;
	spawnButtons.push_back(new Button(SKELETON_ARCHER, "Skeleton Archer", pos, facingLeft, scale));
	pos.x += spawnButtons.at(spawnButtons.size() - 1)->getWidth();
	upgradeButtons.push_back(new Button(SKELETON_ARCHER, "Skeleton Archer", pos, facingLeft, scale));
}

void SetUpInitalCharacters()
{
	//Ally--------------------------------------------------------------------------
	allyCharacters.push_back(allyPortal->createCharacter(SAMURAI, "Samurai", animationSpeed));
	allyCharacters.push_back(allyPortal->createCharacter(SAMURAI_COMMANDER, "Samurai Commander", animationSpeed));
	allyCharacters.push_back(allyPortal->createCharacter(SAMURAI_ARCHER, "Samurai Archer", animationSpeed));
	//Ally--------------------------------------------------------------------------

	//Enemy--------------------------------------------------------------------------
	enemyCharacters.push_back(enemyPortal->createCharacter(SKELETON_WARRIOR, "Skeleton Warrior", animationSpeed));
	enemyCharacters.push_back(enemyPortal->createCharacter(SKELETON_SPEARMAN, "Skeleton Spearman", animationSpeed));
	enemyCharacters.push_back(enemyPortal->createCharacter(SKELETON_ARCHER, "Skeleton Archer", animationSpeed));
	//Enemy--------------------------------------------------------------------------
}

void PrimarySetUp()
{
	SetUpLoadTextures();
	Effect::LoadAnimations();
	SetUpBackground();
}

void SetUp()
{
	SetUpPortals();
	SetUpSpawnButtonsAndUpgradeButtons();

	//SetUpInitalCharacters();
	allyMoney = 50;
	enemyMoney = 0;

	maxDeadBodiesAmount = 30;

	roundLevel = 1;

	passiveIncomeRate = 5;
	timeSinceLastPassiveIncome = 0;

	enemySpawnRate = 20;
	enemySpawnRateSpeedUp = 0.9;
	enemyTimeSinceLastSpawn = enemySpawnRate;

	bossStats = new CharacterStats();
	bossStats->setLevel(1);
	enemyBossStatsMultipler = 1.1;
	maxBossScale = 12;

	selectionMode = NONE;
	firstSelectionRecPos = { 0,0 };
	secondSelectionRecPos = { 0,0 };
	selectionRectangle = { 0,0,0,0 };
	targetIndex = -1;

	drawHitBoxes = false;
	drawAttackBoxes = false;
}

void DrawBackground()
{
	ClearBackground({ 28, 12, 36, 255 });
	background.draw(camera.getPos(), 2, 3.0f);
	middleGround.draw(camera.getPos(), 2, 1.7f);
	ground.draw(camera.getPos(), 2, 1.0f);

	//Purple rectangle below the ground to conceal the backgrounds if the camera moves too low.
	DrawRectangle(0, SCREEN_HEIGHT + ground.getScaledSize().y / 2 - camera.getPos().y, SCREEN_WIDTH, 1000, { 28, 12, 36, 255 });

	for (auto& prop : houseProps) prop->draw(camera.getPos());
	for (auto& prop : smallProps) prop->draw(camera.getPos());
}

void DrawPortals(bool drawHitboxes, int healthBarSize = 200, Color allyPortalCollor = BLUE, Color enemyPortalColor = RED)
{
	if (drawHitboxes) allyPortal->drawHitBox(camera.getPos());
	allyPortal->draw(camera.getPos(), allyPortalCollor);
	allyPortal->drawHealthBar(camera.getPos(), healthBarSize);

	if (drawHitboxes) enemyPortal->drawHitBox(camera.getPos());
	enemyPortal->draw(camera.getPos(), enemyPortalColor);
	enemyPortal->drawHealthBar(camera.getPos(), healthBarSize);
}

//Finding the average of two colors
Color combineColors(Color color1, Color color2)
{
	Color resultColor;
	resultColor.r = (color1.r + color2.r) / 2.0;
	resultColor.g = (color1.g + color2.g) / 2.0;
	resultColor.b = (color1.b + color2.b) / 2.0;
	resultColor.a = (color1.a + color2.a) / 2.0;
	return resultColor;
}

void DrawCharacters(std::vector<Character*>& characters, bool drawHitboxes, bool drawAttackBoxes, Color tintColor = WHITE, Color hitColor = RED)
{
	//Hitboxes
	if (drawHitboxes)
		for (auto& character : characters)
			character->drawHitBox(camera.getPos());

	//Drawing characters
	for (auto& character : characters)
		character->draw(camera.getPos(), tintColor, hitColor);

	//Attackboxes
	if (drawAttackBoxes)
		for (auto& character : characters)
			character->drawAttackbox(camera.getPos());
}

void DrawCharactersWithSelectionHover(std::vector<Character*>& characters, Rectangle selectionRectangle, bool drawHitboxes, bool drawAttackBoxes, Color tintColor = WHITE, Color hitColor = RED, Color hoverColor = BLACK)
{
	//Hitboxes
	if (drawHitboxes)
		for (auto& character : characters)
			character->drawHitBox(camera.getPos());

	//Finding the mouse hover index
	int mouseHoverIndex = -1;
	for (int i = characters.size() - 1; i >= 0; i--)
	{
		if (CheckCollisionPointRec(GetMousePosition(), characters.at(i)->getHitBox(camera.getPos()))) //Mouse hovering over
		{
			mouseHoverIndex = i;
			break;
		}
	}

	//Drawing characters
	for (int i = 0; i < characters.size(); i++)
	{
		if (i == mouseHoverIndex)//Mouse hovering over current character
			characters.at(i)->draw(camera.getPos(), combineColors(tintColor, hoverColor), combineColors(hitColor, hoverColor));
		else if (CheckCollisionRecs(characters.at(i)->getHitBox({0,0}), selectionRectangle) //Selection Rectangle hovering over character
			&& abs(selectionRectangle.width) > 5 && abs(selectionRectangle.height > 5)) //Add a little buffer to differentiate between rectangle selection and mouse selection
			characters.at(i)->draw(camera.getPos(), combineColors(tintColor, hoverColor), combineColors(hitColor, hoverColor));
		else
			characters.at(i)->draw(camera.getPos(), tintColor, hitColor);
	}

	//Attackboxes
	if (drawAttackBoxes)
		for (auto& character : characters)
			character->drawAttackbox(camera.getPos());
}

void PrintSelectEntireTroopTypeGuide(Vector2 initialPos, int fontSize = 30, float rightSideBuffer = 10, Color textColor = YELLOW)
{
	char message[100];
	float textWidth;
	int posX = initialPos.x;
	int posY = initialPos.y;

	strcpy_s(message, "1 - SELECT ALL");
	textWidth = MeasureText(message, fontSize);
	posX = SCREEN_WIDTH / 2 - textWidth * 3/2;
	DrawText(message, posX, posY, fontSize, textColor);

	strcpy_s(message, "2 - SELECT ALL SAMURAI");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, textColor);

	strcpy_s(message, "3 - SELECT ALL SAMURAI COMMANDER");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, textColor);

	strcpy_s(message, "4 - SELECT ALL SAMURAI ARCHER");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, textColor);

	strcpy_s(message, "5 - SELECT ALL SKELETON WARRIOR");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, textColor);

	strcpy_s(message, "6 - SELECT ALL SKELETON SPEARMAN");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, textColor);

	strcpy_s(message, "7 - SELECT ALL SKELETON ARCHER");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, textColor);
}

void PrintCommandGuide(Vector2 initialPos, int fontSize = 30, float rightSideBuffer = 10, Color textColor = YELLOW)
{
	char message[100];
	float textWidth;
	int posX = initialPos.x;
	int posY = initialPos.y;

	strcpy_s(message, "1 - RED: NONE COMMAND");
	textWidth = MeasureText(message, fontSize);
	posX = SCREEN_WIDTH / 2 - textWidth / 2;
	DrawText(message, posX, posY, fontSize, RED);

	strcpy_s(message, "2 - ORANGE: RANDOM COMMAND");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, ORANGE);

	strcpy_s(message, "3 - YELLOW: HOLD COMMAND");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, YELLOW);

	strcpy_s(message, "4 - GREEN: ATTACK COMMAND");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, GREEN);

	strcpy_s(message, "5 - BLUE: CHARGE COMMAND");
	textWidth = MeasureText(message, fontSize);
	posY += fontSize;
	DrawText(message, posX, posY, fontSize, BLUE);
}

void DrawIndicatorAndHealthBarAndPrintInfo(std::vector<Character*>& characters, SelectionModeENUM selectionMode, int index, std::vector<int>& targetIndices, int healthBarSize = 100, int indicatorRadius = 10, Color indicatorColor = GREEN)
{
	switch (selectionMode)
	{
	case NONE: //If nothing is selected, do nothing.
		PrintSelectEntireTroopTypeGuide({0, 10});
		break;
	case SINGLE: //if only one character is selected, draw character information and target indicator
		if (index >= 0 && index < characters.size())
		{
			for (int i = 0; i < characters.size(); i++)
			{
				if (i == index)
				{
					//Print character info
					characters.at(i)->printCharacterInfo();

					//Draw target indicator
					switch (characters.at(i)->getCommandStatus())
					{
					case NONE_COMMAND:
						indicatorColor = RED;
						break;
					case RANDOM_COMMAND:
						indicatorColor = ORANGE;
						break;
					case HOLD_COMMAND:
						indicatorColor = YELLOW;
						break;
					case ATTACK_COMMAND:
						indicatorColor = GREEN;
						break;
					case CHARGE_COMMAND:
						indicatorColor = BLUE;
						break;
					}
					characters.at(i)->drawTargetIndicator(camera.getPos(), indicatorRadius, indicatorColor);

					//Draw health bar
					characters.at(i)->drawHealthBar(camera.getPos(), healthBarSize);
				}
			}
		}
		PrintCommandGuide({ 0,10 });
		break;
	case MULTIPLE: //if multiple characters are selected, draw the target indicator for all of them
		for (auto index : targetIndices)
			if (index >= 0 && index < allyCharacters.size())
			{
				//Draw target indicator
				switch (characters.at(index)->getCommandStatus())
				{
				case NONE_COMMAND:
					indicatorColor = RED;
					break;
				case RANDOM_COMMAND:
					indicatorColor = ORANGE;
					break;
				case HOLD_COMMAND:
					indicatorColor = YELLOW;
					break;
				case ATTACK_COMMAND:
					indicatorColor = GREEN;
					break;
				case CHARGE_COMMAND:
					indicatorColor = BLUE;
					break;
				}
				characters.at(index)->drawTargetIndicator(camera.getPos(), indicatorRadius, indicatorColor);

				//Draw health bar
				characters.at(index)->drawHealthBar(camera.getPos(), healthBarSize);
			}
		PrintCommandGuide({ 0,10 });
		break;
	}

}

void DrawArrows(std::vector<Arrow*>& arrows, bool drawAttackBoxes)
{
	//Hitboxes
	if (drawAttackBoxes)
		for (auto& arrow : arrows)
			arrow->drawAttackbox(camera.getPos());

	//Drawing the arrows
	for (auto& arrow : arrows)
		arrow->draw(camera.getPos());
}

void DrawSpawnButtons(std::vector<Button*>& buttons, int& money, bool drawHitBoxes, Color priceColor = BLACK, float borderSize = 6, Color borderColor = BLACK, Color hoverBorderColor = GOLD, Color backgroundColor = WHITE, Color imageColor = WHITE, Color hoverColor = BLACK, Color hoverUnaffordableColor = RED)
{
	if (drawHitBoxes) for (auto& button : buttons) button->drawHitBox({ 0,0 });
	for (auto& button : buttons)
	{
		if (CheckCollisionPointRec(GetMousePosition(), button->getHitBox({ 0,0 })))
		{
			if (money >= allyPortal->getPurchasePrice(button->getCharacterENUM()))
				button->draw({ 0,0 }, borderSize, hoverBorderColor, combineColors(backgroundColor, hoverColor), imageColor);
			else
				button->draw({ 0,0 }, borderSize, hoverBorderColor, hoverUnaffordableColor, imageColor);
		}
		else
			button->draw({ 0,0 }, borderSize, borderColor, backgroundColor, imageColor);

		//Print price
		char message[100];
		strcpy_s(message, "$");
		strcat_s(message, std::to_string(static_cast<int>(allyPortal->getPurchasePrice(button->getCharacterENUM()))).c_str());

		int posX = button->getHitBox({ 0,0 }).x + borderSize / 2;
		int posY = button->getHitBox({ 0,0 }).y + borderSize / 2;
		DrawText(message, posX, posY, button->getScale() * 30, priceColor);

		//Print level
		strcpy_s(message, "LVL ");
		strcat_s(message, std::to_string(static_cast<int>(allyPortal->getLevel(button->getCharacterENUM()) + 1)).c_str());
		posX = button->getHitBox({ 0,0 }).x + button->getHitBox({ 0,0 }).width / 2;
		DrawText(message, posX, posY, button->getScale(), priceColor);
	}
}

void DrawUpgradeButtons(std::vector<Button*>& buttons, int& money, bool drawHitBoxes, Color priceColor = BLACK, float borderSize = 6, Color borderColor = BLACK, Color hoverBorderColor = GOLD, Color backgroundColor = GREEN, Color imageColor = WHITE, Color hoverColor = BLACK, Color hoverUnaffordableColor = RED)
{
	if (drawHitBoxes) for (auto& button : buttons) button->drawHitBox({ 0,0 });
	for (auto& button : buttons)
	{
		if (CheckCollisionPointRec(GetMousePosition(), button->getHitBox({ 0,0 })))
		{
			if (money >= allyPortal->getUpgradePrice(button->getCharacterENUM()))
				button->draw({ 0,0 }, borderSize, hoverBorderColor, combineColors(backgroundColor, hoverColor), imageColor);
			else
				button->draw({ 0,0 }, borderSize, hoverBorderColor, hoverUnaffordableColor, imageColor);
		}
		else
			button->draw({ 0,0 }, borderSize, borderColor, backgroundColor, imageColor);

		//Print price
		char message[100];
		if (allyPortal->getLevel(button->getCharacterENUM()) == 4)
		{
			strcpy_s(message, "Maxed");
		}
		else
		{
			strcpy_s(message, "$");
			strcat_s(message, std::to_string(static_cast<int>(allyPortal->getUpgradePrice(button->getCharacterENUM()))).c_str());
		}

		int posX = button->getHitBox({ 0,0 }).x + borderSize / 2;
		int posY = button->getHitBox({ 0,0 }).y + borderSize / 2;
		DrawText(message, posX, posY, button->getScale() * 30, priceColor);
	}
}

void DrawSelectionRectangle(Rectangle selectionRectangle, Color color = { 0, 228, 48, 100 }) 
{ DrawRectangle(selectionRectangle.x - camera.getPos().x, selectionRectangle.y - camera.getPos().y, selectionRectangle.width, selectionRectangle.height, color); }

void PrintRoundAndMoneyAndFPS(int roundLevel, int money, int fontSize = 30, float rightSideBuffer = 10, Color textColor = YELLOW)
{
	char message[100];
	float textWidth;
	int posX;
	int posY = 10;

	strcpy_s(message, "Round Level: ");
	strcat_s(message, std::to_string(roundLevel).c_str());
	textWidth = MeasureText(message, fontSize);
	posX = SCREEN_WIDTH - textWidth - rightSideBuffer;
	DrawText(message, posX, posY, fontSize, textColor);
	posY += fontSize;

	strcpy_s(message, "Money: $");
	strcat_s(message, std::to_string(money).c_str());
	textWidth = MeasureText(message, fontSize);
	posX = SCREEN_WIDTH - textWidth - rightSideBuffer;
	DrawText(message, posX, posY, fontSize, textColor);
	posY += fontSize;

	DrawFPS(SCREEN_WIDTH - 100, posY);
}

void Draw()
{
	BeginDrawing();
	DrawBackground();

	DrawPortals(drawHitBoxes);

	//Draw Alive Characters
	DrawCharactersWithSelectionHover(allyCharacters, selectionRectangle, drawHitBoxes, drawAttackBoxes);
	DrawCharacters(enemyCharacters, drawHitBoxes, drawAttackBoxes, WHITE);

	//Draw Dead Characters
	DrawCharacters(allyDeadCharacters, false, false);
	DrawCharacters(enemyDeadCharacters, false, false);

	DrawIndicatorAndHealthBarAndPrintInfo(allyCharacters, selectionMode, targetIndex, targetIndices);

	DrawArrows(allyArrows, drawAttackBoxes);
	DrawArrows(enemyArrows, drawAttackBoxes);

	//Draw special effects
	for (auto& effect : specialEffects)  effect->draw(camera.getPos());

	DrawSpawnButtons(spawnButtons, allyMoney, drawHitBoxes);
	DrawUpgradeButtons(upgradeButtons, allyMoney, drawHitBoxes);

	DrawSelectionRectangle(selectionRectangle);

	PrintRoundAndMoneyAndFPS(roundLevel, allyMoney);

	if (allyPortal->getCurrentHealth() <= 0)
	{
		char message[100];
		float textWidth;
		float fontSize = 100;
		int posX;
		int posY = (SCREEN_HEIGHT) / 2 - fontSize;

		strcpy_s(message, "Game Over");
		textWidth = MeasureText(message, fontSize);
		posX = (SCREEN_WIDTH - textWidth) / 2;
		DrawText(message, posX, posY, fontSize, RED);
		posY += fontSize;

		strcpy_s(message, "Press Space To Play Again");
		textWidth = MeasureText(message, fontSize);
		posX = (SCREEN_WIDTH - textWidth) / 2;
		DrawText(message, posX, posY, fontSize, RED);
		posY += fontSize;
	}

	EndDrawing();
}

void DeveloperInput()
{
	//Toggle hitboxes and attackboxes
	if (IsKeyPressed(KEY_I))
	{
		drawHitBoxes = !drawHitBoxes;
		drawAttackBoxes = !drawAttackBoxes;
	}

	//Swap teams
	if (IsKeyPressed(KEY_P))
	{
		//End movement for deselected characters
		for (auto index : targetIndices) if (index >= 0 && index < allyCharacters.size()) allyCharacters.at(index)->endPreviousControl();
		if (targetIndex >= 0 && targetIndex < allyCharacters.size()) allyCharacters.at(targetIndex)->endPreviousControl();

		Portal* tempPortal = enemyPortal;
		enemyPortal = allyPortal;
		allyPortal = tempPortal;

		int tempMoney = enemyMoney;
		enemyMoney = allyMoney;
		allyMoney = tempMoney;

		std::vector<Character*> tempCharacters = enemyCharacters;
		enemyCharacters = allyCharacters;
		allyCharacters = tempCharacters;

		std::vector<Character*> tempDeadCharacters = enemyDeadCharacters;
		enemyDeadCharacters = allyDeadCharacters;
		allyDeadCharacters = tempDeadCharacters;

		std::vector<Arrow*> tempArrows = enemyArrows;
		enemyArrows = allyArrows;
		allyArrows = tempArrows;

		selectionMode = NONE;
		targetIndex = -1;
		targetIndices.clear();
	}
}

void TakeInputForSpawningCharacters()
{
	for (auto& spawnButton : spawnButtons)
		if (spawnButton->isClicked({ 0,0 }) || spawnButton->isHeldWithShift({ 0,0 }))
		{
			if (allyPortal->hasEnoughMoneyForSpawningCharacter(spawnButton->getCharacterENUM(), allyMoney, true))
				allyCharacters.push_back(allyPortal->createCharacter(spawnButton->getCharacterENUM(), spawnButton->getName(), animationSpeed));
		}
}

void TakeInputForUpgradingCharacters()
{
	for (auto& upgradeButton : upgradeButtons)
		if (upgradeButton->isClicked({ 0,0 }))
		{
			allyPortal->upgrade(upgradeButton->getCharacterENUM(), allyMoney);
		}
}

void TakeInputForSelectingTargetCharacter()
{
	//Picking characters with the rectangle. Locks if only one character is selected, unlocks if multiple characters are selected
	if (IsMouseButtonPressed(0)) firstSelectionRecPos = { GetMousePosition().x + camera.getPos().x, GetMousePosition().y + camera.getPos().y};
	if (IsMouseButtonDown(0))
	{
		secondSelectionRecPos = { GetMousePosition().x + camera.getPos().x, GetMousePosition().y + camera.getPos().y};

		selectionRectangle = { std::min(firstSelectionRecPos.x, secondSelectionRecPos.x), std::min(firstSelectionRecPos.y, secondSelectionRecPos.y), 0, 0 };
		selectionRectangle.width = std::max(firstSelectionRecPos.x, secondSelectionRecPos.x) - selectionRectangle.x;
		selectionRectangle.height = std::max(firstSelectionRecPos.y, secondSelectionRecPos.y) - selectionRectangle.y;
	}
	if (IsMouseButtonReleased(0))
	{
		//Add a little buffer to differentiate between rectangle selection and mouse selection
		if (abs(selectionRectangle.width) > 5 && abs(selectionRectangle.height) > 5)
		{
			//End movement for deselected characters
			for (auto index : targetIndices) if (index >= 0 && index < allyCharacters.size()) allyCharacters.at(index)->endPreviousControl();
			if (targetIndex >= 0 && targetIndex < allyCharacters.size()) allyCharacters.at(targetIndex)->endPreviousControl();
			targetIndices.clear();

			for (int i = 0; i < allyCharacters.size(); i++)
				if (CheckCollisionRecs(allyCharacters.at(i)->getHitBox({0,0}), selectionRectangle))
					targetIndices.push_back(i);

			if (targetIndices.size() == 0)
			{
				selectionMode = NONE;
			}
			else if (targetIndices.size() == 1)
			{
				selectionMode = SINGLE;
				targetIndex = targetIndices.at(0);
			}
			else
			{
				selectionMode = MULTIPLE;
				camera.setLocked(false);
			}
		}
		selectionRectangle = { 0,0,0,0 };
	}



	//Pick target character (Doesn't lock)
	//Clicking on selected characters deselects it
	if (IsMouseButtonPressed(0))
	{
		for (int i = allyCharacters.size() - 1; i >= 0; i--)
		{
			if (CheckCollisionPointRec(GetMousePosition(), allyCharacters.at(i)->getHitBox(camera.getPos())))
			{
				//End movement for deselected characters
				for (auto index : targetIndices) if (index >= 0 && index < allyCharacters.size()) allyCharacters.at(index)->endPreviousControl();
				if (targetIndex >= 0 && targetIndex < allyCharacters.size()) allyCharacters.at(targetIndex)->endPreviousControl();

				if (i != targetIndex || selectionMode != SINGLE)
				{
					selectionMode = SINGLE;
					targetIndex = i;
				}
				else
				{
					selectionMode = NONE;
					targetIndex = -1;
					camera.setLocked(false);
				}
				break;
			}
		}
	}

	//Loop through characters for selection (Does lock)
	if (IsMouseButtonPressed(1))
	{
		//End movement for deselected characters
		for (auto index : targetIndices) if (index >= 0 && index < allyCharacters.size()) allyCharacters.at(index)->endPreviousControl();
		if (targetIndex >= 0 && targetIndex < allyCharacters.size()) allyCharacters.at(targetIndex)->endPreviousControl();

		targetIndex++;
		if (targetIndex >= allyCharacters.size())
			targetIndex = 0;

		selectionMode = SINGLE;
		camera.setLocked(true);

		if (allyCharacters.size() == 0)
		{
			targetIndex = -1;
			selectionMode = NONE;
			camera.setLocked(false);
		}
	}
}

void takeInputForSelectingEntireTroopType()
{
	//Select entire troop type
	if (selectionMode == NONE)
	{
		targetIndices.clear();
		if (IsKeyPressed(KEY_ONE)) //Select All
		{
			for (int i = 0; i < allyCharacters.size(); i++)
				targetIndices.push_back(i);
		}
		else if (IsKeyPressed(KEY_TWO))
		{
			for (int i = 0; i < allyCharacters.size(); i++)
				if (typeid(*allyCharacters.at(i)) == typeid(Samurai))
					targetIndices.push_back(i);
		}
		else if (IsKeyPressed(KEY_THREE))
		{
			for (int i = 0; i < allyCharacters.size(); i++)
				if (typeid(*allyCharacters.at(i)) == typeid(SamuraiCommander))
					targetIndices.push_back(i);
		}
		else if (IsKeyPressed(KEY_FOUR))
		{
			for (int i = 0; i < allyCharacters.size(); i++)
				if (typeid(*allyCharacters.at(i)) == typeid(SamuraiArcher))
					targetIndices.push_back(i);
		}
		else if (IsKeyPressed(KEY_FIVE))
		{
			for (int i = 0; i < allyCharacters.size(); i++)
				if (typeid(*allyCharacters.at(i)) == typeid(SkeletonWarrior))
					targetIndices.push_back(i);
		}
		else if (IsKeyPressed(KEY_SIX))
		{
			for (int i = 0; i < allyCharacters.size(); i++)
				if (typeid(*allyCharacters.at(i)) == typeid(SkeletonSpearman))
					targetIndices.push_back(i);
		}
		else if (IsKeyPressed(KEY_SEVEN))
		{
			for (int i = 0; i < allyCharacters.size(); i++)
				if (typeid(*allyCharacters.at(i)) == typeid(SkeletonArcher))
					targetIndices.push_back(i);
		}


		if (targetIndices.size() == 0)
		{
			selectionMode == NONE;
		}
		else if (targetIndices.size() == 1)
		{
			targetIndex = targetIndices.at(0);
			selectionMode = SINGLE;
			camera.setLocked(true);
		}
		else
		{
			selectionMode = MULTIPLE;
			camera.setLocked(false);
		}
	}
}

void Input()
{
	//Show HitBoxes, And swap teams
	//DeveloperInput();

	//Spawn Characters Buttons
	TakeInputForSpawningCharacters();

	//Upgrade Characters Buttons
	TakeInputForUpgradingCharacters();

	//Input for selecting target character
	TakeInputForSelectingTargetCharacter();

	//Input for selected character. Seleted characters take user input for movement and for changing command status
	//Non selected characters move based on the command status.
	for (int i = 0; i < allyCharacters.size(); i++)
	{
		bool selected = false;

		if (selectionMode == SINGLE && targetIndex == i)
		{
			allyCharacters.at(i)->takeInputForCommandStatus();
			allyCharacters.at(i)->takeInput();
			selected = true;
		}

		if (selectionMode == MULTIPLE)
			for (auto index : targetIndices)
			{
				if (index == i)
				{
					allyCharacters.at(i)->takeInputForCommandStatus();
					allyCharacters.at(i)->takeInput();
					selected = true;
					break;
				}
			}

		//If the character is selected and took no user input, it will take generated command input instead.
		if (selected)
			if (!(IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D) || IsKeyPressed(KEY_G) || IsKeyPressed(KEY_H) || IsKeyPressed(KEY_J) || IsKeyPressed(KEY_SPACE)))
				allyCharacters.at(i)->getInputBasedOnCommandStatus(enemyPortal->getHitBox({0,0}), enemyCharacters);

		if (!selected)
			allyCharacters.at(i)->getInputBasedOnCommandStatus(enemyPortal->getHitBox({ 0,0 }), enemyCharacters);
	}

	takeInputForSelectingEntireTroopType();

	//Enemy characters move based on command status
	for (auto& character : enemyCharacters)
		character->getInputBasedOnCommandStatus(allyPortal->getHitBox({ 0,0 }), allyCharacters);

	//Camera inputs
	camera.takeInput();
}

void CreateArrows(std::vector<Character*>& characters, std::vector<Arrow*>& arrows)
{
	for (int i = 0; i < characters.size(); i++)
	{
		if (typeid(*characters.at(i)) == typeid(SamuraiArcher))
			if (dynamic_cast<SamuraiArcher*>(characters.at(i))->isFiringArrow())
				arrows.push_back(dynamic_cast<SamuraiArcher*>(characters.at(i))->fireArrow());

		if (typeid(*characters.at(i)) == typeid(SkeletonArcher))
			if (dynamic_cast<SkeletonArcher*>(characters.at(i))->isFiringArrow())
				arrows.push_back(dynamic_cast<SkeletonArcher*>(characters.at(i))->fireArrow());
	}
}

//Characters being attacked by melee characters
void HandleMeleeAttacks(std::vector<Character*>& attackers, std::vector<Character*>& defenders)
{
	for (auto& attacker : attackers)
		if (attacker->attackBoxesAreActive())
			for (auto& defender : defenders)
				defender->isHitByRec(attacker->getAttackBox(camera.getPos()), attacker->getMeleeDamage(), attacker->getFacingLeft(), camera.getPos());
}

//Portal being attacked by melee characters
void HandleMeleeAttacks(std::vector<Character*>& attackers, Portal* defender)
{
	for (auto& attacker : attackers)
		if (attacker->attackBoxesAreActive())
			defender->isHitByRec(attacker->getAttackBox(camera.getPos()), attacker->getMeleeDamage(), camera.getPos());
}

//Characters being attacked by arrows
void HandleRangeAttacks(std::vector<Arrow*>& attackerArrows, std::vector<Character*>& defenders)
{
	for (int arrowIndex = 0; arrowIndex < attackerArrows.size(); arrowIndex++)
	{
		for (auto& defender : defenders)
		{
			if (defender->isHitByRec(attackerArrows.at(arrowIndex)->getAttackBox(camera.getPos()), attackerArrows.at(arrowIndex)->getDamage(), attackerArrows.at(arrowIndex)->getFacingLeft(), camera.getPos()))
			{
				Arrow* temp = attackerArrows.at(arrowIndex);
				attackerArrows.erase(attackerArrows.begin() + arrowIndex);
				delete temp;
				break;
			}
		}
	}
}

//Portal being attacked by arrows
void HandleRangeAttacks(std::vector<Arrow*>& attackerArrows, Portal* defender)
{
	for (int arrowIndex = 0; arrowIndex < attackerArrows.size(); arrowIndex++)
	{
		if (defender->isHitByRec(attackerArrows.at(arrowIndex)->getAttackBox(camera.getPos()), attackerArrows.at(arrowIndex)->getDamage(), camera.getPos()))
		{
			Arrow* temp = attackerArrows.at(arrowIndex);
			attackerArrows.erase(attackerArrows.begin() + arrowIndex);
			delete temp;
		}
	}
}

void HandleAnimationAndMovement(std::vector<Character*>& characters)
{
	for (auto& character : characters)
	{
		character->changeAnimation();
		character->updatePosition();
		character->checkBound(mapHorizontalBound);
	}
}

void HandleRemovingDeadCharacters(std::vector<Character*>& characters, std::vector<Character*>& deadCharacters, bool fixSelectionIndex)
{
	for (int i = 0; i < characters.size(); i++)
	{
		if (characters.at(i)->getCurrentHealth() <= 0)
		{
			if (fixSelectionIndex)
			{
				//Fixing TargetIndex
				if (targetIndex > i) targetIndex--;
				else if (targetIndex == i)
				{
					targetIndex = -1;
					if (selectionMode == SINGLE) selectionMode = NONE;
				}

				//Fixing targetIndices
				if (selectionMode == MULTIPLE)
				{
					for (int j = 0; j < targetIndices.size(); j++)
					{
						if (targetIndices.at(j) > i) targetIndices.at(j)--;
						else if (targetIndices.at(j) == i)
						{
							targetIndices.erase(targetIndices.begin() + j);
							j--;
						}
					}

					if (targetIndices.size() == 1)
					{
						targetIndex = targetIndices.at(0);
						selectionMode = SINGLE;
					}

					if (targetIndices.size() == 0)
					{
						targetIndex = targetIndices.at(0);
						selectionMode = NONE;
					}
				}
			}

			//Remove character to deadCharacters
			deadCharacters.push_back(characters.at(i));
			characters.erase(characters.begin() + i);
		}
	}
}

void HandleRemovingDeadBodies(std::vector<Character*>& deadCharacters, int size)
{
	while (deadCharacters.size() > size)
	{
		delete deadCharacters.at(0);
		deadCharacters.erase(deadCharacters.begin() + 0);
	}
}

void HandleArrowMovementAndLogic(std::vector<Arrow*>& arrows)
{
	for (int i = 0; i < arrows.size(); i++)
	{
		if (arrows.at(i)->reachedMaxRange())
		{
			Arrow* temp = arrows.at(i);
			arrows.erase(arrows.begin() + i);
			delete temp;
			i--;
		}
		else
		{
			arrows.at(i)->updatePosition();
			arrows.at(i)->checkBound();
		}
	}
}

void addSpecialEffects(std::vector<Character*>& characters)
{
	for (auto& character : characters)
	{
		std::vector<Effect*>& currentCharacterSE = character->getSpecialEffects();
		specialEffects.insert(specialEffects.end(), currentCharacterSE.begin(), currentCharacterSE.end());
		currentCharacterSE.clear();
	}
}

void addSpecialEffects(Portal* portal)
{
	std::vector<Effect*>& currentPortalSE = portal->getSpecialEffects();
	specialEffects.insert(specialEffects.end(), currentPortalSE.begin(), currentPortalSE.end());
	currentPortalSE.clear();
}

void HandleSpecialEffectsLogic()
{
	//Add special effects
	addSpecialEffects(allyCharacters);
	addSpecialEffects(enemyCharacters);
	addSpecialEffects(allyPortal);
	addSpecialEffects(enemyPortal);

	//Remove special effects that are finished
	for (int i = 0; i < specialEffects.size(); i++)
		if (specialEffects.at(i)->finishedAnimation())
		{
			delete specialEffects.at(i);
			specialEffects.erase(specialEffects.begin() + i);
			i--;
		}
}

void HandlePassiveIncome()
{
	timeSinceLastPassiveIncome += GetFrameTime();
	if (timeSinceLastPassiveIncome > passiveIncomeRate)
	{
		allyMoney += roundLevel;
		timeSinceLastPassiveIncome = 0;
	}
}

void HandleEarningMoneyFromDeadCharacters()
{
	for (auto& character : enemyCharacters)
	{
		if (character->getCurrentHealth() <= 0)
		{
			allyMoney += (character->getLevel() + 1) * roundLevel;
		}
	}
}

void HandleEarningMoneyFromDeadPortal()
{
	if (enemyPortal->getCurrentHealth() <= 0)
		allyMoney += roundLevel * 2;
}

void EnemySpawnCharacters()
{
	enemyTimeSinceLastSpawn += GetFrameTime();

	if (enemyTimeSinceLastSpawn >= enemySpawnRate)
	{
		int randNum2 = rand() % 3;
		if (randNum2 == 0) enemyCharacters.push_back(enemyPortal->createCharacter(SKELETON_WARRIOR, "Skeleton Warrior", animationSpeed));
		else if (randNum2 == 1) enemyCharacters.push_back(enemyPortal->createCharacter(SKELETON_SPEARMAN, "Skeleton Spearman", animationSpeed));
		else if (randNum2 == 2) enemyCharacters.push_back(enemyPortal->createCharacter(SKELETON_ARCHER, "Skeleton Archer", animationSpeed));
		enemyTimeSinceLastSpawn = 0;
	}
}

//Triggers when enemy portal is destroyed, revive it and spawn a boss. Increase enemy spawn rate, portal health, and boss stats.
void EnemyPortalUpgrades()
{
	if (enemyPortal->getCurrentHealth() == 0)
	{
		//Spawn a boss
		int randNum2 = rand() % 3;
		if (randNum2 == 0) enemyCharacters.push_back(enemyPortal->createBoss(SKELETON_WARRIOR, "Skeleton Warrior Boss", animationSpeed, bossStats));
		else if (randNum2 == 1) enemyCharacters.push_back(enemyPortal->createBoss(SKELETON_SPEARMAN, "Skeleton Spearman Boss", animationSpeed, bossStats));
		else if (randNum2 == 2) enemyCharacters.push_back(enemyPortal->createBoss(SKELETON_ARCHER, "Skeleton Archer Boss", animationSpeed, bossStats));

		roundLevel++;

		//Increase Enemy spawn rate
		enemySpawnRate *= enemySpawnRateSpeedUp;

		//Reset Portal Health
		enemyPortal->setMaxHealth(enemyPortal->getMaxHealth() * enemyBossStatsMultipler);
		enemyPortal->setCurrentHealth(enemyPortal->getMaxHealth());

		//Increase boss stats
		if (bossStats->getLevel() < bossStats->maxLevel) bossStats->setLevel(roundLevel / 2);
		else
		{
			bossStats->damage *= enemyBossStatsMultipler;
			bossStats->health *= enemyBossStatsMultipler;
			bossStats->scale *= enemyBossStatsMultipler;
		}
		if (bossStats->scale > maxBossScale) bossStats->scale = maxBossScale;

		//Randomly upgrade a troop type
		if (enemyPortal->getLevel(SKELETON_WARRIOR) < bossStats->maxLevel ||
			enemyPortal->getLevel(SKELETON_SPEARMAN) < bossStats->maxLevel ||
			enemyPortal->getLevel(SKELETON_ARCHER) < bossStats->maxLevel)
		{
			bool upgraded = false;
			while (!upgraded)
			{
				int randNum = rand() % 3;
				if (randNum == 0) upgraded = enemyPortal->upgrade(SKELETON_WARRIOR);
				else if (randNum == 1) upgraded = enemyPortal->upgrade(SKELETON_SPEARMAN);
				else upgraded = enemyPortal->upgrade(SKELETON_ARCHER);
			}
		}

		//Blow ally characters away from portal
		for (auto& character : allyCharacters)
		{
			Vector2 characterPos = character->getHitBoxCenterPos();
			Vector2 portalPos = enemyPortal->getHitBoxCenterPos();
			float distanceToPortal = sqrt(pow(characterPos.x - portalPos.x, 2) + pow(characterPos.y - portalPos.y, 2));

			int value = ((int)(mapHorizontalBound.y / 100 - distanceToPortal / 100));
			if (value == 0) value = 1;
			float magnitudeX = (rand() % value + value) * roundLevel;
			float magnitudeY = (rand() % value + value) * roundLevel;
			character->setInAirVelocity({-(rand() % (int)magnitudeX/2 + magnitudeX/2), -(rand() % (int)magnitudeY / 2 + magnitudeY / 2) });
		}
	}
}

void Logic()
{
	//Arrow Creation
	CreateArrows(allyCharacters, allyArrows);
	CreateArrows(enemyCharacters, enemyArrows);

	//Handle Melee Attacks for characters and portals
	HandleMeleeAttacks(allyCharacters, enemyCharacters);
	HandleMeleeAttacks(enemyCharacters, allyCharacters);
	HandleMeleeAttacks(allyCharacters, enemyPortal);
	HandleMeleeAttacks(enemyCharacters, allyPortal);

	//Handle Arrow Attacks for characters and portals
	HandleRangeAttacks(allyArrows, enemyCharacters);
	HandleRangeAttacks(enemyArrows, allyCharacters);
	HandleRangeAttacks(allyArrows, enemyPortal);
	HandleRangeAttacks(enemyArrows, allyPortal);

	//Handle earning passive income and money for killing enemy characters
	HandlePassiveIncome();
	HandleEarningMoneyFromDeadCharacters();
	HandleEarningMoneyFromDeadPortal();

	//Handle Enemy Portal Logic, spawning enemy troops, moving to next round level, spawning boss
	EnemySpawnCharacters();
	EnemyPortalUpgrades();

	//Handling Alive Character animation and movement logic
	HandleAnimationAndMovement(allyCharacters);
	HandleAnimationAndMovement(enemyCharacters);

	//Handling Dead Character animation and movement logic
	HandleAnimationAndMovement(allyDeadCharacters);
	HandleAnimationAndMovement(enemyDeadCharacters);

	//Handle moving dead characters to deadCharacters vector
	HandleRemovingDeadCharacters(allyCharacters, allyDeadCharacters, true);
	HandleRemovingDeadCharacters(enemyCharacters, enemyDeadCharacters, false);

	//Remove dead bodies
	HandleRemovingDeadBodies(allyDeadCharacters, maxDeadBodiesAmount);
	HandleRemovingDeadBodies(enemyDeadCharacters, maxDeadBodiesAmount);

	//Handling Arrow movement logic
	HandleArrowMovementAndLogic(allyArrows);
	HandleArrowMovementAndLogic(enemyArrows);

	//Handle Special Effects Logic
	HandleSpecialEffectsLogic();

	//Handle camera position
	if (camera.getLocked() && (targetIndex >= 0 && targetIndex < allyCharacters.size()))
		camera.setPos(allyCharacters.at(targetIndex)->getSyncCameraPos());
	camera.checkTopDownBound(mapVerticalBound, mapHorizontalBound);
}

void ClearVector()
{
	spawnButtons.clear();
	upgradeButtons.clear();

	allyCharacters.clear();
	allyDeadCharacters.clear();
	allyArrows.clear();

	enemyCharacters.clear();
	enemyDeadCharacters.clear();
	enemyArrows.clear();
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Samurai Vs Skeleton");
	SetWindowState(FLAG_VSYNC_HINT);
	srand(time(0));

	PrimarySetUp();
	SetUp();
	while (!WindowShouldClose())
	{
		Draw();
		if (allyPortal->getCurrentHealth() > 0) Input();
		else
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				ClearVector();
				SetUp();
			}
		}
		Logic();
	}
	CloseWindow();

	return 0;
}