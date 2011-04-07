#ifndef EASYAGENT_H
#define EASYAGENT_H


class EasyAgent : public Player {

public:
	void perform_logic();
	void handleWiiEvent();

	EasyAgent(int playerIndex_);
};

#endif