#include "MyCamera.h"

MyCamera::MyCamera() {}
MyCamera::MyCamera(Vector2 pos, float speed, bool locked)
	: pos(pos), speed(speed), locked(locked) {}

Vector2 MyCamera::getPos() { return pos; }
bool MyCamera::getLocked() { return locked; }
void MyCamera::setPos(Vector2 pos) { this->pos = pos; }
void MyCamera::setLocked(bool locked) { this->locked = locked; }

void MyCamera::takeInput()
{
	//Get The Direction
	Vector2 direction = { 0,0 };
	if (IsKeyDown(KEY_LEFT)) direction.x -= 1;
	if (IsKeyDown(KEY_RIGHT)) direction.x += 1;
	if (IsKeyDown(KEY_UP)) direction.y -= 1;
	if (IsKeyDown(KEY_DOWN)) direction.y += 1;
	direction = Vector2Normalize(direction);

	//Calculate The Change Vector
	Vector2 changeVector = Vector2Scale(direction, speed * GetFrameTime());

	//Update The Camera Position
	pos = Vector2Add(pos, changeVector);

	//Lock Or Unlock Camera
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)) locked = false;
	if (IsKeyPressed(KEY_L) || IsMouseButtonPressed(3)) locked = !locked;
}

void MyCamera::checkTopDownBound(Vector2 verticalBound, Vector2 horizontalBound)
{
	if (pos.y < verticalBound.x) pos.y = verticalBound.x;
	if (pos.y > verticalBound.y) pos.y = verticalBound.y;

	if (pos.x < horizontalBound.x) pos.x = horizontalBound.x;
	if (pos.x > horizontalBound.y - SCREEN_WIDTH) pos.x = horizontalBound.y - SCREEN_WIDTH;
}