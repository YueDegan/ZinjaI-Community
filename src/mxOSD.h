#ifndef MXOSD_H
#define MXOSD_H

#include <functional>
#include <wx/dialog.h>
#include <wx/timer.h>
	
class wxFont;
class wxColour;
class wxBrush;
class wxButton;

/**
* @brief Clase para mostrar mensajes en pantalla tipo OSD
* 
* Esta clase se desarrolló inicialmente para mostrar mensajes de estado durante la
* depuración, pero no se utiliza.
**/
class mxOSD : public wxDialog {
private:
	wxButton *cancel_button;
	std::function<void()> on_cancel; ///< if this message has a cancel button, this is its callback
	wxTimer *timer;
	wxString text;
	wxWindow *parent;
	static wxFont *font;
	static wxColour *cb,*cf;
	static wxBrush *br;
	static mxOSD *current_osd;
	bool corner;
	mxOSD(wxWindow *aparent, wxString str="", int time=0, bool coner=false, std::function<void()> aon_cancel={});
	~mxOSD();
public:
	void ShowText(wxString str, int time=0, bool corner=false);
	void OnTimer(wxTimerEvent &evt);
	void OnPaint (wxPaintEvent &event);
	void OnCancel (wxCommandEvent &evt);
	void OnResize (wxSizeEvent &evt);
	static void HideCurrent() { if (current_osd) current_osd->Hide(); }
	
	static void Execute(wxString command, wxString message, std::function<void(int)> aon_end);
	
	static void MakeTimed(wxWindow *aparent, wxString str,int time) { 
		new mxOSD(aparent,str,time,true,nullptr); 
	}
	
	friend class mxOSDGuard;
	
	DECLARE_EVENT_TABLE();
};

class mxOSDGuard {
	mxOSD *m_osd;
	// no copiable
	mxOSDGuard(const mxOSDGuard&);
	mxOSDGuard &operator=(const mxOSDGuard&);
public:
	mxOSDGuard() : m_osd(nullptr) {}
	mxOSDGuard(wxWindow *aparent, wxString str) 
		: m_osd (new mxOSD(aparent,str,0,false,{})) {}
		void Create(wxWindow *aparent, wxString str) {
			if (m_osd) m_osd->Destroy();
			m_osd = new mxOSD(aparent,str,0,false,{});
		}
		void Hide() { if (m_osd) m_osd->Hide(); }
		void Dettach() { m_osd=nullptr; }
		~mxOSDGuard() { if (m_osd) m_osd->Destroy(); }
};

#endif

