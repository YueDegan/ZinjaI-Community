#ifndef MXINFOWINDOW_H
#define MXINFOWINDOW_H
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "ComplementArchive.h"
#include <wx/gauge.h>
#include <wx/checkbox.h>
#include <wx/textfile.h>

class mxInfoWindow : public wxFrame {
private:
	complement_info info;
	wxCheckBox *show_details;
	wxString file,dest; int step; bool should_stop, details;
	wxTextCtrl *text;
	wxButton *but_ok;
	wxButton *but_cancel;
	wxGauge *gauge;
	wxTextFile *log_file = nullptr;
protected:
public:
	mxInfoWindow(wxString _dest, wxString _file);
	void OnButtonOk(wxCommandEvent &evt);
	void OnButtonCancel(wxCommandEvent &evt);
	void OnClose(wxCloseEvent &evt);
	void Notify(const wxString &message);
	void Progress(int progress);
	bool ShouldStop() { return should_stop; }
	void DontStop() { should_stop=false; }
	bool CheckRequirements();
	void Finish();
	DECLARE_EVENT_TABLE();
};

#endif

