#include "stdafx.h"
#include <iostream>

#include "MainForm.h"


using namespace OpenglOnCli;

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "User32.lib" )


//double buffering用のハック
//https://www.codeproject.com/Questions/226895/Double-buffered-painting-of-a-panel
public ref class MyPanel :  public System::Windows::Forms::Panel
{
public:
    void SetStyle(System::Windows::Forms::ControlStyles flag, System::Boolean value)
    {
        System::Windows::Forms::Control::SetStyle(flag, value);
    }
};


MainForm::MainForm(void)
{
  m_ogl = 0;
  InitializeComponent();
  m_ogl = new OglForCLI(GetDC((HWND)m_main_panel->Handle.ToPointer()));
}



System::Void MainForm::m_main_panel_Paint(
    System::Object^  sender, 
    System::Windows::Forms::PaintEventArgs^  e)
{
  RedrawMainPanel();
}



System::Void MainForm::m_main_panel_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
  m_bBtnDown = true;
  if (e->Button == System::Windows::Forms::MouseButtons::Left  ) m_ogl->BtnDown_Trans(EVec2i(e->X, e->Y));
  if (e->Button == System::Windows::Forms::MouseButtons::Middle) m_ogl->BtnDown_Zoom (EVec2i(e->X, e->Y));
  if (e->Button == System::Windows::Forms::MouseButtons::Right ) m_ogl->BtnDown_Rot  (EVec2i(e->X, e->Y));
}

System::Void MainForm::m_main_panel_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
  if(m_bBtnDown) {
    m_ogl->MouseMove(EVec2i(e->X, e->Y));
    this->RedrawMainPanel();
  }
}

System::Void MainForm::m_main_panel_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
{
  m_ogl->BtnUp();
  m_bBtnDown = false;
}




System::Void MainForm::m_main_panel_Resize(System::Object^  sender, System::EventArgs^  e)
{
}

System::Void MainForm::MainForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
  std::cout << (int) e->KeyCode << "\n";
}


System::Void MainForm::MainForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
}






void MainForm::RedrawMainPanel()
{
  float  fovY     = 45.0;
  float  nearDist = 0.1f;
  float  farDist  = 1000.0f;
  m_ogl->OnDrawBegin(m_main_panel->Width, m_main_panel->Height,   
                     fovY, nearDist, farDist);
  //ここにレンダリングルーチンを書く
  glBegin(GL_LINES );
  glColor3d(1,0,0); glVertex3d(0,0,0); glVertex3d(10,0,0);
  glColor3d(0,1,0); glVertex3d(0,0,0); glVertex3d(0,10,0);
  glColor3d(0,0,1); glVertex3d(0,0,0); glVertex3d(0,0,10);
  glEnd();

  m_ogl->OnDrawEnd();
}
