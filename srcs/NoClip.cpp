#include <Windows.h>

#include "../includes/NoClip.h"
#include "../includes/Vector3.h"
#include "../includes/Game.h"

// WIP
// TODO: It must not clip ...
// TODO: Get and use forward vector

bool NoClip::active = false;
float NoClip::speed = 5.0f;

void NoClip::Init()
{
	std::cout << "Press F4 to toggle No Clip" << std::endl << std::endl;
}

void NoClip::Loop()
{
	if (GetAsyncKeyState(VK_F4) & 0x1)
	{
		NoClip::Toggle();
	}

	if (NoClip::active)
	{
		Entity localEntity = Game::GetLocalEntity();
		Vector3 velocity(0.0f, 0.0f, 0.0f);

		// Left/right
		if (GetAsyncKeyState(VK_UP))
		{
			velocity.x = NoClip::speed;
		}
		
		if (GetAsyncKeyState(VK_DOWN))
		{
			velocity.x = -NoClip::speed;
		}

		// Forward/backward
		if (GetAsyncKeyState(VK_LEFT))
		{
			velocity.y = -NoClip::speed;
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			velocity.y = NoClip::speed;
		}

		// Up/Down
		if (GetAsyncKeyState(VK_SPACE))
		{
			velocity.z = NoClip::speed;
		}

		if (GetAsyncKeyState(VK_CONTROL))
		{
			velocity.z = -NoClip::speed;
		}

		localEntity.SetVelocity(velocity);
	}
}

void NoClip::Toggle()
{
	NoClip::active = !NoClip::active;

	std::cout << (NoClip::active ? "NoClip activated" : "NoClip deactivated") << std::endl;
}