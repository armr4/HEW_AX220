//=============================================================================
//
// 一時停止 [pause.cpp]
// Author : 武井遥都
//
//=============================================================================
#include "pause.h"
#include "polygon.h"
#include "Texture.h"
#include "mesh.h"
#include "input.h"
#include "scene.h"
#include "Sound.h"

// ***************************************
// マクロ定義
// ***************************************
#define PAUSE_TEXTURE_PATH L"data/texture/pause.png"
#define SPAUSE_TEXTURE_PATH L"data/texture/triangle.png"
#define MAX_TEXTURE (2)


// ****************************************
// 構造体
// ****************************************
struct PAUSE
{
	XMFLOAT2 m_pos;
	XMFLOAT2 m_size;
	bool m_pause;
};

// ****************************************
// グローバル変数
// ****************************************
static MESH	g_pauseMesh;					// 構造体
static PAUSE g_pause[MAX_TEXTURE];
static ID3D11ShaderResourceView* g_pPauseTexture[MAX_TEXTURE];
static EScene g_Scene = SCENE_NONE;
static JOYINFOEX *pad; // ゲームパッド十字ボタンのため


HRESULT InitPause()
{
	ID3D11Device* pDevice = GetDevice();

	// 初期化処理
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		g_pause[0].m_pos = XMFLOAT2(0.0f, 0.0f);
		g_pause[1].m_pos = XMFLOAT2(-300.0f, -30.0f);
		g_pause[i].m_size = XMFLOAT2(0.0f, 0.0f);
		g_pause[i].m_pause = false; // デフォルトでfalse(止まっていない)
	}

	pad = GetJoyState(0);

	// テクスチャの読み込み
	HRESULT hr;
	hr = CreateTextureFromFile(pDevice, PAUSE_TEXTURE_PATH, &g_pPauseTexture[0]);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "テクスチャ読み込みエラー", "ポーズのテクスチャ", MB_OK);
		return hr;
	}
	hr = CreateTextureFromFile(pDevice, SPAUSE_TEXTURE_PATH, &g_pPauseTexture[1]);
	if (FAILED(hr))
	{
		MessageBoxA(GetMainWnd(), "テクスチャ読み込みエラー", "三角テクスチャ", MB_OK);
		return hr;
	}
	return hr;
}

void UninitPause()
{
	// メッシュ開放
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		SAFE_RELEASE(g_pPauseTexture[i]);
		g_pause[i].m_pause = false;
	}
}

bool UpdatePause()
{
	pad = GetJoyState(0);
	PrintDebugProc("state:%d\n", pad->dwPOV);

	if (GetKeyTrigger(VK_M)||GetJoyButton(0,JOYSTICKID8))
	{
			g_pause[0].m_pause = true;
			g_pause[1].m_pause = true;
	}

	if (g_pause[0].m_pause)
	{
		if (GetKeyTrigger(VK_UP) || (g_GamePadNum > 0 && (35999 >= pad->dwPOV >= 27001 || pad->dwPOV <= 8999)/*上*/))
		{
			if (g_pause[1].m_pos.y < -31.0f)
			{
				g_pause[1].m_pos.y += 130.0f;
					CSound::Play(SE_PAUSE_SELECT);
			}
		}
		if (GetKeyTrigger(VK_DOWN)||((g_GamePadNum > 0 && (pad->dwPOV >= 9001 && pad->dwPOV <= 26999))/*下*/))
		{
			if (g_pause[1].m_pos.y > -160.0f)
			{
				g_pause[1].m_pos.y -= 130.0f;
				CSound::Play(SE_PAUSE_SELECT);
			}
		}
		if ((GetKeyPress(VK_X) || (GetJoyButton(0, JOYSTICKID1)) && g_pause[1].m_pos.y == -30.0f))
		{
			g_pause[0].m_pause = false;
			g_pause[1].m_pause = false;
			CSound::Play(SE_DECIDE);
		}
		else if ((GetKeyPress(VK_X)||GetJoyButton(0,JOYSTICKID1)) && g_pause[1].m_pos.y == -160.0f)
		{
			SetScene(SCENE_STAGE);	
			CSound::Play(SE_DECIDE);
			return true;
		}
	}

	return g_pause[0].m_pause;
}

void DrawPause()
{
	if (g_pause[0].m_pause)
	{
		ID3D11DeviceContext* pDC = GetDeviceContext();
		SetPolygonSize(500, 500);
		SetPolygonPos(g_pause[0].m_pos.x, g_pause[0].m_pos.y);
		SetPolygonAngle(0.0f);
		SetPolygonTexture(g_pPauseTexture[0]);
		DrawPolygon(pDC);

		pDC = GetDeviceContext();
		SetBlendState(BS_ALPHABLEND);
		SetZBuffer(false);
		SetPolygonSize(100, 100);
		SetPolygonPos(g_pause[1].m_pos.x, g_pause[1].m_pos.y);
		SetPolygonAngle(0.0f);
		SetPolygonTexture(g_pPauseTexture[1]);
		DrawPolygon(pDC);
		SetBlendState(BS_NONE);
		SetZBuffer(true);
	}
}
