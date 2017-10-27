//
//  DetectionSouris.h
//

#ifndef __ID__DetectionSouris__
#define __ID__DetectionSouris__

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;


class DetectionSouris : public IEventReceiver
{
	public:

	DetectionSouris();

	struct SMouseState {
		core::position2di Position;
		bool LeftButtonDown;
        bool appui;
		SMouseState() : LeftButtonDown(false) { }
	} MouseState;

	virtual bool OnEvent(const SEvent &event);
	const SEvent::SJoystickEvent &GetJoystickState(void) const;
	const SMouseState &GetMouseState(void) const;

private:
	SEvent::SJoystickEvent JoystickState;
};


#endif
