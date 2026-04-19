#pragma once

#include <queue>
#include <functional>
#include <mutex>

typedef float(__fastcall* _sub_52BED0)(float* a1, float* a2, float* a3);

class Main
{
public:
	static std::queue <std::function<void()>> funcQueue;
	static std::mutex funcQueueMutex;

	static void Init(HMODULE hModule);
	static void Shutdown(HMODULE hModule);
	static void InitModules();
	static void LoopModules();
	static void RunInGameThread(std::function<void()> fn);
};