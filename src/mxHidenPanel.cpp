#include <wx/dcclient.h>
#include <wx/settings.h>
#include <wx/aui/aui.h>
#include "mxHidenPanel.h"
#include "mxMainWindow.h"
#include "mxWelcomePanel.h"
#include "mxAUI.h"

BEGIN_EVENT_TABLE(mxHidenPanel,wxPanel)
	EVT_PAINT(mxHidenPanel::OnPaint)
	EVT_ENTER_WINDOW(mxHidenPanel::OnMotion)
	EVT_LEAVE_WINDOW(mxHidenPanel::OnMotion)
	EVT_RIGHT_DOWN(mxHidenPanel::OnClick)
	EVT_LEFT_DOWN(mxHidenPanel::OnClick)
	EVT_TIMER(wxID_ANY,mxHidenPanel::OnTimer)
	EVT_SIZE(mxHidenPanel::OnResize)
END_EVENT_TABLE()

int mxHidenPanel::ignore_autohide=0;
mxMainWindow *mxHidenPanel::main_window = nullptr;
	
mxHidenPanel::mxHidenPanel(wxWindow *parent, wxWindow *acontrol, hp_pos apos, int alayer, int aorder, wxString alabel):wxPanel(parent,wxID_ANY,wxDefaultPosition,wxSize(15,15)) {
	label=alabel; selected=false; control=acontrol; pos=apos; forced_show=mouse_in=false; showing=false;
	layer=alayer; order=aorder;
	if (pos!=HP_BOTTOM) { 
		unsigned int i=1; 
		while (i<label.Len()) { 
			label=label.Mid(0,i)+"\n"+label.Mid(i); 
			i+=2;
		}
	}
	timer = new wxTimer(GetEventHandler(),wxID_ANY);
}

void mxHidenPanel::OnPaint(wxPaintEvent &evt) {
	wxPaintDC dc(this);
	PrepareDC(dc);
	dc.SetBackground(wxSystemSettings::GetColour(selected?wxSYS_COLOUR_3DSHADOW:(mouse_in||showing?wxSYS_COLOUR_3DHIGHLIGHT:wxSYS_COLOUR_3DFACE)));
	dc.Clear();
	dc.SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
	int w,h; GetClientSize(&w,&h);
	dc.DrawLabel(label,wxRect(0,0,w,h),wxALIGN_CENTER);
}

void mxHidenPanel::ToggleDock() {
	if (selected) 
		ShowFloat(true);
	else 
		ShowDock();
}

void mxHidenPanel::ShowFloat(bool set_focus) {
	wxAuiPaneInfo &pane = main_window->m_aui->GetPane(control);
	pane.Float();
	pane.Show();
	pane.MinSize(wxSize(10,10));
	// With wx 3.x and gtk3, first update takes size from minsize, the second one respects floatingsize, so
	// we need two (this is the extra one) or the first time the panel appears it does with the wrong size
	// We also do it here, so if we are toggling a panel from fixed to floating, it considers the
	// space that it has just released, in the size queries that follows
	main_window->m_aui->Update(); 
	
	int px,py,pw,ph,ax,ay,aw,ah;
	if (g_welcome_panel && g_welcome_panel->IsVisible()) {
		g_welcome_panel->GetScreenPosition(&px,&py);
		g_welcome_panel->GetSize(&pw,&ph);
	} else {
		main_window->notebook_sources->GetScreenPosition(&px,&py);
		main_window->notebook_sources->GetSize(&pw,&ph);
	}
	GetScreenPosition(&ax,&ay);
	GetSize(&aw,&ah);
//	int used_left = px-ax;
//	int used_right = (ax+aw)-(px+pw);
//	int used_bottom = (ay+ah)-(py+ph);
	if (pos==HP_LEFT) {
		px=ax+aw;
		pw/=4;
//		if (used_bottom && used_bottom_left<px+pw) ph-=used_bottom;
	} else if (pos==HP_BOTTOM) {
		ph/=3;
		py=ay-ph;
//		if (used_left && used_left_bottom>px) { pw-=used_left; px+=used_left; }
//		if (used_right && used_right_bottom<px+pw) pw-=used_right;
	} else if (pos==HP_RIGHT) {
		pw/=control==(wxWindow*)main_window->inspection_ctrl?3:4;
		px=ax-pw;
//		if (used_bottom && used_bottom_right>px) ph-=used_bottom;
	}
	
	pane.FloatingPosition(px,py);
	pane.FloatingSize(pw,ph);
	main_window->m_aui->Update();
	timer->Start(200,false);
	selected=false; showing=true; Refresh();
	if (!set_focus) main_window->Raise(); // los paneles flotando le sacan el foco a la main window y no se soluciona con setfocus
}

void mxHidenPanel::ShowDock() {
	timer->Stop();
	wxAuiPaneInfo &pane = main_window->m_aui->GetPane(control);
	pane.Show();
	pane.Dock();
	pane.Layer(layer);
	pane.Position(order);
	if      (pos==HP_LEFT)   pane.Left();
	else if (pos==HP_BOTTOM) pane.Bottom();
	else if (pos==HP_RIGHT)  pane.Right();
	showing=true;
	main_window->m_aui->Update();
	selected=true; Refresh();
}

void mxHidenPanel::Hide() {
	timer->Stop();
	selected=false; mouse_in=false; forced_show=false;
	main_window->m_aui->GetPane(control).Hide();
	main_window->m_aui->Update();
	showing=false; selected=false; Refresh();
}

void mxHidenPanel::ToggleFull() {
	if (selected) return;
	wxAuiPaneInfo &pane = main_window->m_aui->GetPane(control);
	int px,py,pw,ph;
	main_window->notebook_sources->GetScreenPosition(&px,&py);
	main_window->notebook_sources->GetSize(&pw,&ph);
	int ow,oh; control->GetSize(&ow,&oh);
	if (showing&&ow>2*pw/3&&oh>2*ph/3) {
		Hide();
		return;
	}
	pane.Show();
	pane.Float();
	pane.FloatingSize(pw,ph);
	pane.FloatingPosition(px,py);
	showing=true;
	main_window->m_aui->Update();
}

void mxHidenPanel::ProcessClose() {
	showing=false; 
	mouse_in=control->GetScreenRect().Contains(wxGetMousePosition());
	if (selected) { selected=false; Refresh(); }
}

void mxHidenPanel::ForceShow(bool set_focus) {
	forced_show=true;
	if (selected) {
		if (set_focus) Raise();
		if (set_focus) SetFocus();
		if (set_focus) control->Raise();
		if (set_focus) control->SetFocus();
		return;
	}
	if  (!main_window->m_aui->GetPane(control).IsShown()) 
		ShowFloat(set_focus);
}

void mxHidenPanel::OnClick(wxMouseEvent &evt) {
	if (evt.GetButton()==wxMOUSE_BTN_LEFT) {
		ToggleDock();
	} else {
		if (selected) return;
		ToggleFull();
	}
}

void mxHidenPanel::OnMotion(wxMouseEvent &evt) {
	if (selected) return;
	mouse_in=evt.GetEventType()==wxEVT_ENTER_WINDOW;
	timer->Start(200,false); Refresh();
}

void mxHidenPanel::OnTimer(wxTimerEvent &evt) {
	if (selected || ignore_autohide) return;
	if (showing) {
		if (forced_show) 
			forced_show=!control->GetScreenRect().Contains(wxGetMousePosition());
		else if (!mouse_in && !control->GetScreenRect().Contains(wxGetMousePosition())) {
			wxMouseState ms = wxGetMouseState();
			if (ms.LeftIsDown() || ms.RightIsDown() || ms.MiddleIsDown() ) return;
			Hide();
		}
	} else {
		if (mouse_in && main_window->IsActive()) ShowFloat(false);
	}
}

void mxHidenPanel::ProcessParentResize() {
	if (!selected && showing) Hide();
}

void mxHidenPanel::OnResize (wxSizeEvent & evt) {
	evt.Skip();
	Refresh();
}

mxHidenPanel::~mxHidenPanel ( ) {
	timer->Stop();
}

