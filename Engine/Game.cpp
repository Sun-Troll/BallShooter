/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	float dt = ft.FrameDuration();

	bool slow = false;
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool moving = false;
	bool fireTeleport = false;
	Vec2 fTelMousePos{ 0.f, 0.f };
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		slow = true;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		left = true;
		moving = true;
	}
	else if (wnd.kbd.KeyIsPressed('D'))
	{
		right = true;
		moving = true;
	}
	if (wnd.kbd.KeyIsPressed('W'))
	{
		up = true;
		moving = true;
	}
	else if (wnd.kbd.KeyIsPressed('S'))
	{
		down = true;
		moving = true;
	}
	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			fireTeleport = true;
			fTelMousePos = e.GetPos();
		}
	}

	if (player.SlowDown(dt, slow))
	{
		dt /= 2.0f;
	}
	player.Move(dt, left, right, up, down);
	player.ClampScreen();
	player.Fire(wnd.mouse.GetPos(), dt, moving);
	player.UpdateBullets(dt);
	const Player::TeleportState telState = player.TeleportFirePort(fTelMousePos, dt, fireTeleport);
	player.UpdateTelleport(dt);
}

void Game::ComposeFrame()
{
	for (int y = 0; y < Graphics::ScreenHeight; y++)
	{
		for (int x = 397; x < 400; x++)
		{
			gfx.PutPixel(x, y, Colors::White);
		}
	}
	player.Draw(gfx);
	player.DrawBullets(gfx);
	player.DrawTeleport(gfx);
}
