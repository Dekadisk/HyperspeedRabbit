#pragma once

#include "moteur.h"
#include "dispositifD3D11.h"
#include "Chrono.h"

namespace PM3D
{
#define MAX_LOADSTRING 100

class CMoteurWindows final : public CMoteur<CMoteurWindows, CDispositifD3D11>
{
public:
	void SetWindowsAppInstance(HINSTANCE hInstance);
	void quit();

	void setCursorArrow() noexcept;
	void setCursorTargetUnavailable() noexcept;
	void setCursorTargetAvailable() noexcept;
private:
	ATOM MyRegisterClass(HINSTANCE hInstance);
	bool initAppInstance();
	void show();

	// Les fonctions spécifiques
	bool initSpecific() override;
	bool runSpecific() override;
	int64_t getTimeSpecific() const override;
	double getTimeIntervalsInSec(int64_t start, int64_t stop) const override;
	CDispositifD3D11* creationDispositifSpecific(const CDS_MODE cdsMode);
	void beginRenderSceneSpecific() override;
	void endRenderSceneSpecific() override;

	// Fonctions "Callback" -- Doivent être statiques
	static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

	HACCEL hAccelTable;						// handle Windows de la table des accélérateurs
	static HINSTANCE hAppInstance;			// handle Windows de l'instance actuelle de l'application
	HWND hMainWnd;							// handle Windows de la fenêtre principale
	TCHAR szWindowClass[MAX_LOADSTRING];	// le nom de la classe de fenêtre principale

	Chrono m_chrono;

	//Cursors
	static HCURSOR m_hCursorArrow;
	static HCURSOR m_hCursorTargetAvailable;
	static HCURSOR m_hCursorTargetUnavailable;
	static HCURSOR* m_hCurrentCursor;
};
} // namespace PM3D
