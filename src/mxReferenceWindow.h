#ifndef MXREFERENCEWINDOW_H
#define MXREFERENCEWINDOW_H
#include "mxGenericHelpWindow.h"
#include <stack>
#include <map>
#include <vector>

class mxReferenceWindow : public mxGenericHelpWindow {
	
	std::vector< std::pair<wxString,wxString> > search_index;
	bool LoadSearchIndex();
	
	wxTreeItemId page_tree_item;
	bool PopulateInitialTree();
	
	std::vector< std::pair<wxTreeItemId,wxString> > items_general,items_page;
	
	static mxReferenceWindow *instance;
	mxReferenceWindow(wxString file="");
	wxString current_path;
	wxString current_page;
	
	wxString m_doc_version; ///< version of the cppreference content archive
	
	std::stack<wxString> history_prev;
	std::stack<wxString> history_next;
//	bool CurrentPageIsHome() override;
	bool CanPrev() override;
	bool CanNext() override;
	
public:
	static void ShowPage(wxString page="");
	static void ShowAndSearch(const wxString &keyword);
	void ScrollToAnchor(wxString anchor);
	wxString ProcessHTML(wxString fname, mxReferenceWindow *w=nullptr);
	
	void LoadHelp(wxString file, bool update_history=true);
	
	void OnPrev() override;
	void OnNext() override;
	void OnTree(wxTreeItemId item) override;
	void ShowIndex() override;
	void OnSearch(wxString value) override;
	void OnSearch(wxString value, bool update_history);
	bool OnLink(wxString href) override;
	
	
	~mxReferenceWindow();
};

#endif

