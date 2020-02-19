#include "stdafx.h"
#include "MainForm.h"
#include "EventManager.h"

#include "math.h"




class MyVec3
{
private: 
  //ここに各変数はprivate変数になり外からは見えない
  int m_priv;
  
public: 
  //ここに各変数はpublic変数になり外から参照・修正可能
  int m_publ;

  float m_data[3];
  
  //constructrue
  MyVec3(float x = 0, float y = 0, float z = 0)
  {
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
  }

  MyVec3(const MyVec3 &src)
  {
    //todo copy
    
  }

  MyVec3 &operator=(const MyVec3 &src)
  {
    //todo copy
    return *this;
  }

  //オペレーターオーバーロード
  MyVec3 operator+(const MyVec3 &v){
    MyVec3 p;
    p.m_data[0] = this->m_data[0] + v.m_data[0];
    p.m_data[1] = this->m_data[1] + v.m_data[1];
    p.m_data[2] = this->m_data[2] + v.m_data[2];
    return p;
  }

  void Trace()
  {
    std::cout << m_data[0] << m_data[1] << m_data[2];
  }
};














EventManager::EventManager()
{
  std::cout << "EventManager constructor\n";
  MyVec3 p(1,2,3);
  MyVec3 q(1,2,3);
  auto a = p + q;
  p.Trace();
  q.Trace();
  a.Trace();

  


  m_btn_right = m_btn_left = m_btn_middle = false;
}



void EventManager::BtnDownLeft  (int x, int y, OglForCLI *ogl)
{
  m_btn_left = true;
  ogl->BtnDown_Trans( EVec2i(x,y) );
} 

void EventManager::BtnDownMiddle(int x, int y, OglForCLI *ogl)
{
  m_btn_middle = true;
  ogl->BtnDown_Zoom( EVec2i(x,y) );
}

void EventManager::BtnDownRight (int x, int y, OglForCLI *ogl)
{
  m_btn_right = true;
  ogl->BtnDown_Rot( EVec2i(x,y) );
}



void EventManager::BtnUpLeft  (int x, int y, OglForCLI *ogl)
{
  m_btn_left = false;
  ogl->BtnUp();
}

void EventManager::BtnUpMiddle(int x, int y, OglForCLI *ogl)
{
  m_btn_middle = false;
  ogl->BtnUp();
}

void EventManager::BtnUpRight (int x, int y, OglForCLI *ogl)
{
  m_btn_right = false;
  ogl->BtnUp();
}

void EventManager::MouseMove    (int x, int y, OglForCLI *ogl)
{
  if ( !m_btn_right && !m_btn_middle && !m_btn_left) return;

  ogl->MouseMove( EVec2i(x,y) );
  OpenglOnCli::MainForm_RedrawPanel();
}



void EventManager::DrawScene()
{
  //ここにレンダリングルーチンを書く
  glBegin(GL_LINES );
  glColor3d(1,0,0); glVertex3d(0,0,0); glVertex3d(10,0,0);
  glColor3d(0,1,0); glVertex3d(0,0,0); glVertex3d(0,10,0);
  glColor3d(0,0,1); glVertex3d(0,0,0); glVertex3d(0,0,10);
  glEnd();

  
  const static float diff[4] = { 1.0f, 0.2f, 0, 0.3f };
  const static float ambi[4] = { 1.0f, 0.2f, 0, 0.3f };
  const static float spec[4] = { 1,1,1,0.3f };
  const static float shin[1] = { 64.0f };

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR  , spec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , ambi);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin);
  

  glBegin(GL_TRIANGLES );
  
  int Ni = 200;
  int Nj = 200;
  double p1, t1, p2, t2;
  static double PI = 3.1415926535897932384626433832795028841971693993751058209749445928078164062;

  for (int i=0; i<Ni; i++) {
	  for (int j=0; j<Nj; j++) {
		  p1 = PI  * i / (Ni) - PI/2;
		  t1 = PI * 2.0 * j / (Nj);
		  p2 = PI * (i+1) / (Ni)-PI / 2;
		  t2 = PI * 2.0 * (j+1) / (Nj);


		  
		  glNormal3f(cos(p1)*cos(t1), sin(p1), -1*cos(p1)*sin(t1));
		  glVertex3f(cos(p1)*cos(t1), sin(p1), -1*cos(p1)*sin(t1));

		  glNormal3f(cos(p2)*cos(t1), sin(p2), -1 * cos(p2)*sin(t1));
		  glVertex3f(cos(p2)*cos(t1), sin(p2), -1*cos(p2)*sin(t1));

		  glNormal3f(cos(p2)*cos(t2), sin(p2), -1 * cos(p2)*sin(t2));
		  glVertex3f(cos(p2)*cos(t2), sin(p2), -1*cos(p2)*sin(t2));



		  glNormal3f(cos(p1)*cos(t1), sin(p1), -1*cos(p1)*sin(t1));
		  glVertex3f(cos(p1)*cos(t1), sin(p1), -1 * cos(p1)*sin(t1));

		  glNormal3f(cos(p1)*cos(t2), sin(p1), -1 * cos(p1)*sin(t2));
		  glVertex3f(cos(p1)*cos(t2), sin(p1), -1 * cos(p1)*sin(t2));

		  glNormal3f(cos(p2)*cos(t2), sin(p2), -1 * cos(p2)*sin(t2));
		  glVertex3f(cos(p2)*cos(t2), sin(p2), -1*cos(p2)*sin(t2));

	  }
  }



  glEnd();
 
}


