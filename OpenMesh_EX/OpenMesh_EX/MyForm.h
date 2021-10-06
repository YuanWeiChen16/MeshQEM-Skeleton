#pragma once

#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"

Tri_Mesh *mesh;
std::vector<Tri_Mesh> mesh_serial;
int edgeCount;
int Allcount = 99;
int simplifyCount;
int average;
std::vector <int> percent;
xform xf;
GLCamera camera;
float fov = 0.7f;

static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};
Mouse::button Mouse_State = Mouse::ROTATE;

namespace OpenMesh_EX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  loadModelToolStripMenuItem;
	private: System::Windows::Forms::OpenFileDialog^  openModelDialog;
	private: System::Windows::Forms::SaveFileDialog^  saveModelDialog;
	private: System::Windows::Forms::ToolStripMenuItem^  saveModelToolStripMenuItem;
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::HScrollBar^ hScrollBar1;
	private: System::Windows::Forms::Label^ Last_percent;
	private: System::Windows::Forms::NumericUpDown^ numericUpDown1;
	private: System::Windows::Forms::GroupBox^ GroupSimplify;
	private: System::Windows::Forms::Label^ SimplifyTo_percent;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::NumericUpDown^ numericUpDown2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::NumericUpDown^ numericUpDown3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::Label^ n_edges;
	private: System::Windows::Forms::Label^ n_faces;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label5;


			 //private: System::Windows::Forms::HScrollBar^ hscroll;
	protected:

	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->Last_percent = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupSimplify = (gcnew System::Windows::Forms::GroupBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->SimplifyTo_percent = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->n_edges = (gcnew System::Windows::Forms::Label());
			this->n_faces = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			this->GroupSimplify->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(825, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->loadModelToolStripMenuItem,
					this->saveModelToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadModelToolStripMenuItem
			// 
			this->loadModelToolStripMenuItem->Name = L"loadModelToolStripMenuItem";
			this->loadModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->loadModelToolStripMenuItem->Text = L"Load Model";
			this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
			// 
			// saveModelToolStripMenuItem
			// 
			this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
			this->saveModelToolStripMenuItem->Size = System::Drawing::Size(144, 22);
			this->saveModelToolStripMenuItem->Text = L"Save Model";
			this->saveModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveModelToolStripMenuItem_Click);
			// 
			// openModelDialog
			// 
			this->openModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openModelDialog_FileOk);
			// 
			// saveModelDialog
			// 
			this->saveModelDialog->DefaultExt = L"obj";
			this->saveModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveModelDialog_FileOk);
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 24);
			this->hkoglPanelControl1->MaximumSize = System::Drawing::Size(613, 430);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(613, 430);
			this->hkoglPanelControl1->TabIndex = 2;
			this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::hkoglPanelControl1_executesimplify);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(10, 115);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(172, 25);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Least Square";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(10, 69);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(172, 25);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Simplify";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->Location = System::Drawing::Point(3, 120);
			this->hScrollBar1->Maximum = 109;
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(160, 18);
			this->hScrollBar1->TabIndex = 5;
			this->hScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &MyForm::hScrollBar1_Scroll);
			// 
			// Last_percent
			// 
			this->Last_percent->AutoSize = true;
			this->Last_percent->Font = (gcnew System::Drawing::Font(L"新細明體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Last_percent->Location = System::Drawing::Point(166, 120);
			this->Last_percent->Name = L"Last_percent";
			this->Last_percent->Size = System::Drawing::Size(16, 16);
			this->Last_percent->TabIndex = 6;
			this->Last_percent->Text = L"0";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->numericUpDown1->Font = (gcnew System::Drawing::Font(L"新細明體", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->numericUpDown1->Location = System::Drawing::Point(21, 37);
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(140, 22);
			this->numericUpDown1->TabIndex = 7;
			this->numericUpDown1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown1_ValueChanged);
			// 
			// GroupSimplify
			// 
			this->GroupSimplify->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->GroupSimplify->Controls->Add(this->label2);
			this->GroupSimplify->Controls->Add(this->SimplifyTo_percent);
			this->GroupSimplify->Controls->Add(this->label1);
			this->GroupSimplify->Controls->Add(this->button2);
			this->GroupSimplify->Controls->Add(this->numericUpDown1);
			this->GroupSimplify->Controls->Add(this->hScrollBar1);
			this->GroupSimplify->Controls->Add(this->Last_percent);
			this->GroupSimplify->Location = System::Drawing::Point(625, 27);
			this->GroupSimplify->Name = L"GroupSimplify";
			this->GroupSimplify->Size = System::Drawing::Size(200, 141);
			this->GroupSimplify->TabIndex = 8;
			this->GroupSimplify->TabStop = false;
			this->GroupSimplify->Text = L"Simplification";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 101);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 12);
			this->label2->TabIndex = 10;
			this->label2->Text = L"DisplayLevel";
			// 
			// SimplifyTo_percent
			// 
			this->SimplifyTo_percent->AutoSize = true;
			this->SimplifyTo_percent->Font = (gcnew System::Drawing::Font(L"新細明體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->SimplifyTo_percent->Location = System::Drawing::Point(167, 39);
			this->SimplifyTo_percent->Name = L"SimplifyTo_percent";
			this->SimplifyTo_percent->Size = System::Drawing::Size(16, 16);
			this->SimplifyTo_percent->TabIndex = 9;
			this->SimplifyTo_percent->Text = L"1";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 12);
			this->label1->TabIndex = 8;
			this->label1->Text = L"SimplifyTo";
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->groupBox1->Controls->Add(this->numericUpDown3);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->numericUpDown2);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Location = System::Drawing::Point(625, 286);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(200, 156);
			this->groupBox1->TabIndex = 9;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Skeleton";
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->DecimalPlaces = 3;
			this->numericUpDown3->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 196608 });
			this->numericUpDown3->Location = System::Drawing::Point(21, 82);
			this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(140, 22);
			this->numericUpDown3->TabIndex = 7;
			this->numericUpDown3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->numericUpDown3->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown3_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(8, 66);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(18, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"SL";
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->DecimalPlaces = 3;
			this->numericUpDown2->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 196608 });
			this->numericUpDown2->Location = System::Drawing::Point(21, 37);
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(140, 22);
			this->numericUpDown2->TabIndex = 5;
			this->numericUpDown2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &MyForm::numericUpDown2_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 22);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(30, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"WH0";
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->n_edges);
			this->groupBox2->Controls->Add(this->n_faces);
			this->groupBox2->Location = System::Drawing::Point(625, 180);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(200, 90);
			this->groupBox2->TabIndex = 10;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"MeshStatus";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(16, 47);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(31, 12);
			this->label6->TabIndex = 3;
			this->label6->Text = L"edges";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(19, 18);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(28, 12);
			this->label5->TabIndex = 2;
			this->label5->Text = L"faces";
			// 
			// n_edges
			// 
			this->n_edges->AutoSize = true;
			this->n_edges->Location = System::Drawing::Point(62, 47);
			this->n_edges->Name = L"n_edges";
			this->n_edges->Size = System::Drawing::Size(11, 12);
			this->n_edges->TabIndex = 1;
			this->n_edges->Text = L"0";
			// 
			// n_faces
			// 
			this->n_faces->AutoSize = true;
			this->n_faces->Location = System::Drawing::Point(62, 18);
			this->n_faces->Name = L"n_faces";
			this->n_faces->Size = System::Drawing::Size(11, 12);
			this->n_faces->TabIndex = 0;
			this->n_faces->Text = L"0";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(825, 454);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->GroupSimplify);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			this->GroupSimplify->ResumeLayout(false);
			this->GroupSimplify->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e)
	{

	}
	private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
	{
		glEnable(GL_COLOR_MATERIAL);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		point center;
		center[0] = 0.0;
		center[1] = 0.0;
		center[2] = 0.0;
		camera.setupGL(xf * center, 1.0);

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glMultMatrixd((double *)xf);
		if (mesh != NULL)
			mesh->Render_SolidWireframe();
		glPopMatrix();
	}
	private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left ||
			e->Button == System::Windows::Forms::MouseButtons::Middle)
		{
			point center;
			Mouse_State = Mouse::NONE;
			center[0] = 0.0;
			center[1] = 0.0;
			center[2] = 0.0;
			camera.mouse(e->X, e->Y, Mouse_State,
				xf * center,
				1.0, xf);
		}
	}
	private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			point center;
			Mouse_State = Mouse::ROTATE;
			center[0] = 0.0;
			center[1] = 0.0;
			center[2] = 0.0;
			camera.mouse(e->X, e->Y, Mouse_State,
				xf * center,
				1.0, xf);
			hkoglPanelControl1->Invalidate();
		}

		if (e->Button == System::Windows::Forms::MouseButtons::Middle)
		{
			point center;
			Mouse_State = Mouse::MOVEXY;
			center[0] = 0.0;
			center[1] = 0.0;
			center[2] = 0.0;
			camera.mouse(e->X, e->Y, Mouse_State,
				xf * center,
				1.0, xf);
			hkoglPanelControl1->Invalidate();
		}
	}
	private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (e->Delta < 0)
		{
			point center;
			Mouse_State = Mouse::WHEELDOWN;
			center[0] = 0.0;
			center[1] = 0.0;
			center[2] = 0.0;
			camera.mouse(e->X, e->Y, Mouse_State,
				xf * center,
				1.0, xf);
			hkoglPanelControl1->Invalidate();
		}
		else
		{
			point center;
			Mouse_State = Mouse::WHEELUP;
			center[0] = 0.0;
			center[1] = 0.0;
			center[2] = 0.0;
			camera.mouse(e->X, e->Y, Mouse_State,
				xf * center,
				1.0, xf);
			hkoglPanelControl1->Invalidate();
		}
	}
	private: System::Void loadModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		openModelDialog->Filter = "Model(*.obj)|*obj";
		openModelDialog->Multiselect = false;
		openModelDialog->ShowDialog();
	}
	private: System::Void openModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		std::string filename;
		MarshalString(openModelDialog->FileName, filename);

		if (mesh != NULL)
			delete mesh;

		mesh = new Tri_Mesh;

		if (ReadFile(filename, mesh))
			std::cout << filename << std::endl;
		mesh_serial.clear();
		//mesh_serial.push_back(*mesh);

		Allcount = 100;
		simplifyCount = 0;
		percent.clear();
		//test Update ErrorMatrix
		mesh->Model_Init_Property();
		mesh->ErrorQuadricsMatrix();
		edgeCount = mesh->n_edges();
		//mesh->testBox();
		//mesh->testBox();
		hkoglPanelControl1->Invalidate();
		n_faces->Text = mesh->n_faces().ToString();
		n_edges->Text = mesh->n_edges().ToString();
	}
	private: System::Void saveModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		saveModelDialog->Filter = "Model(*.obj)|*obj";
		saveModelDialog->ShowDialog();
	}
	private: System::Void saveModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		std::string filename;
		MarshalString(saveModelDialog->FileName, filename);

		if (SaveFile(filename, mesh))
			std::cout << filename << std::endl;
	}
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		if (mesh != NULL)
		{
			if (mesh->t == 0)
			{
				mesh->Model_Init_Property();
			}
			mesh->InitEH0 = double(numericUpDown2->Value);
			mesh->InitSL = double(numericUpDown3->Value);
			mesh->LSMesh(mesh->t);
			mesh->t += 1;
		}
	}
	private: System::Void hkoglPanelControl1_executesimplify(System::Object^ sender, KeyEventArgs^ e)
	{
		if (e->KeyCode == Keys::S)
		{
			double angleT = 0, matrixT = 0, vectorT = 0;
			if (mesh->simplification(angleT, matrixT, vectorT))
			{
				if (mesh->simplification(angleT, matrixT, vectorT))
				{
					std::cout << "Simplify One Time.\n";
					std::cout << "angleT: " << angleT << "\tmatrixT: " << matrixT << "\tvectorT: " << vectorT << "\n";
					mesh->garbage_collection();
					hkoglPanelControl1->Invalidate();
					n_faces->Text = mesh->n_faces().ToString();
					n_edges->Text = mesh->n_edges().ToString();
				}
				else std::cout << "Cannot Simplify.\n";
			}
		}
		if (e->KeyCode == Keys::K)
		{
			mesh_serial.push_back(*mesh);
		}
		if (e->KeyCode == Keys::D)
		{
		   mesh->scale = mesh->scale <= 0.01 ? 0.01 : mesh->scale / 10;
		}
		if (e->KeyCode == Keys::A)
		{
		   mesh->scale = mesh->scale * 10;
		}
		if (e->KeyCode == Keys::F)
		{
		   mesh->saveFile();
		}
	}
	private: System::Void hScrollBar1_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e)
	{

		if (hScrollBar1->Value > mesh_serial.size())
			hScrollBar1->Value = mesh_serial.size();
		if (mesh_serial.size() > 0)
		{
			int index = mesh_serial.size() - hScrollBar1->Value - 1;
			index = index < 0 ? 0 : index;
			if (&mesh_serial[index] == NULL) {
				std::cout << "Mesh Null.\n";
			}
			else
			{
				*mesh = mesh_serial[index];
				mesh->Render_SolidWireframe();
				n_faces->Text = mesh->n_faces().ToString();
				n_edges->Text = mesh->n_edges().ToString();
			}
			hkoglPanelControl1->Invalidate();

		}
		Last_percent->Text = hScrollBar1->Value.ToString();
	}
	private: System::Void numericUpDown1_ValueChanged(System::Object^ sender, System::EventArgs^ e)
	{
		SimplifyTo_percent->Text = numericUpDown1->Value.ToString();
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e)
	{
		int original_edge_size = mesh->n_edges();
		int count = 0, tmpedge;
		//int edgeNum = mesh->n_faces();
		int edgeNum = 0;
		double angleT = 0, matrixT = 0, vectorT = 0;
		clock_t start, end;
		start = clock();
		simplifyCount = 0;
		mesh_serial.push_back(*mesh);
		while (Allcount > Decimal::ToInt32(numericUpDown1->Value))
		{
			if (mesh->simplification(angleT, matrixT, vectorT))
			{
				simplifyCount++;
				//mesh->update_face_normals();
				edgeNum = edgeCount - returnEdgeSize();

				if (percent.size() >= 75)
				{
					int partCount = simplifyCount - percent[percent.size() - 1];
					if (partCount >= (average * 0.3))
					{
						percent.push_back(simplifyCount);
						mesh_serial.push_back(*mesh);
						mesh_serial[mesh_serial.size() - 1].garbage_collection();
						std::cout << Allcount << " % Model Save\n";
						Allcount -= 1;
					}

				}
				else if (edgeNum < edgeCount * (Allcount / 100.0))
				{
					percent.push_back(simplifyCount);
					if (percent.size() == 75) {
						average = 0;
						for (int x = 0; x < percent.size(); x++)
						{
							average += percent[x];
						}
						average /= percent.size();
					}
					mesh_serial.push_back(*mesh);
					mesh_serial[mesh_serial.size() - 1].garbage_collection();

					std::cout << Allcount << " % Model Save\n";
					std::cout << "angleT: " << angleT << "\tmatrixT: " << matrixT << "\tvectorT: " << vectorT << "\n";
					Allcount -= 1;
				}
				//mesh_serial.push_back(*mesh);
				//mesh_face_count.push_back((*mesh).n_faces());
			}
			else break;
		}
		mesh_serial.push_back(*mesh);
		mesh_serial[mesh_serial.size() - 1].garbage_collection();
		mesh->garbage_collection();
		std::cout << "edge: " << mesh->n_edges() << "\t" << edgeCount << "\n";
		end = clock();
		std::cout << double(end - start) / CLOCKS_PER_SEC << " second\n";

		hkoglPanelControl1->Invalidate();
		n_faces->Text = mesh->n_faces().ToString();
		n_edges->Text = mesh->n_edges().ToString();
	};
	private: System::Void numericUpDown2_ValueChanged(System::Object^ sender, System::EventArgs^ e) 
	{
		mesh->InitEH0 = Decimal::ToDouble(numericUpDown2->Value);
		

	}
	private: System::Void numericUpDown3_ValueChanged(System::Object^ sender, System::EventArgs^ e)
	{
		mesh->InitSL = Decimal::ToDouble(numericUpDown3->Value);
	}
};
}
