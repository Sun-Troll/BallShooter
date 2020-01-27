#include "Player.h"
#include <algorithm>

void Player::Move(float dt, bool left, bool right, bool up, bool down)
{
	Vec2 moveDir{ 0.0f, 0.0f };
	if (left)
	{
		moveDir.x = -1.0f;
	}
	else if (right)
	{
		moveDir.x = 1.0f;
	}
	if (up)
	{
		moveDir.y = -1.0f;
	}
	else if (down)
	{
		moveDir.y = 1.0f;
	}
	pos += moveDir.GetNormalized() * speed * dt;
}

void Player::ClampScreen()
{
	if (pos.x < 399.0f + radius)
	{
		pos.x = 399.0f + radius;
	}
	else if (pos.x > float(Graphics::ScreenWidth) - radius)
	{
		pos.x = float(Graphics::ScreenWidth) - radius;
	}
	if (pos.y < -1.0f + radius)
	{
		pos.y = -1.0f + radius;
	}
	else if (pos.y > float(Graphics::ScreenHeight) - radius)
	{
		pos.y = float(Graphics::ScreenHeight) - radius;
	}
}

CircF Player::GetCirc() const
{
	return CircF(pos, radius);
}

void Player::Draw(Graphics& gfx) const
{
	int blue = std::max(hp, 0) * 255 / hpMax;
	gfx.DrawCircle(GetCirc(), Color{ 255, 0, unsigned char(std::max(hp, 0) * 255 / hpMax) });
}
