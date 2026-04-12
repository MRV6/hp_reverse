#include "../includes/Studs.h"
#include "../includes/Memory.h"
#include "../includes/Debug.h"

uintptr_t Studs::address = 0;

void Studs::Init()
{
	Studs::address = Memory::GetPointerAddress(0x01419430, { 0x0 });

	Debug::Print("Studs initialized");
}

void Studs::Add(unsigned int amount)
{
	Studs::Set(Studs::Get() + amount);
}

void Studs::Remove(unsigned int amount)
{
	Studs::Set(Studs::Get() - amount);
}

void Studs::Set(unsigned int amount)
{
	*(unsigned int*)Studs::address = amount;
}

unsigned int Studs::Get()
{
	return *(unsigned int*)Studs::address;
}