#include "manager.h"
#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "DirectXTex.h"
#include "texture.h"
#include "input.h"
#include "mouse.h"
#include "gameObject.h"
#include "polygon.h"
#include "templateCharacter.h"
#include "ClientManager.h"

void CTemplateCharacter::Init(std::string fileName, float x, float y)
{
	m_ScreenImg = ASPECT;
	m_ImgW = 106.0f * m_ScreenImg;
	m_ImgH = 317.0f * m_ScreenImg;
	m_MoveSpeed = 1.0f;

	m_Poly = new CPolygon();
	m_Poly->Init(fileName, m_ImgW, m_ImgH, 1.0f);
	m_Poly->m_number = 1;

	m_Position = XMFLOAT3(x, y, 0.0f);
	m_Poly->SetPosition(m_Position);

	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Poly->SetScale(m_Scale);
}

void CTemplateCharacter::Uninit()
{
	m_Poly->Uninit();
}

void CTemplateCharacter::Update()
{
	//m_Poly->SetPosition(m_Position);
}

void CTemplateCharacter::Draw()
{
	m_Poly->Draw(m_Poly->m_number);
}