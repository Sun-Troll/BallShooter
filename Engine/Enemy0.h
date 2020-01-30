#pragma once

#include "Player.h"

class Enemy0
{
public:
	enum class Phase
	{
		Waiting,
		First,
		Second,
		Dead
	};
public:
	bool Spawn(float dt);
	CircF GetCirc() const;
	void Draw(Graphics& gfx) const;
private:
	float spawnTimer = -10.0f;
	Phase curPhase = Phase::Waiting;
	Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
	float target = 0.0f;
	static constexpr float pi = 3.14159274f;
	static constexpr float speed = 250.0f;
	static constexpr float radius = 50.0f;
	static constexpr int hpMax = 1000000;
	int hp = hpMax;
};