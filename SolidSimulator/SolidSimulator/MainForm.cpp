#include "stdafx.h"
#include "MainForm.h"
#include "TCore.h"

// ‘å‚«‚­‚·‚é
// pickŽž‚É‰ñ“]‚·‚é
// getNearestŽž‚É‚à‰ñ“]‚·‚é

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "User32.lib" )

using namespace SolidSimulator;



System::Void MainForm::m_main_panel_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
{
  RedrawMainPanel();
}



void MainForm::RedrawMainPanel()
{
  float  fovY     = 45.0;
  float  nearDist = 0.1f;
  float  farDist  = 1000.0f;
  m_ogl->OnDrawBegin(m_main_panel->Width, m_main_panel->Height, fovY, nearDist, farDist);

  //initialize ogl parameters
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);


  TCore::getInst()->DrawScene();

  //‚±‚±‚ÉƒŒƒ“ƒ_ƒŠƒ“ƒOƒ‹[ƒ`ƒ“‚ð‘‚­
  glLineWidth(2);
  glBegin(GL_LINES );
    glColor3d(1,0,0); glVertex3d(0,0,0); glVertex3d(10,0,0);
    glColor3d(0,1,0); glVertex3d(0,0,0); glVertex3d(0,10,0);
    glColor3d(0,0,1); glVertex3d(0,0,0); glVertex3d(0,0,10);
  glEnd();

  m_ogl->OnDrawEnd();
}






System::Void MainForm::m_main_panel_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
  if (e->Button == System::Windows::Forms::MouseButtons::Left  ) TCore::getInst()->LBtbnDown(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Middle) TCore::getInst()->MBtbnDown(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Right ) TCore::getInst()->RBtbnDown(EVec2i(e->X, e->Y), m_ogl);
}

System::Void MainForm::m_main_panel_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
  TCore::getInst()->MouseMove(EVec2i(e->X, e->Y), m_ogl);
}

System::Void MainForm::m_main_panel_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
  if (e->Button == System::Windows::Forms::MouseButtons::Left  ) TCore::getInst()->LBtbnUp(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Middle) TCore::getInst()->MBtbnUp(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Right ) TCore::getInst()->RBtbnUp(EVec2i(e->X, e->Y), m_ogl);
}


System::Void MainForm::m_main_panel_Resize(System::Object^  sender, System::EventArgs^  e) 
{

}




static void CALLBACK MyTimerProc(
    HWND hWnd,           // handle of CWnd that called SetTimer
    UINT nMsg,           // WM_TIMER
    UINT_PTR nIDEvent,   // timer identification
    DWORD dwTime         // system time
)
{
  TCore::getInst()->StepTime();
  SolidSimulator::RedrawMainWindow();
}


MainForm::MainForm(void)
{
  m_ogl = 0;
  InitializeComponent();
  m_ogl = new OglForCLI(GetDC((HWND)m_main_panel->Handle.ToPointer()));
  SetTimer( (HWND)m_main_panel->Handle.ToPointer(), 1, 10, &MyTimerProc);
}



