#pragma once


#include "OglForCLI.h"


namespace SolidSimulator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MainForm �̊T�v
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{

  public: 
    void RedrawMainPanel();

  private: 
    OglForCLI *m_ogl;
	private:
		MainForm(void);

  private:
    static MainForm^ m_singleton;

  public:
    static MainForm^ getInst()
    {
      if (m_singleton == nullptr) m_singleton = gcnew MainForm();
      return m_singleton;
    }



	protected:
		/// <summary>
		/// �g�p���̃��\�[�X�����ׂăN���[���A�b�v���܂��B
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::Panel^  m_main_panel;
  protected:

	private:
		/// <summary>
		/// �K�v�ȃf�U�C�i�[�ϐ��ł��B
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �f�U�C�i�[ �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�[�ŕύX���Ȃ��ł��������B
		/// </summary>
		void InitializeComponent(void)
		{
      this->m_main_panel = (gcnew System::Windows::Forms::Panel());
      this->SuspendLayout();
      // 
      // m_main_panel
      // 
      this->m_main_panel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
        | System::Windows::Forms::AnchorStyles::Left)
        | System::Windows::Forms::AnchorStyles::Right));
      this->m_main_panel->AutoSize = true;
      this->m_main_panel->Location = System::Drawing::Point(12, 12);
      this->m_main_panel->Name = L"m_main_panel";
      this->m_main_panel->Size = System::Drawing::Size(576, 517);
      this->m_main_panel->TabIndex = 0;
      this->m_main_panel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::m_main_panel_Paint);
      this->m_main_panel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::m_main_panel_MouseDown);
      this->m_main_panel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::m_main_panel_MouseMove);
      this->m_main_panel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::m_main_panel_MouseUp);
      this->m_main_panel->Resize += gcnew System::EventHandler(this, &MainForm::m_main_panel_Resize);
      // 
      // MainForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(600, 541);
      this->Controls->Add(this->m_main_panel);
      this->Name = L"MainForm";
      this->Text = L"MainForm";
      this->ResumeLayout(false);
      this->PerformLayout();

    }

#pragma endregion
  private: System::Void m_main_panel_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
  private: System::Void m_main_panel_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
  private: System::Void m_main_panel_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
  private: System::Void m_main_panel_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
  private: System::Void m_main_panel_Resize(System::Object^  sender, System::EventArgs^  e);
  };


  inline void RedrawMainWindow(){
    MainForm::getInst()->RedrawMainPanel();
  }



}

