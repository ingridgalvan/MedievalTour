////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <math.h> 

#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "terrainshaderclass.h"
#include "watershaderclass.h"
#include "lightclass.h"
#include "skydomeClass.h"
#include "skydomeshaderclass.h"
#include "billboardclass.h"
#include "billboardshaderclass.h"
#include "model.h"
#include "groupshaderclass.h"
#include "sphereclass.h"
#include "diffuseshaderclass.h"

#include "bitmapclass.h"
#include "textureshaderclass.h"

#include "stadistics.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();
	// estos puedo quitarlos 
	bool resetGame;

	BoundingSphere* globalSphere;
	

private:
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain, *m_Water;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;
	TerrainShaderClass* m_TerrainShader;
	SkydomeShaderClass* m_SkydomeShader;
	WaterShaderClass* m_WaterShader;
	BillboardShaderClass* m_BillboardShader;
	LightClass* m_Light;
	ModelClass* m_Skydome, *m_Billboard[3];
	DiffuseShaderClass *m_DiffuseShader;
	GroupShaderClass* m_GroupShader;

	stadistics* m_stadistics;

	//HUD
	TextureShaderClass* m_TextureShader;
	D3DXMATRIX m_baseStaticMatrix;
	

	//Models
	Model* m_Principal, *m_Cup, *m_building01, *m_building01_1, *m_building02, *m_building03,*m_building04,
		*m_building05, *m_building06, *m_building06_1, *m_building06_2, *m_building06A, *m_building04_1,
		 *m_car01, *m_car02,
		*m_floor01, *m_floor02;
};

#endif