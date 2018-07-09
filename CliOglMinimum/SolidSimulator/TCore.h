#pragma once

#include "OglForCLI.h"

class TCore
{
  //singleton
  //競合を避けるため，確実にsingle threadである段階でgetInstを一度読んで初期化を行う

private:
  TCore();
public:
  static TCore* getInst(){
    static TCore p;
    return &p;
  }

private:
  bool m_bL, m_bR, m_bM; //mouse button flg

public:
  ~TCore();

  void LBtbnDown(EVec2i &p, OglForCLI *ogl);
  void RBtbnDown(EVec2i &p, OglForCLI *ogl);
  void MBtbnDown(EVec2i &p, OglForCLI *ogl);
  void LBtbnUp  (EVec2i &p, OglForCLI *ogl);
  void RBtbnUp  (EVec2i &p, OglForCLI *ogl);
  void MBtbnUp  (EVec2i &p, OglForCLI *ogl);
  void MouseMove(EVec2i &p, OglForCLI *ogl);
  void DrawScene();

};

