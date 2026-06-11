#ifndef PLAYER_SESSION_H
#define PLAYER_SESSION_H

struct PlayerSession
{
	int const PlayerId;
	bool const AsEnemy;

	int Lives{};
	int Score{};
};

#endif // !PLAYER_SESSION_H
