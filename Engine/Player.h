#pragma once

#include "Graphics.h"

class Player
{
public:
	enum class TeleportState
	{
		Nothing,
		Fire,
		Port
	};
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
		static constexpr float speed = 1000.0f;
		static constexpr float radius = 5.0f;
	};
	class Teleport
	{
	public:
		void Spawn(const Vec2& mousePos, const Vec2& playerPos);
		void Port();
		void Move(float dt);
		void ClampScreen();
		CircF GetCirc() const;
		bool GetActive() const;
		Vec2 GetPos() const;
		void Draw(Graphics& gfx) const;
	private:
		Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
		Vec2 vel{ 0.0f, 0.0f };
		bool active = false;
		static constexpr float speed = 500.0f;
		static constexpr float radius = 15.0f;
	};
public:
	void Move(float dt, bool left, bool right, bool up, bool down);
	void ClampScreen();
	bool SlowDown(float dt, bool slow);
	bool Fire(const Vec2& mousePos, float dt, bool moving);
	void UpdateBullets(float dt);
	TeleportState TeleportFirePort(const Vec2& mousePos, float dt, bool fire);
	void UpdateTelleport(float dt);
	CircF GetCirc() const;
	void Draw(Graphics& gfx) const;
	void DrawBullets(Graphics& gfx);
	void DrawTeleport(Graphics& gfx);
private:
	Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
	static constexpr float speed = 250.0f;
	static constexpr float radius = 15.0f;
	static constexpr int hpMax = 10000;
	int hp = hpMax;
	//slowDown
	bool isSlowed = false;
	static constexpr float slowDur = 5.0f;
	float slowDurRem = 0.0f;
	static constexpr float slowRech = 15.0f;
	float slowRechRem = 0.0f;
	//bullet
	static constexpr float fireRate = 0.2f;
	float fireTime = 0.0f;
	static constexpr int nBullets = 100;
	int currentBullet = 0;
	Bullet bullets[nBullets];
	Teleport teleport;
	//teleport
	static constexpr float telRech = 2.0f;
	float telRechCur = 0.0f;
};