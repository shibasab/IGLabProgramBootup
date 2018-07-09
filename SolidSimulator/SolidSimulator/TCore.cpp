#include "stdafx.h"
#include "TCore.h"
#include "MainForm.h"

TCore::TCore()
{
  m_bL = m_bR = m_bM = false; 
}


TCore::~TCore()
{
}


void TCore::LBtbnDown(EVec2i &p, OglForCLI *ogl)
{
  m_bL = true;
  ogl->BtnDown_Trans(p);
}
void TCore::RBtbnDown(EVec2i &p, OglForCLI *ogl)
{
  m_bR = true;
  ogl->BtnDown_Rot(p);
}
void TCore::MBtbnDown(EVec2i &p, OglForCLI *ogl)
{
  m_bM = true;
  ogl->BtnDown_Zoom(p);
}

void TCore::LBtbnUp  (EVec2i &p, OglForCLI *ogl){
  m_bL = false;
  ogl->BtnUp();
}

void TCore::RBtbnUp  (EVec2i &p, OglForCLI *ogl)
{
  m_bR = false;
  ogl->BtnUp();
}

void TCore::MBtbnUp  (EVec2i &p, OglForCLI *ogl)
{
  m_bM = false;
  ogl->BtnUp();
}

void TCore::MouseMove(EVec2i &p, OglForCLI *ogl)
{
  if(!m_bL && !m_bR && !m_bM ) return;
  ogl->MouseMove(p);
  SolidSimulator::RedrawMainWindow();
}

