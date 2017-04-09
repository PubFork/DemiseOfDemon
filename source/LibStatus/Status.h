#pragma once

#include "Task.h"
#include "Network.h"

PTR( Status );
PTR( ViewerUI );
PTR( Sound );

class Status : public Task {
public:
	static std::string getTag( ) { return "STATUS"; }
	static StatusPtr getTask( );
public:
	Status( int index );
	virtual ~Status( );
public:
	virtual void update( );
	void updateGauge( );
	int getGauge( );
private:
	void updateSE( );
private:
	const int _index;
	ViewerUIPtr _ui;
	int _gauge;
	int _timeout;
	int _cheak_state;
	int _cheak_life;
	SoundPtr _sound;
};

