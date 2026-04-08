#pragma once

class NoClip
{
private:
	static bool active;
	static float speed;

	static void Toggle();

public:
	static void Init();
	static void Loop();
};