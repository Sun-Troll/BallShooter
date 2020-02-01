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
private:
	class BulletBasic
	{
	public:
		void Spawn(const Vec2& targetPos, const Vec2& enemy0Pos);
		void Move(float dt);
		void ClampScreen();
		int Hit();
		CircF GetCirc() const;
		bool GetActive() const;
		void Draw(Graphics& gfx) const;
	private:
		Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
		Vec2 vel{ 0.0f, 0.0f };
		bool active = false;
		static constexpr float speed = 200.0f;
		static constexpr float radius = 7.0f;
		static constexpr int damage = 1000;
	};
	class BulletAim
	{
	public:
		void Spawn(const Vec2& playerPos, const Vec2& enemy0Pos);
		void Move(float dt);
		void ClampScreen();
		int Hit();
		CircF GetCirc() const;
		bool GetActive() const;
		void Draw(Graphics& gfx) const;
	private:
		Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
		Vec2 vel{ 0.0f, 0.0f };
		bool active = false;
		static constexpr float speed = 400.0f;
		static constexpr float radius = 9.0f;
		static constexpr int damage = 2000;
	};
public:
	bool Spawn(float dt);
	bool PhaseSwitch();
	bool Die(); //call last!
	void DamagePlayer(Player& player, float dt) const;
	void BulletHit(Player& player, int index);
	void BombHit(Player& player, int index, float dt);
	bool FireBulletBasic(float dt);
	void UpdateBulletsBasic(float dt);
	void BulletBasicHit(Player& player);
	bool FireBulletAim(const Vec2& playerPos, float dt);
	void UpdateBulletsAim(float dt);
	void BulletAimHit(Player& player);
	CircF GetCirc() const;
	void Draw(Graphics& gfx) const;
	void DrawBulletsBasic(Graphics& gfx) const;
	void DrawBulletsAim(Graphics& gfx) const;
private:
	float spawnTimer = -10.0f;
	Phase curPhase = Phase::Waiting;
	Vec2 pos{ float(Graphics::ScreenWidth) / 2.0f + 200.0f, float(Graphics::ScreenHeight) / 2.0f };
	static constexpr float pi = 3.14159274f;
	static constexpr float speed = 250.0f;
	static constexpr float radius = 50.0f;
	static constexpr int hpMax = 1000000;
	int hp = hpMax;
	static constexpr float touchDamage = 3000.0f;
	//bulletBasic
	static constexpr float fireRateBasic = 0.3f;
	float fireTimeBasic = 0.0f;
	static constexpr int nBulletsBasic = 200;
	int currentBulletBasic = 0;
	float targetBasic = 0.0f;
	BulletBasic bulletsBasic[nBulletsBasic];
	//bulletAim
	static constexpr float fireRateAim = 1.0f;
	float fireTimeAim = 0.0f;
	static constexpr int nBulletsAim = 30;
	int currentBulletAim = 0;
	static constexpr int RapidFireSalvo = 5;
	int curSalvo = 0;
	BulletAim bulletsAim[nBulletsAim];
};