#pragma once

#include "Graphics.h"

class Player
{
private:
	class Bullet
	{
	public:
		void Spawn(const Vec2& mousePos, const Vec2& playerPos);
		void Move(float dt);
		void ClampScreen();
		CircF GetCirc() const;
		void Draw(Graphics& gfx) const;
	private:
		Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
		Vec2 vel{ 0.0f, 0.0f };
		bool active = false;
		static constexpr float speed = 500.0f;
		static constexpr float radius = 5.0f;
	};
public:
	void Move(float dt, bool left, bool right, bool up, bool down);
	void ClampScreen();
	bool Fire(const Vec2& mousePos, float dt, bool fire);
	void UpdateBullets(float dt);
	CircF GetCirc() const;
	void Draw(Graphics& gfx) const;
	void DrawBullets(Graphics& gfx);
private:
	Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
	static constexpr float speed = 200.0f;
	static constexpr float radius = 10.0f;
	static constexpr int hpMax = 10000;
	int hp = hpMax;
	static constexpr float fireRate = 0.5f;
	float fireTime = 0.0f;
	static constexpr int nBullets = 100;
	int currentBullet = 0;
	Bullet bullets[nBullets];
};