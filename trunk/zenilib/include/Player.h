#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	void perform_logic();
	void handleWiiEvent();

	Player(int playerIndex_);

private:
	int playerIndex; // zero-indexed
}

#endif