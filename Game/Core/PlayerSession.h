#ifndef PLAYER_SESSION_H
#define PLAYER_SESSION_H

struct PlayerSession
{
	int PlayerId{};
	int Lives{};
	int Score{};
	bool IsDead{ false };
};

#endif // !PLAYER_SESSION_H
