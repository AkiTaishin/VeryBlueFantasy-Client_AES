#include "manager.h"
#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "characterPoly.h"
#include "DirectXTex.h"
#include "texture.h"
#include "input.h"
#include "mouse.h"
#include "gameObject.h"
#include "polygon.h"
#include "ClientManager.h"

void CCharacterPoly::Init()
{
	m_ScreenImg = SCREEN_WIDTH / 320.0f;
	m_ImgW = 320.0f * m_ScreenImg;
	m_ImgH = 470.0f * m_ScreenImg;
	m_MoveSpeed = 1.0f;

	m_Poly = new CPolygon();
	m_Poly->Init("asset/background/char.png", m_ImgW, m_ImgH, 1.0f);
	m_Poly->m_number = 1;

	m_Position = XMFLOAT3(0.0f, -105.0f, 0.0f);
	m_Poly->SetPosition(m_Position);

	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Poly->SetScale(m_Scale);
}

void CCharacterPoly::Uninit()
{
	m_Poly->Uninit();
}

void CCharacterPoly::Update()
{
}

void CCharacterPoly::Draw()
{
	m_Poly->Draw(m_Poly->m_number);
}