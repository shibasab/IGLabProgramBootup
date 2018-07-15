#include "stdafx.h"
#include "TCore.h"
#include "MainForm.h"


#define FLOOR_Y 0
#define REFL_COEF 0.2 


TCore::TCore()
{
  m_bL = m_bR = m_bM = false; 

  m_drag_index = -1;
  m_drag_trgt_pos << 0, 0, 0;

}


TCore::~TCore()
{
}





static void drawFloor()
{
	glDisable(GL_CULL_FACE);
	static float ambi[4] = { 0.3f, 0.3f, 0.3f, 0.1f};
	static float diff[4] = { 0.7f, 0.7f, 0.3f, 0.1f};
	static float spec[4] = { 0.3f, 0.3f, 0   , 0.1f};
	static float shin[1] = { 64.0f };
	
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT  , ambi );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE  , diff );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR , spec );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shin );

	const double R = 200;
	glEnable( GL_LIGHTING );
	glBegin( GL_TRIANGLES );
		glNormal3d( 0,1,0 );
		glVertex3d( -R, FLOOR_Y, -R);
		glVertex3d(  R, FLOOR_Y,  R);
		glVertex3d(  R, FLOOR_Y, -R);

		glVertex3d( -R, FLOOR_Y, -R);
		glVertex3d( -R, FLOOR_Y,  R);
		glVertex3d(  R, FLOOR_Y,  R);
	glEnd();

	double thick = 0.1;
	glEnable(GL_BLEND );
	glBegin( GL_TRIANGLES );
		glNormal3d( 0,1,0 );
		glVertex3d( -R, FLOOR_Y+ thick, -R);
		glVertex3d(  R, FLOOR_Y+ thick,  R);
		glVertex3d(  R, FLOOR_Y+ thick, -R);

		glVertex3d( -R, FLOOR_Y+ thick, -R);
		glVertex3d( -R, FLOOR_Y+ thick,  R);
		glVertex3d(  R, FLOOR_Y+ thick,  R);
	glEnd();
	glDisable(GL_BLEND );

	glEnable(GL_CULL_FACE);
}




void TCore::DrawScene()
{
	if( m_drag_index != -1 ) 
  {
    glDisable(GL_LIGHTING);
    glLineWidth(10);
    EVec3f p = m_obj.getVertexPos(m_drag_index);
		glBegin(GL_LINES);
    glVertex3fv(p.data());
    glVertex3fv(m_drag_trgt_pos.data());
    glEnd();
  }

  drawFloor();
  m_obj.draw();
}


static float GRAVITY_FORCE = 10;
static float FRIC_COEF     = 0.95f;


void TCore::StepTime()
{

  //5回iterationを回した後に1回描画する
  for( int count = 0; count < 5; ++count)
  {
	  EVec3f total_force(0,0,0), total_tork(0,0,0);
    EVec3f const_velocityCoef(1,1,1), const_translate(0,0,0);

    const EVec3f gcP = m_obj.GetGCPos ();
    const EVec3f gcV = m_obj.GetGCVelo();


	  if( m_drag_index != -1 ) 
    {
      EVec3f p = m_obj.getVertexPos(m_drag_index);

		  EVec3f f = m_drag_trgt_pos - p;
		  total_force += f;
		  total_tork  += ( p - gcP).cross( f );
	  }

    //gravity
	  total_force += EVec3f(0,-GRAVITY_FORCE,0);


	  //collision with floor
    vector<EVec3f> vs = m_obj.GetVertsUnderFloor( FLOOR_Y );

    if( vs.size() != 0)
    {
      //下方向の速度をなくし，めり込み分を上に持ち上げる
      if( gcV[1] < 0) const_velocityCoef[1] = 0;

      float lowestY = 0;
      for( int i=0; i <(int)vs.size(); ++i) lowestY = min(lowestY, vs[i][1]);
      const_translate[1] = -lowestY;


      //抗力
      EVec3f dragForce = EVec3f(0,GRAVITY_FORCE,0);
		  total_force += dragForce;

      //TODO 動摩擦力によるトルク

      //抗力（トルク）
      EVec3f drafForceOnVert = EVec3f(0,GRAVITY_FORCE,0) / (float) vs.size();
      for( int i=0; i <(int)vs.size(); ++i)
			  total_tork += ( vs[i] - gcP ).cross(drafForceOnVert);

    }

    m_obj.StepSimulation( 0.05f, total_force, total_tork, const_velocityCoef, const_translate);
  }

}






void TCore::LBtbnDown(EVec2i &p, OglForCLI *ogl)
{
  m_bL = true;
  m_drag_index = -1;
 
	EVec3f rayP, rayD;
	ogl->GetCursorRay( p, rayP, rayD );

	EVec3f pos;
  int    vIdx;
  if( m_obj.pickSurfaceByRay(rayP, rayD, pos, vIdx) )
  {
    m_drag_index    = vIdx;
    m_drag_trgt_pos = pos;
  }

  if( m_drag_index == -1 ) ogl->BtnDown_Trans(p);
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
  m_drag_index = -1;
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
 
	if( m_drag_index != -1 )
	{
		EVec3f rayP, rayD;
		ogl->GetCursorRay( p, rayP, rayD );

    double depth = (rayP - m_drag_trgt_pos).norm();
    m_drag_trgt_pos = rayP + rayD.normalized() * depth;
	}
	else
    ogl->MouseMove(p);
}

