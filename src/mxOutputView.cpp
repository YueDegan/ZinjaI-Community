#include <wx/file.h>
#include <wx/textctrl.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/gauge.h>
#include "mxOutputView.h"
#include "mxMainWindow.h"
#include "mxBitmapButton.h"
#include "mxSizers.h"
#include "ConfigManager.h"
#include "ids.h"
#include "mxUtils.h"
#include "Language.h"

BEGIN_EVENT_TABLE(mxOutputView, wxFrame)
	EVT_BUTTON(wxID_OK,mxOutputView::OnOkButton)
	EVT_BUTTON(wxID_CANCEL,mxOutputView::OnExtraButton)
	EVT_CLOSE(mxOutputView::OnClose)
	EVT_TIMER(mxID_TIMER_OUTPUT, mxOutputView::OnTimer)
	EVT_END_PROCESS(wxID_ANY, mxOutputView::OnProcessTerminate)
END_EVENT_TABLE()
	
/**
* @param caption       title for this dialog
* @param extra_mode    mode for the additional button (mxOV_EXTRA_*)
* @param extra_label   if extra_mode!=mxOV_EXTRA_NULL, label for the additional button
* @param extra_command if extra_mode!=mxOV_EXTRA_NULL, file/URL to be opened/launched by the additional button
* @param output_mode   mode for openning results pannel in main_window (mxOV_OUTPUT_*)
* @param output_file   if output_mode!=mxVO_NULL, the full path for the output file
**/
mxOutputView::mxOutputView(wxString caption, wxString output_file, bool show_progress_bar) 
	: wxFrame(main_window, wxID_ANY, caption, wxDefaultPosition, wxSize(600,500) /*,wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER*/ ) 
{
	process = nullptr;
	working = false;
	win_caption = caption;
	
	textfile = output_file.IsEmpty() ? nullptr : new wxFile(output_file,wxFile::write);
	
	timer = new wxTimer(GetEventHandler(),mxID_TIMER_OUTPUT);
	
	wxBoxSizer *mySizer = new wxBoxSizer(wxVERTICAL);
	
	mySizer->Add(new wxStaticText(this,wxID_ANY,LANG(LAUNCH_STANDAR_OUTPUT,"Salida estandar")),sizers->BLRT5_Exp0);
	ctrl_std = new wxTextCtrl(this,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	mySizer->Add(ctrl_std,sizers->BA5_Exp1);
	
	mySizer->Add(new wxStaticText(this,wxID_ANY,LANG(LAUNCH_ERROR_OUTPUT,"Salida de errores")),sizers->BLRT5_Exp0);
	ctrl_err = new wxTextCtrl(this,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	mySizer->Add(ctrl_err,sizers->BA5_Exp1);
	
	wxBoxSizer *statusSizer = new wxBoxSizer(wxHORIZONTAL);
	statusSizer->Add(state = new wxStaticText(this,wxID_ANY,LANG(LAUNCH_STATUS_STARTING,"Estado: Comenzando")),sizers->BA5_Exp1);
	if (show_progress_bar) {
		statusSizer->Add(progress_bar = new wxGauge(this,wxID_ANY,100) ,sizers->BA5_Exp1);
	} else {
		progress_bar = nullptr;
	}
	
	mySizer->Add(statusSizer,sizers->BA5_Exp0);
	
	
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	
	close_button= new mxBitmapButton (this,wxID_OK,bitmaps->buttons.ok,LANG(GENERAL_CLOSE_BUTTON,"&Cerrar"));
//	SetEscapeId(wxID_OK);
	close_button->SetDefault(); 
	close_button->Enable(false);
	
	extra_button = new mxBitmapButton (this,wxID_CANCEL,bitmaps->buttons.cancel,LANG(GENERA_STOP_BUTTON,"Detener"));
	buttonSizer->Add(extra_button,sizers->BA5);
	extra_button->Enable(false);
	
	buttonSizer->Add(close_button,sizers->BA5);
	
	mySizer->Add(buttonSizer,sizers->Center);
	
	SetSizer(mySizer);
//	close_button->SetFocus();
	
	Show();
	
}

mxOutputView::~mxOutputView() {
	delete textfile;
}

void mxOutputView::OnOkButton(wxCommandEvent &evt) {
	Close();
}

void mxOutputView::OnClose(wxCloseEvent &event) {
	timer->Stop();
	if (process) process->Detach();
	Destroy();
}

// cppcheck-suppress publicAllocationError
void mxOutputView::Launch(wxString path, const wxString &command, mxOutputView::OnProcTerminate_t opt_callback) {
	process = new wxProcess(GetEventHandler(),wxID_ANY);
	process->Redirect();
	m_opt_callback = opt_callback;
//	command = command.Mid(0,4000); ???
	
	RaiiWorkDirChanger cwd_guard(path); // set temp cwd
	ZLINF2("OutputView","cmd: "<<command);
	pid = wxExecute(command,wxEXEC_ASYNC,process);
	ZLINF2("OutputView","pid: "<<pid);
	cwd_guard.RestoreNow();
	Launched(process,pid,command);
}

static void LogInOutput_aux(wxTextCtrl *ctrl, const wxString &line) {
	if (line.IsEmpty()) { ctrl->AppendText("\n"); return; }
	wxString val = ctrl->GetValue();
	if ( (not val.IsEmpty()) and (val.Last()!='\n'))
			ctrl->AppendText("\n");
	ctrl->AppendText(">>> "+line+"\n");
	ctrl->ShowPosition(ctrl->GetLastPosition());
}
void mxOutputView::LogInOutput(const wxString &line) {
	LogInOutput_aux(ctrl_std,line);
	LogInOutput_aux(ctrl_err,line);
}

void mxOutputView::Launched(wxProcess *_process, int _pid, const wxString &command) {
	process = _process;
	pid = _pid;
	if (pid) {
		LogInOutput("Running command: " +command);
		working=true;
		close_button->Enable(false);
		extra_button->Enable(true);
		m_extra_callback = [this](mxOutputView*){ if (working) process->Kill(pid,wxSIGKILL); };
		timer->Start(500,true);
		state->SetLabel(LANG(LAUNCH_STATUS_RUNNING,"Estado: Ejecutando"));
	} else {
		LogInOutput("ERROR: Could not run: " +command);
	}
}

void mxOutputView::OnProcessTerminate(wxProcessEvent &evt) {
	wxProcess *ptr = process;
	OnProcessTerminateCommon(evt.GetExitCode());
	delete ptr; // el nullptr lo asigna OnProcessTerminateCommon
}

void mxOutputView::OnProcessTerminateCommon(int exit_code) {
	working=false;
	timer->Stop();
	wxYield();
	GetProcessOutput();
	LogInOutput(wxString()<<"Process Terminated, exit code: "<<exit_code);
	process=nullptr; // el delete lo hace OnProcessTerminate
	extra_button->Enable(false);
	close_button->Enable(true);
	if (exit_code) {
		state->SetLabel(wxString(LANG(LAUNCH_STATUS_EXITCODE,"Estado: Codigo de salida: "))<<exit_code);
	} else {
		state->SetLabel(LANG(LAUNCH_STATUS_FINISH,"Estado: Terminado"));
		if (textfile) { textfile->Close(); delete textfile; textfile = nullptr; }
	}
	if (m_opt_callback) {
		LogInOutput("");
		auto cb = m_opt_callback; // usar una copia, porque el callback puede definir un callback nuevo si hace otro Launch
		cb(this,exit_code);
	}
	if (close_button->IsEnabled()) close_button->SetFocus();
}

void mxOutputView::OnTimer(wxTimerEvent &evt) {
	GetProcessOutput();
}

static wxString GetProcessOutput_aux(wxInputStream *stream, wxTextCtrl *ctrl) {
	wxTextInputStream input(*stream);
	static wxString line; line.Clear();
	for(wxChar c; c=input.GetChar(); ) // GetChar returns 0 when there's no more data
		if (c!='\r') line<<c;
	ctrl->AppendText(line);
	ctrl->ShowPosition(ctrl->GetLastPosition());
	return line;
}
void mxOutputView::GetProcessOutput() {
	if (!process) return;
	wxString line = GetProcessOutput_aux(process->GetInputStream(),ctrl_std);
	if (progress_bar && line.Contains("% done")) {
		long p;
		wxString aux1 = line.Mid(0,line.Find("% done"));
		if (line.Mid(0,line.Find("% done")).AfterLast(' ').ToLong(&p)) {
			progress_bar->SetValue(p);
			this->SetTitle(wxString(win_caption)<<" ("<<p<<"%)");
		}
	}
	line = GetProcessOutput_aux(process->GetErrorStream(),ctrl_err);
	if (textfile && not line.IsEmpty()) textfile->Write(line);
	if (working) timer->Start(500,true);
}

void mxOutputView::OnExtraButton(wxCommandEvent &evt) {
	m_extra_callback(this);
}

void mxOutputView::SetExtraButton (const wxString & label, mxOutputView::OnExtraButon_t callback) {
	m_extra_callback = callback;
	extra_button->SetThings(bitmaps->buttons.next,label);
	extra_button->Enable(true);
}

