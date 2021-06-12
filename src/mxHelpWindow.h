#ifndef MXHELPWINDOW_H
#define MXHELPWINDOW_H
#include "mxGenericHelpWindow.h"

class wxDialog;

class mxHelpWindow : public mxGenericHelpWindow {
	
	static mxHelpWindow *instance;
	mxHelpWindow(wxString file="");
	HashStringTreeItem items;
	
	wxString current_dir;
	wxString FixURL(wxString url, bool set_dir, bool select_tree, bool keep_args);
	wxString GetRealFName(wxString url);
	bool LoadPage(wxString url);
	
//	wxString GetHelpFile(wxString file);
	
	void PopulateIndex(wxTreeItemId root, wxString dir);
	
public:
	static mxHelpWindow *ShowHelp(wxString page="", wxDialog *from_modal=nullptr);
	
//	void LoadPage(wxString file);
	
	void ShowIndex() override;
	void OnTree(wxTreeItemId item) override;
	void OnSearch(wxString value) override;
	bool OnLink(wxString href) override;
//	bool CurrentPageIsHome() override;
	
	void OnForum(wxCommandEvent &event);
	
//	/// @brief Wrapper for wxHtmlWindow::LoadPage that fix href path (it enters relative to current page, but current page's path is not current working directory)
//	void FixLoadPage(const wxString &href); 
	
private:
	bool ignore_tree_event;
public:
	/// @brief Selects an iden in the index tree skipping its selecion event callback
	void SelectTreeItem(const wxString &fname);
	
	static void DeleteInstance() { if (instance) instance->Destroy(); }
	
	~mxHelpWindow();
};

#endif
