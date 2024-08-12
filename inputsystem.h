#ifndef  INPUTSYSTEM_H   
#define  INPUTSYSTEM_H 

// Local includes: 
#include "vector2.h"

// Library includes: 
#include <SDL.h>

// Forward declarations: 
class XboxController;

// Enumerations 
enum ButtonState
{
	BS_NEUTRAL, BS_PRESSED, BS_RELEASED, BS_HELD
};

// Class declaration: 
class InputSystem
{
	// Member methods: 
public:
	InputSystem();
	~InputSystem();
	bool Initialise();
	void ProcessInput();

	// Keyboard:
	ButtonState GetKeyState(SDL_Scancode key);

	// Mouse:
	const Vector2& GetMousePosition() const;
	const Vector2& GetMouseScrollWheel() const;
	ButtonState GetMouseButtonState(int button);
	void ShowMouseCursor(bool show);
	void SetRelativeMode(bool relative);

	// Xbox Controllers:
	int GetNumberOfControllersAttached() const;
	XboxController* GetController(int controllerIndex);

protected:
	void ProcessMouseWheel(SDL_Event& event);

private:
	InputSystem(const InputSystem& inputsystem);
	InputSystem& operator=(const InputSystem& inputsystem);
	// Member data:
public:

protected:
	unsigned char m_previousKeyboardState[SDL_NUM_SCANCODES];
	const unsigned char* m_pCurrentKeyboardState;

	Vector2 m_mousePosition;
	Vector2 m_mouseWheel;
	unsigned int m_previousMouseButtons;
	unsigned int m_currentMouseButtons;
	bool m_bRelativeMouseMode;

	XboxController* m_pXboxController;
	int m_iNumAttachedControllers;

private:

};

#endif //   INPUTSYSTEM_H 
