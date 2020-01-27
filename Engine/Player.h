#pragma once

#include "Graphics.h"

class Player
{
public:
	void Move(float dt, bool left, bool right, bool up, bool down);
	void ClampScreen();
	CircF GetCirc() const;
	void Draw(Graphics& gfx) const;
private:
	Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
	float speed = 200.0f;
	float radius = 10.0f;
	int hpMax = 10000;
	int hp = hpMax;
};