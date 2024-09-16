//this includes:
#include "Win.h"

//local includes:
#include "renderer.h"
#include "sprite.h"

#include "imgui/imgui.h"

//library includes:
#include <cassert>

WinScene::WinScene() :
	m_pCentre(0),
	m_angle(0.0f),
	m_rotationSpeed(0.0f)
{

}

WinScene::~WinScene()
{
	delete m_pCentre;
	m_pCentre = 0;
}

bool WinScene::Initialise(Renderer& renderer)
{
	m_pCentre = renderer.CreateSprite("Sprites\\win.png");

	const int BOARD_HALF_WIDTH = m_pCentre->GetWidth() / 2;
	const int BOARD_HALF_HEIGHT = m_pCentre->GetHeight() / 2;
	const int SCREEN_WIDTH = renderer.GetWidth();
	const int SCREEN_HEIGHT = renderer.GetHeight();

	m_pCentre->SetX(SCREEN_WIDTH / 2);
	m_pCentre->SetY(SCREEN_HEIGHT / 2);

	return true;
}
void WinScene::Process(float deltaTime, InputSystem& inputSystem)
{
	m_angle += m_rotationSpeed * deltaTime;

	m_pCentre->SetAngle(m_angle);
	m_pCentre->Process(deltaTime);
}

void WinScene::Draw(Renderer& renderer)
{
	m_pCentre->Draw(renderer);
}

void WinScene::DebugDraw()
{
	ImGui::Text("Scene: Winning Title");
	ImGui::InputFloat("Rotation speed", &m_rotationSpeed); float scale = m_pCentre->GetScale();
	ImGui::SliderFloat("Demo scale", &scale, 0.0f, 2.0f, "%.3f"); m_pCentre->SetScale(scale);

	int position[2];
	position[0] = m_pCentre->GetX(); position[1] = m_pCentre->GetY();
	ImGui::InputInt2("Demo position", position); m_pCentre->SetX(position[0]);
	m_pCentre->SetY(position[1]);

	float tint[4];
	tint[0] = m_pCentre->GetRedTint();
	tint[1] = m_pCentre->GetGreenTint();
	tint[2] = m_pCentre->GetBlueTint();
	tint[3] = m_pCentre->GetAlpha();
	ImGui::ColorEdit4("Demo tint", tint);
	m_pCentre->SetRedTint(tint[0]);
	m_pCentre->SetGreenTint(tint[1]);
	m_pCentre->SetBlueTint(tint[2]);
	m_pCentre->SetAlpha(tint[3]);
}
