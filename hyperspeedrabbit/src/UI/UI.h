#pragma once

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "AfficheurSprite.h"
#include "AfficheurTexte.h"
#include "DispositifD3D11.h"
#include "Button.h"

class Scene;
class Button;

class UI
{
public:
	using spritePtr = std::unique_ptr<PM3D::CAfficheurSprite>;
	using textPtr = std::unique_ptr<PM3D::CAfficheurTexte>;
	using buttonPtr = std::unique_ptr<Button>;

	using spriteList = std::vector<spritePtr>;
	using textList = std::vector<textPtr>;
	using buttonList = std::vector<buttonPtr>;

	UI(PM3D::CDispositifD3D11* _pDispositif, Scene* _scene);
	UI(const UI&) = delete;
	UI& operator=(const UI&) = delete;

	~UI() = default;

	void init();

	void updateDisplayedItems(); 
	void update();
	void updateButtons();
	void updateTexts();
	void updateSprites();
	void updateLoadingScreen(const float percent);
	void updateTimer(long long time);
	void draw();

	bool leftClick() const;

	void addSprite(spriteList& _sprites, const std::string& _filename, const unsigned int _x, const unsigned int _y, const unsigned int _width, const unsigned int _height);
	void addText(spriteList& _sprites, textList& _texts, const std::wstring& _text, const unsigned int _x, const unsigned int _y, const unsigned int _width, const unsigned int _height, Gdiplus::Font *_font, const Gdiplus::Color &_color = Gdiplus::Color(255, 0, 0, 0), const Gdiplus::Color& _bgColor = Gdiplus::Color(255, 0, 0, 0), bool _activateBlender = false);

	void addButton(spriteList& _sprites, buttonList& _buttons, std::unique_ptr<Button> _button);

	void updateScreenSizes();

private:
	Scene* m_scene;

	PM3D::CDispositifD3D11* m_pDispositif;
	HWND m_handler;

	float m_screenWidth, m_screenHeight;
	float m_screenWidthRatio, m_screenHeightRatio;

	// Polices
	void initPolices();

	spriteList* m_currentSprites;
	buttonList* m_currentButtons;
	Button* m_buttonPressed = {};

	// Loading screen
	spriteList m_spritesLoadingScreen;
	buttonList m_buttonsLoadingScreen;
	void initLoadingScreen();

	// Timer screen
	spriteList m_spritesTimerScreen;
	textList m_textsTimerScreen;
	buttonList m_buttonsTimerScreen;
	void initTimerScreen();

	// Main menu
	spriteList m_spritesMainMenu;
	textList m_textsMainMenu;
	buttonList m_buttonsMainMenu;
	void initMainMenu();

	// Settings menu
	spriteList m_spritesSettingsMenu;
	textList m_textsSettingsMenu;
	buttonList m_buttonsSettingsMenu;
	void initSettingsMenu();

	// Help menu
	spriteList m_spritesHelpMenu;
	textList m_textsHelpMenu;
	buttonList m_buttonsHelpMenu;
	void initHelpMenu();

	// Credit menu
	spriteList m_spritesCreditMenu;
	textList m_textsCreditMenu;
	buttonList m_buttonsCreditMenu;
	void initCreditMenu();

	// In-game menu
	spriteList m_spritesInGameMenu;
	textList m_textsInGameMenu;
	buttonList m_buttonsInGameMenu;
	void initInGameMenu();

	PM3D::CAfficheurTexte* m_elapsedTimeIngame;
	PM3D::CAfficheurTexte* m_elapsedTimeEndGame;
	PM3D::CAfficheurTexte* m_shipSpeed;
	PM3D::CAfficheurTexte* m_shipX, *m_shipY, *m_shipZ; // Ship position
	PM3D::CAfficheurTexte* m_camX, *m_camY, *m_camZ; // Cam position
	PM3D::CAfficheurTexte* m_camrX, * m_camrY, * m_camrZ, *m_camrW; // Cam position
	PM3D::CAfficheurTexte* m_shiprX, * m_shiprY, * m_shiprZ, * m_shiprW;
	PM3D::CAfficheurTexte*m_volumeMusic;
	PM3D::CAfficheurTexte*m_volumeSound;

	PM3D::CAfficheurTexte* m_nbGoodCollectablesInGame;
	PM3D::CAfficheurTexte* m_nbBadCollectablesInGame;
	PM3D::CAfficheurTexte* m_nbGoodCollectablesEndGame;
	PM3D::CAfficheurTexte* m_nbBadCollectablesEndGame;
	PM3D::CAfficheurTexte* m_newRecord;
	PM3D::CAfficheurTexte* m_finalScore;

	PM3D::CAfficheurTexte* m_test;
	PM3D::CAfficheurTexte* m_timer;

	int m_currentSpeedBar;
	int m_currentProgressBar = 40;
	int m_nbIterationProgressBar = 600;
	int m_lengthProgressBar = 1000;
	int testX, testY;
	

	// In-game items
	spriteList m_spritesInGameItems;
	spriteList m_spritesSpeedBar;
	spriteList m_spritesProgressBar;
	spriteList m_spritesProgressBarShip;

	textList m_textsInGameItems;
	buttonList m_buttonsInGameItems;
	void initInGameItems();

	// End game menu
	spriteList m_spritesEndGameMenu;
	textList m_textsEndGameMenu;
	buttonList m_buttonsEndGameMenu;
	void initEndGameMenu();

	std::unique_ptr<Gdiplus::Font> m_policeH0;
	std::unique_ptr<Gdiplus::Font> m_policeH1;
	std::unique_ptr<Gdiplus::Font> m_policeH2;
	std::unique_ptr<Gdiplus::Font> m_policeH3;
	std::unique_ptr<Gdiplus::Font> m_policeButton1;
	std::unique_ptr<Gdiplus::Font> m_policeButton2;
	std::unique_ptr<Gdiplus::Font> m_policeText;
};
