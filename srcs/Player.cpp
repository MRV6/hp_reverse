#include "../includes/Player.h"

Player::Player(uintptr_t address): ptr(reinterpret_cast<GamePlayer*>(address))
{

}

void Player::SetVelocity(Vector3 velocity)
{
	this->ptr->xVelocity = velocity.x;
	this->ptr->yVelocity = velocity.y;
	this->ptr->zVelocity = velocity.z;
}

Vector3 Player::GetVelocity()
{
	return Vector3(this->ptr->xVelocity, this->ptr->yVelocity, this->ptr->zVelocity);
}