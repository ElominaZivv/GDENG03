#pragma once
#include <All.h>

class MainGame : public jazzy::Game
{
public:
	explicit MainGame(const jazzy::GameDesc& desc);
protected:
	virtual void onCreate();
	virtual void onUpdate(jazzy::f32 deltaTime);

};

