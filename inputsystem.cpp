// This include:
#include "inputsystem.h"

// Local includes: 
#include "renderer.h" 
#include "logmanager.h" 
#include "scene.h"
#include "game.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"

// Library includes:
#include <cassert> 
#include <SDL.h>

InputSystem::InputSystem()
	: m_pCurrentKeyboardState(0)
	, m_previousMouseButtons(0)
	, m_currentMouseButtons(0)
	, m_pXboxController(0)
	, m_iNumAttachedControllers(0)
	, m_bRelativeMouseMode(false)
{

}
InputSystem::~InputSystem()
{

}

bool InputSystem::Initialise()
{
	for (int k = 0; k < SDL_NUM_SCANCODES; ++k)
	{
		m_previousKeyboardState[k] = 0;
	}

	m_pCurrentKeyboardState = SDL_GetKeyboardState(0);

	unsigned int alreadyInit = SDL_WasInit(SDL_INIT_GAMECONTROLLER);
	
	if (alreadyInit == 0)
	{
		int result = SDL_Init(SDL_INIT_GAMECONTROLLER);

		LogManager::GetInstance().Log("SDL Init Game Controller System");
	}

	ShowMouseCursor(false);

	m_iNumAttachedControllers = SDL_NumJoysticks();

	return true;
}

void InputSystem::ProcessInput()
{
	// Store the old state...
	for (int k = 0; k < SDL_NUM_SCANCODES; ++k)
	{
		m_previousKeyboardState[k] = m_pCurrentKeyboardState[k];
	}
	m_previousMouseButtons = m_currentMouseButtons;

	// Update the mouse state... 
	m_mouseWheel.Set(0.0f, 0.0f); 
	int mouseX = 0;
	int mouseY = 0;

	if (m_bRelativeMouseMode)
	{
		m_currentMouseButtons = SDL_GetRelativeMouseState(&mouseX, &mouseY);
	}
	else
	{
		m_currentMouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
	}

	m_mousePosition.Set(static_cast<float>(mouseX), static_cast<float>(mouseY));

	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		ImGuiIO& io = ImGui::GetIO();

		// Tell ImGui to hide the mouse cursor:
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		if (io.WantCaptureMouse || io.WantCaptureKeyboard)
		{
			ImGui_ImplSDL2_ProcessEvent(&event); continue;
		}
		ProcessMouseWheel(event);
		
		if (event.type == SDL_QUIT)
		{
			Game::GetInstance().Quit();
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_BACKQUOTE)
			{
				Game::GetInstance().ToggleDebugWindow();
			}
		}
	}
}
ButtonState InputSystem::GetKeyState(SDL_Scancode key)
{
	if (m_previousKeyboardState[key] == 0)
	{
		if (m_pCurrentKeyboardState[key] == 0)
		{
			return BS_NEUTRAL;
		}
		else
		{
			return BS_PRESSED;
		}
	}
	else
	{
		if (m_pCurrentKeyboardState[key] == 0)
		{
			return BS_RELEASED;
		}
		else
		{
			return BS_HELD;
		}
	}
}

void InputSystem::ShowMouseCursor(bool show)
{
	if (show == true)
	{
		SDL_ShowCursor(1);
	}
	else
	{
		SDL_ShowCursor(0);
	}
}

const Vector2& InputSystem::GetMousePosition() const
{
	return m_mousePosition;
}

const Vector2& InputSystem::GetMouseScrollWheel() const
{
	return m_mouseWheel;
}

ButtonState InputSystem::GetMouseButtonState(int button)
{
	if ((m_previousMouseButtons & SDL_BUTTON(button)) == 0)
	{
		if ((m_currentMouseButtons & SDL_BUTTON(button)) == 0)
		{
			return BS_NEUTRAL;
		}
		else
		{
			return BS_PRESSED;
		}
	}
	else
	{
		if ((m_currentMouseButtons & SDL_BUTTON(button)) == 0)
		{
			return BS_RELEASED;
		}
		else
		{
			return BS_HELD;
		}
	}
}
void
InputSystem::ProcessMouseWheel(SDL_Event& event)
{
	if (event.type == SDL_MOUSEWHEEL)
	{
		m_mouseWheel.x = static_cast<float>(event.wheel.x);
		m_mouseWheel.y = static_cast<float>(event.wheel.y);
	}
}

void
InputSystem::SetRelativeMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	m_bRelativeMouseMode = relative;
}

int
InputSystem::GetNumberOfControllersAttached() const
{
	return m_iNumAttachedControllers;
}

