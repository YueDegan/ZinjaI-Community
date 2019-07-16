#ifndef MXREALTIMEINSPECTIONEDITOR_H
#define MXREALTIMEINSPECTIONEDITOR_H
#include <wx/frame.h>
#include "Inspection.h"
#include "mxUtils.h"
#include "Cpp11.h"
#include "raii.h"
#include <wx/timer.h>

class wxTextCtrl;
class wxButton;
class wxFlexGridSizer;
class wxStaticText;

class mxRealTimeInspectionEditor : public wxFrame, public myDIEventHandler, public myDIGlobalEventHandler {
	BoolFlag mask_events;
	wxFlexGridSizer *sizer;
	struct AuxRTIE {
		DebuggerInspection *di;
		int level; bool broken;
		wxStaticText *label;
		wxTextCtrl *text;
		wxButton *button;
	};
	SingleList<AuxRTIE> inspections;
	wxTimer m_resize_timer;
public:
	mxRealTimeInspectionEditor(const wxString &inspection);
	~mxRealTimeInspectionEditor();
	
	void Add(int pos, int lev, DebuggerInspection *di);
	void Break(int num);
	void Resize(bool only_grow_h);
	
	// eventos myDIGlobalEventHandler
	void OnDebugStop() override { Destroy(); }
	// eventos myDIEventHandler
	void OnDIError(DebuggerInspection *di) override;
	void OnDIValueChanged(DebuggerInspection *di) override;
	void OnDIOutOfScope(DebuggerInspection *di) override;
	
	// eventos propios
	void OnUpdateValues(wxCommandEvent &evt);
	void OnButton(wxCommandEvent &evt);
	void OnText(wxCommandEvent &evt);
	void OnClose(wxCloseEvent &evt);
	void OnResizeTimer(wxTimerEvent &evt);
	DECLARE_EVENT_TABLE();
};

#endif

