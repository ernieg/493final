#include "zenilib.h"
#include "Player.h"

Player::Player(int playerIndex_)
	:playerIndex(playerIndex_)
{
}

int Player::getPlayerIndex()
{
	return playerIndex;
}

void Player::perform_logic()
{

}

void Player::handleWiiEvent()
{

}