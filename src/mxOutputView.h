#ifndef MXOUTPUTVIEW_H
#define MXOUTPUTVIEW_H
#include <wx/dialog.h>
#include <wx/process.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include "mxValgrindOuput.h"

class wxTextCtrl;
class wxProcess;
class wxTimer;
class wxStaticText;
class mxBitmapButton;
class wxFile;
class wxGauge;

class mxOutputView : public wxFrame {
public:
	using OnProcTerminate_t = std::function<void(mxOutputView*,int)>;
	using OnExtraButon_t = std::function<void(mxOutputView*)>;
	mxOutputView(wxString caption, wxString output_file="", bool show_progress_bar = false);
	void Launch(wxString path, const wxString &command, OnProcTerminate_t opt_callback={});
	void Launched(wxProcess *_process, int _pid, const wxString &command);
	void GetProcessOutput();
	void OnOkButton(wxCommandEvent &evt);
	void OnExtraButton(wxCommandEvent &evt);
	void OnClose(wxCloseEvent &evt);
	void OnProcessTerminateCommon(int exit_code);
	void OnProcessTerminate(wxProcessEvent &evt);
	void OnTimer(wxTimerEvent &evt);
	void LogInOutput(const wxString &line);
	void SetExtraButton(const wxString &label, OnExtraButon_t callback);
	~mxOutputView();
private:
	wxTextCtrl *ctrl_std;
	wxTextCtrl *ctrl_err;
	wxStaticText *state;
	wxGauge *progress_bar;
	wxTimer *timer;
	wxString win_caption;
	mxBitmapButton *extra_button;
	mxBitmapButton *close_button;
	wxFile *textfile;
	wxProcess *process = nullptr;
	int pid = -1;
	bool working = false;
	OnProcTerminate_t m_opt_callback;
	OnExtraButon_t m_extra_callback;
	DECLARE_EVENT_TABLE();
};

#endif

