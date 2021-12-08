//=============================================================================
//
// �A�� [plant.cpp]
// Author : �����@��
//
//=============================================================================
#include "plant.h"

Tree::Tree()
{
	m_pBox = new Box;
}
Tree::~Tree()
{
	delete m_pBox;
}
void Tree::Update()
{
	m_pBox->Update();
}
void Tree::OldDraw()
{
	m_pBox->DrawOldNow(1);
}
void Tree::NowDraw()
{
	m_pBox->DrawOldNow(0);
}

// �؂̎��A����(�j�̎q�̍s��)
void Tree::Plant(XMFLOAT3 pos)
{
	pos.x += 10.0f;
	pos.y += -8.0f;
	m_pBox->CreateOldNow(pos, 1);
}

// �؂�������(���ł̕ω�)
void Tree::Grow(XMFLOAT3 pos)
{
	pos.x += 10.0f;
	for (int i = 0; i < 9; i++)
	{
		m_pBox->CreateOldNow(pos, 0);
		pos.y += 10.0f;
	}
}