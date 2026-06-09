#ifndef UI_EVENTS_H
#define UI_EVENTS_H

struct LivesChangedEvent
{
	int PlayerId;
	int Lives;
};

struct ScoreChangedEvent
{
	int PlayerId;
	int Score;
};

#endif // !UI_EVENTS_H