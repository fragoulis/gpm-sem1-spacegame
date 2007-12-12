#pragma once

class Timer;
class Event
{
private:
	Timer *m_Timer;
	bool m_bIsFired;

public:
	Event( double dToFire );
	const Timer* getTimer() const { return m_Timer; }
	bool isFired() const { return m_bIsFired; }
	void fired() { m_bIsFired = true; }
	virtual void fire() const = 0;
};