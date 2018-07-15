#pragma once

#include "OglForCLI.h"
#include "tmesh.h"


//todo 成田論文
//todo UIを作製
//todo simulationをつなぐ


#define OBJECT_TOTAL_WEIGHT 10.0f

//Rigid Object 
class TRigidObj
{
	EVec3f m_p; //重心のposition
	EVec3f m_v; //重心のvelocity
	EMat3f m_R; //重心を中心とした姿勢 本来は 3D vectorでも表せる 
	EVec3f m_w; //重心を中心とした姿勢角速度
	EMat3f m_I; //慣性モーメントテンソル
	
  TMesh  m_obj; //形状モデル


public:
	TRigidObj(){
    m_obj.initializeAsCube( 30, 20, 10);
    initializeConfiguration();
	}

  EVec3f GetGCVelo(){ return m_v;}
  EVec3f GetGCPos (){ return m_p;}


  //重心をlocal coordinateの原点に, 慣性モーメント行列も計算
	void initializeConfiguration() 
	{
    m_p << 0,10,0;
    m_v << 0,10,0;
    m_w << 0,0,0;
    m_R.setIdentity();
    m_I.setIdentity();

		EVec3f g = m_obj.getGravityCenter();
		m_obj.Translate( -g );

    m_I << 0,0,0, 0,0,0, 0,0,0;
    for( int i=0, s = m_obj.m_vSize; i < s; ++i)
		{
      float mi = OBJECT_TOTAL_WEIGHT / (float)s;
			float x  = m_obj.m_vVerts[i][0];
			float y  = m_obj.m_vVerts[i][1];
			float z  = m_obj.m_vVerts[i][2];
			m_I(0,0)  += y*y + z*z;  m_I(0,1) += -y*x     ;  m_I(0,2) += -z*x      ; 
			m_I(1,0)  += -x*y     ;  m_I(1,1) += x*x + z*z;  m_I(1,2) += -z*y      ; 
			m_I(2,0)  += -x*z     ;  m_I(2,1) += -y*z     ;  m_I(2,2) +=  x*x + y*y; 
		}
	}


  

	//forces :pos & dir
	void StepSimulation( 
      const float  time_step, 
      const EVec3f &total_force, 
      const EVec3f &total_tork,
      const EVec3f &const_veloCoef,
      const EVec3f &const_trans)
	{
    m_v[0] *= const_veloCoef[0];
    m_v[1] *= const_veloCoef[1];
    m_v[2] *= const_veloCoef[2];
    m_p += const_trans;
		//position p = p + v * dt
    EVec3f dp = m_v * time_step;
		m_p = m_p + dp;

    //velocity v = v + a * dt
		m_v = m_v + (total_force / OBJECT_TOTAL_WEIGHT) * time_step;

    //rotation R = dR * R   (dR は 回転ベクトル m_w*dt より得られる回転行列)
    if( m_w.norm() > 0.001 )
    {
      EMat3f dR;
      dR = Eigen::AngleAxisf( m_w.norm() * time_step, m_w.normalized() );
      m_R = dR * m_R;
    }
    //anguler velocity w = w + (RIR^-1)^-1 * tork * dt
		EMat3f Rinv = m_R.inverse();
		EMat3f Iinv = m_I.inverse();

		EVec3f dw = time_step * (m_R * Iinv * Rinv) * total_tork;
		//if( dw.norm() > 0.01 ) 
    m_w = m_w + dw;
		
		m_v *= 0.99f;
		m_w *= 0.9995f;

	}

	void draw(){
		static float diff[4] = { 0.7f, 0.7f, 0.7f,1};
		static float ambi[4] = { 0.2f, 0.2f, 0.7f, 1};
		static float spec[4] = { 1.0f, 1.0f, 1.0f, 1};
		static float shin[4] = { 0.7f, 0.7f, 0.7f, 1};



		float A[16];
		A[0] = m_R(0,0); A[4] = m_R(0,1); A[ 8] = m_R(0,2); A[12] = m_p[0];
		A[1] = m_R(1,0); A[5] = m_R(1,1); A[ 9] = m_R(1,2); A[13] = m_p[1];
		A[2] = m_R(2,0); A[6] = m_R(2,1); A[10] = m_R(2,2); A[14] = m_p[2];
		A[3] = 0       ; A[7] =     0   ; A[11] =  0      ; A[15] = 1;

		glPushMatrix();
		glMultMatrixf( A );
      glEnable( GL_LIGHTING );
      m_obj.draw(diff, ambi, spec, shin);
		  glDisable( GL_LIGHTING);
    
      glLineWidth(10);
		  glBegin( GL_LINES );
		  glColor3d(1,0,0); glVertex3d(0,0,0); glVertex3d(10,0,0);
		  glColor3d(0,1,0); glVertex3d(0,0,0); glVertex3d(0,10,0);
		  glColor3d(0,0,1); glVertex3d(0,0,0); glVertex3d(0,0,10);
		  glEnd();
		glPopMatrix();
	}

  EVec3f getVertexPos(int i){
    return m_R * m_obj.m_vVerts[ i ] + m_p;
  }


  vector<EVec3f> GetVertsUnderFloor( float floor_y )
  {
    vector<EVec3f> vs;
    for( int i=0; i < m_obj.m_vSize; ++i)
    {
      EVec3f v = m_R * m_obj.m_vVerts[i] + m_p;
      if( v[1] < floor_y ) vs.push_back(v);
    }
    return vs;
  }

  // posはpickしたsurface上の位置
  // vIdxは最も近いメッシュ頂点のidx
  bool pickSurfaceByRay(const EVec3f &rayP, const EVec3f &rayD, EVec3f &pos, int &vIdx)
  {
    //rayP, rayD で  M*v　をpickする事は...
    //A*rayP, A*rayD で v をpickするのと同じ事 (A = M^(-1))

    // |R t|   |R' t'|     |RR'  Rt' + t|       R' = R^-1, t' = -R*t 
    // |0 1| * |0  1 |  =  |0        1  | = I

    EMat3f inv_R = m_R.inverse();
    EVec3f inv_t = -( inv_R * m_p);
    EVec3f rayP_mod = inv_R * rayP + inv_t;
    EVec3f rayD_mod = inv_R * rayD;
    
    if( m_obj.pickByRay(rayP_mod, rayD_mod, pos))
    {
      vIdx = m_obj.getCrosestVtxIdx(pos);
      pos  = m_R * pos + m_p;
      return true;
    }
    return false;
  }

	

};

















class TCore
{
  //singleton
  //競合を避けるため，確実にsingle threadである段階でgetInstを一度呼んで初期化を行う

  TRigidObj m_obj;

private:
  TCore();

public:
  static TCore* getInst(){
    static TCore p;
    return &p;
  }

private:
  bool m_bL, m_bR, m_bM; //mouse button flg

  int    m_drag_index   ; //-1: if false
  EVec3f m_drag_trgt_pos;


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

  void StepTime();


};

