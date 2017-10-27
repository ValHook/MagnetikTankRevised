//
//  DetectionSouris.cpp
//


#include "DetectionSouris.h"

bool DetectionSouris::OnEvent(const SEvent &event)
{
    // On quitte le jeu si on appuie sur échap
    // En fait cette classe detecte aussi le clavier contrairement à son nom
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        if (event.KeyInput.Key == KEY_ESCAPE)
            exit(0);

	// Etat de la souris
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		switch(event.MouseInput.Event) {
			case EMIE_LMOUSE_PRESSED_DOWN:
                if (!MouseState.LeftButtonDown)
                    MouseState.appui = true;
                else
                    MouseState.appui = false;
                MouseState.LeftButtonDown = true;
				break;

			case EMIE_LMOUSE_LEFT_UP:
				MouseState.LeftButtonDown = false;
                MouseState.appui = false;
				break;

			case EMIE_MOUSE_MOVED:
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				break;

			default:
				// On ignore la molette
				break;
		}
	}


	if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
		&& event.JoystickEvent.Joystick == 0) {
		JoystickState = event.JoystickEvent;
	}

	return false;
}

const SEvent::SJoystickEvent &DetectionSouris::GetJoystickState(void) const
{
	return JoystickState;
}

const DetectionSouris::SMouseState &DetectionSouris::GetMouseState(void) const
{
	return MouseState;
}


DetectionSouris::DetectionSouris()
{
}



