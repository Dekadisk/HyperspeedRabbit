// PetitMoteur3D.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "HyperEngine.h"
#include "MoteurWindows.h"

using namespace PM3D;

int APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	// Pour ne pas avoir d'avertissement
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	try
	{
		// Création de l'objet Moteur
		CMoteurWindows& rMoteur = CMoteurWindows::get();

		// Spécifiques à une application Windows
		rMoteur.SetWindowsAppInstance(hInstance);

		// Initialisation du moteur
		if (!rMoteur.Initialisations())
			return -1;

		// Boucle d'application
		rMoteur.run();

		return 0;
	}

	catch (const std::exception& E)
	{
		const int BufferSize = 128;
		wchar_t message[BufferSize];

		size_t numCharacterConverted;
		mbstowcs_s(&numCharacterConverted, message, E.what(), BufferSize - 1);
		::MessageBox(nullptr, message, L"Erreur", MB_ICONWARNING);

		return (int)99;
	}

	catch (int codeErreur)
	{
		wchar_t szErrMsg[MAX_LOADSTRING];	// Un message d'erreur selon le code

		::LoadString(hInstance, codeErreur, szErrMsg, MAX_LOADSTRING);
		::MessageBox(nullptr, szErrMsg, L"Erreur", MB_ICONWARNING);

		return (int)99; // POURQUOI 99???
	}

}
