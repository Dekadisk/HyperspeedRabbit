#include "stdafx.h"
#include "UI.h"
#include "Scene.h"
#include <algorithm>
#include <InputManager.h>

#include "SoundManager.h"
#include "ButtonResume.h"
#include "ButtonRestart.h"
#include "ButtonMainMenu.h"
#include "ButtonQuit.h"
#include "ButtonHelp.h"
#include "ButtonSettings.h"
#include "ButtonLowerMusic.h"
#include "ButtonHigherMusic.h"
#include "ButtonLowerSound.h"
#include "ButtonHigherSound.h"
#include "ButtonCredit.h"

constexpr int m_screenWidthAssumption = 1920;
constexpr int m_screenHeightAssumption = 1080;

constexpr int m_centerX = m_screenWidthAssumption / 2;
constexpr int m_centerY = m_screenHeightAssumption / 2;

constexpr int m_padding = 20;
constexpr int m_spacing = 28;

const std::wstring textureBgButton = L"assets/textures/buttons/ButtonFillSmall.png";
const std::wstring textureBgButtonHovered = L"assets/textures/buttons/ButtonFillDarkSmall.png";

UI::UI(PM3D::CDispositifD3D11* _pDispositif, Scene* _scene) : m_pDispositif{ _pDispositif }, m_scene{ _scene }, m_currentSprites{ nullptr }, m_shipSpeed{ nullptr }, m_shipX{ nullptr }, m_shipY{ nullptr }, m_shipZ{ nullptr }, m_currentSpeedBar{ 0 }, m_handler{ FindWindowA(nullptr, "PetitMoteur3D") }
{
	updateScreenSizes();
	initLoadingScreen();
}

void UI::initLoadingScreen()
{
	addSprite(m_spritesLoadingScreen, "assets/textures/background/space.dds", 0, 0, m_screenWidth, m_screenHeight);
	addSprite(m_spritesLoadingScreen, "assets/textures/loading/neon.dds", 0, 1060, m_screenWidth, m_screenHeight);
	m_currentSprites = &m_spritesLoadingScreen;
	m_currentButtons = &m_buttonsLoadingScreen;
}

void UI::initTimerScreen()
{
	constexpr int buttonWidth = 460;
	constexpr int buttonHeight = 60;
	
	constexpr int restartX = m_centerX - buttonWidth / 2;
	constexpr int restartY = 340 + buttonHeight + m_spacing * 4;

	const Gdiplus::Color buttonColor = Gdiplus::Color(255, 255, 255, 255);
	const Gdiplus::Color bgColor = Gdiplus::Color(190, 0, 0, 0);

	addText(m_spritesTimerScreen, m_textsTimerScreen, L"HELLO", 0, 0, m_screenWidth, m_screenHeight, m_policeH2.get(), Gdiplus::Color(255, 255, 255, 255), bgColor, true);
	m_timer = m_textsTimerScreen.back().get();

}

void UI::updateLoadingScreen(const float percent)
{
	if (percent <= 100.f && percent >= 0.f) {
		m_spritesLoadingScreen.erase(m_spritesLoadingScreen.end() - 1);
		addSprite(m_spritesLoadingScreen, "assets/textures/loading/neon.dds", 0, 1060, static_cast<int>(m_screenWidth * percent / 100.f), 1070);
	}
	m_currentSprites = &m_spritesLoadingScreen;
	m_currentButtons = &m_buttonsLoadingScreen;
}

void UI::updateTimer(long long time)
{
}

void UI::init()
{
	// Polices
	initPolices();

	// UI items
	initMainMenu();
	initTimerScreen();
	initSettingsMenu();
	initHelpMenu();
	initCreditMenu();
	initInGameMenu();
	initInGameItems();
	initEndGameMenu();

	updateDisplayedItems();
}

void UI::initPolices()
{
	const Gdiplus::FontFamily oFamily(L"Arial", nullptr);

	m_policeH0 = std::make_unique<Gdiplus::Font>(&oFamily, 100.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	m_policeH1 = std::make_unique<Gdiplus::Font>(&oFamily, 60.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	m_policeH2 = std::make_unique<Gdiplus::Font>(&oFamily, 46.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	m_policeH3 = std::make_unique<Gdiplus::Font>(&oFamily, 38.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

	m_policeButton1 = std::make_unique<Gdiplus::Font>(&oFamily, 30.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	m_policeButton2 = std::make_unique<Gdiplus::Font>(&oFamily, 28.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

	m_policeText = std::make_unique<Gdiplus::Font>(&oFamily, 22.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
}

void UI::initMainMenu()
{
	// Button
	constexpr int buttonWidth = 223;
	constexpr int buttonHeight = 65;

	constexpr int buttonStartX = m_centerX - buttonWidth / 2;
	constexpr int buttonStartY = 600;

	constexpr int buttonSettingsX = buttonStartX;
	constexpr int buttonSettingsY = buttonStartY + buttonHeight + m_spacing;

	constexpr int buttonHelpX = buttonStartX;
	constexpr int buttonHelpY = buttonSettingsY + buttonHeight + m_spacing;

	constexpr int buttonCreditX = buttonStartX;
	constexpr int buttonCreditY = buttonHelpY + buttonHeight + m_spacing;

	constexpr int buttonQuitX = buttonStartX;
	constexpr int buttonQuitY = buttonCreditY + buttonHeight + m_spacing;

	const Gdiplus::Color color = Gdiplus::Color(255, 255, 255, 255);
	const Gdiplus::Color bgColor = Gdiplus::Color(255, 100, 100, 100);

	addSprite(m_spritesMainMenu, "assets/textures/background/bgMenu.dds", 0, 0, m_screenWidth, m_screenHeight);

	auto buttonStart = std::make_unique<ButtonRestart>(m_pDispositif, L"Start", buttonStartX, buttonStartY, buttonWidth, buttonHeight, m_policeButton1.get(), color, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");
	auto buttonSettings = std::make_unique<ButtonSettings>(m_pDispositif, L"Settings", buttonSettingsX, buttonSettingsY, buttonWidth, buttonHeight, m_policeButton1.get(), color, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");
	auto buttonHelp = std::make_unique<ButtonHelp>(m_pDispositif, L"Help", buttonHelpX, buttonHelpY, buttonWidth, buttonHeight, m_policeButton1.get(), color, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");
	auto buttonCredit = std::make_unique<ButtonCredit>(m_pDispositif, L"Credits", buttonCreditX, buttonCreditY, buttonWidth, buttonHeight, m_policeButton1.get(), color, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");
	auto buttonQuit = std::make_unique<ButtonQuit>(m_pDispositif, L"Quit", buttonQuitX, buttonQuitY, buttonWidth, buttonHeight, m_policeButton1.get(), color, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");

	addButton(m_spritesMainMenu, m_buttonsMainMenu, std::move(buttonStart));
	addButton(m_spritesMainMenu, m_buttonsMainMenu, std::move(buttonSettings));
	addButton(m_spritesMainMenu, m_buttonsMainMenu, std::move(buttonHelp));
	addButton(m_spritesMainMenu, m_buttonsMainMenu, std::move(buttonCredit));
	addButton(m_spritesMainMenu, m_buttonsMainMenu, std::move(buttonQuit));
}

void UI::initSettingsMenu()
{
	constexpr int buttonWidth = 223;
	constexpr int buttonHeight = 65;
	constexpr int buttonSize = 50;

	///////// Music

	// Text
	constexpr int volumeMusicTextWidth = 200;
	constexpr int volumeMusicTextHeight = 50;

	constexpr int volumeMusicTextX = (1920 - volumeMusicTextWidth) / 2 - 10;
	constexpr int volumeMusicTextY = 490;

	// Button
	constexpr int buttonLowerMusicX = volumeMusicTextX - m_spacing * 2.5;
	constexpr int buttonLowerMusicY = volumeMusicTextY;
	constexpr int buttonHigherMusicX = volumeMusicTextX + volumeMusicTextWidth + m_spacing;
	constexpr int buttonHigherMusicY = buttonLowerMusicY;

	///////// Sound
	// Text
	constexpr int volumeSoundTextWidth = volumeMusicTextWidth;
	constexpr int volumeSoundTextHeight = volumeMusicTextHeight;

	constexpr int volumeSoundTextX = volumeMusicTextX;
	constexpr int volumeSoundTextY = 700;

	// Button
	constexpr int buttonLowerSoundX = volumeSoundTextX - m_spacing * 2.5;
	constexpr int buttonLowerSoundY = volumeSoundTextY;
	constexpr int buttonHigherSoundX = volumeSoundTextX + volumeSoundTextWidth + m_spacing;
	constexpr int buttonHigherSoundY = volumeSoundTextY;

	///////// Main menu
	constexpr int buttonMainMenuX = (1920 - buttonWidth) / 2;
	constexpr int buttonMainMenuY = 1080 - buttonHeight - m_padding;

	// Colors
	const Gdiplus::Color color = Gdiplus::Color(255, 0, 0, 0);
	const Gdiplus::Color bgColor = Gdiplus::Color(255, 140, 140, 140);

	const Gdiplus::Color buttonColor = Gdiplus::Color(255, 255, 255, 255);
	const Gdiplus::Color buttonBgColor = Gdiplus::Color(255, 238, 130, 238);


	//////////////
	addSprite(m_spritesSettingsMenu, "assets/textures/background/bgSettings.dds", 0, 0, 1920, 1080);

	////////// Music
	addText(m_spritesSettingsMenu, m_textsSettingsMenu, L"Medium", volumeMusicTextX, volumeMusicTextY, volumeMusicTextWidth, volumeMusicTextHeight, m_policeH3.get(), Gdiplus::Color(255, 255, 255, 255), buttonBgColor, true);
	m_volumeMusic = m_textsSettingsMenu.back().get();

	auto buttonLowerMusicVolume = std::make_unique<ButtonLowerMusic>(m_pDispositif, L"-", buttonLowerMusicX, buttonLowerMusicY, buttonSize, buttonSize, m_policeH2.get(), color, L"assets/textures/buttons/button1.png", L"assets/textures/buttons/button2.png");
	auto buttonHigherMusicVolume = std::make_unique<ButtonHigherMusic>(m_pDispositif, L"+", buttonHigherMusicX, buttonHigherMusicY, buttonSize, buttonSize, m_policeH2.get(), color, L"assets/textures/buttons/button1.png", L"assets/textures/buttons/button2.png");
	
	addButton(m_spritesSettingsMenu, m_buttonsSettingsMenu, std::move(buttonLowerMusicVolume));
	addButton(m_spritesSettingsMenu, m_buttonsSettingsMenu, std::move(buttonHigherMusicVolume));

	////////// Sound
	addText(m_spritesSettingsMenu, m_textsSettingsMenu, L"Medium", volumeSoundTextX, volumeSoundTextY, volumeSoundTextWidth, volumeSoundTextHeight, m_policeH3.get(), Gdiplus::Color(255, 255, 255, 255), buttonBgColor, true);
	m_volumeSound = m_textsSettingsMenu.back().get();

	auto buttonLowerSoundVolume = std::make_unique<ButtonLowerSound>(m_pDispositif, L"-", buttonLowerSoundX, buttonLowerSoundY, buttonSize, buttonSize, m_policeH2.get(), color, L"assets/textures/buttons/button1.png", L"assets/textures/buttons/button2.png");
	auto buttonHigherSoundVolume = std::make_unique<ButtonHigherSound>(m_pDispositif, L"+", buttonHigherSoundX, buttonHigherSoundY, buttonSize, buttonSize, m_policeH2.get(), color, L"assets/textures/buttons/button1.png", L"assets/textures/buttons/button2.png");

	addButton(m_spritesSettingsMenu, m_buttonsSettingsMenu, std::move(buttonLowerSoundVolume));
	addButton(m_spritesSettingsMenu, m_buttonsSettingsMenu, std::move(buttonHigherSoundVolume));


	auto buttonMainMenu = std::make_unique<ButtonMainMenu>(m_pDispositif, L"Main Menu", buttonMainMenuX, buttonMainMenuY, buttonWidth, buttonHeight, m_policeButton1.get(), buttonColor, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");

	addButton(m_spritesSettingsMenu, m_buttonsSettingsMenu, std::move(buttonMainMenu));
}

void UI::initHelpMenu()
{
	constexpr int buttonWidth = 223;
	constexpr int buttonHeight = 65;
	constexpr int buttonMainMenuX = (m_screenWidthAssumption - buttonWidth) / 2;
	constexpr int buttonMainMenuY = m_screenHeightAssumption - buttonHeight - m_padding;

	const Gdiplus::Color color = Gdiplus::Color(255, 0, 0, 0);
	const Gdiplus::Color bgColor = Gdiplus::Color(255, 140, 140, 140);

	const Gdiplus::Color buttonColor = Gdiplus::Color(255, 255, 255, 255);

	addSprite(m_spritesHelpMenu, "assets/textures/background/bgHelp.dds", 0, 0, m_screenWidth, m_screenHeight);

	auto buttonMainMenu = std::make_unique<ButtonMainMenu>(m_pDispositif, L"Main Menu", buttonMainMenuX, buttonMainMenuY, buttonWidth, buttonHeight, m_policeButton1.get(), buttonColor, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");
	addButton(m_spritesHelpMenu, m_buttonsHelpMenu, std::move(buttonMainMenu));
}

void UI::initCreditMenu()
{
	constexpr int buttonWidth = 223;
	constexpr int buttonHeight = 65;
	constexpr int buttonMainMenuX = (m_screenWidthAssumption - buttonWidth) / 2;
	constexpr int buttonMainMenuY = m_screenHeightAssumption - buttonHeight - m_padding;

	const Gdiplus::Color color = Gdiplus::Color(255, 0, 0, 0);
	const Gdiplus::Color bgColor = Gdiplus::Color(255, 140, 140, 140);

	const Gdiplus::Color buttonColor = Gdiplus::Color(255, 255, 255, 255);

	addSprite(m_spritesCreditMenu, "assets/textures/background/bgCredit.dds", 0, 0, m_screenWidth, m_screenHeight);

	auto buttonMainMenu = std::make_unique<ButtonMainMenu>(m_pDispositif, L"Main Menu", buttonMainMenuX, buttonMainMenuY, buttonWidth, buttonHeight, m_policeButton1.get(), buttonColor, L"assets/textures/buttons/ButtonFillSmall.png", L"assets/textures/buttons/ButtonFillDarkSmall.png");
	addButton(m_spritesCreditMenu, m_buttonsCreditMenu, std::move(buttonMainMenu));
}

void UI::initInGameMenu()
{
	constexpr int buttonWidth = 460;
	constexpr int buttonHeight = 60;

	constexpr int resumeX = m_centerX - buttonWidth / 2;
	constexpr int resumeY = 340;

	constexpr int restartX = m_centerX - buttonWidth / 2;
	constexpr int restartY = resumeY + buttonHeight + m_spacing * 4;

	constexpr int mainMenuX = m_centerX - buttonWidth / 2;
	constexpr int mainMenuY = restartY + buttonHeight + m_spacing * 4;

	const Gdiplus::Color buttonColor = Gdiplus::Color(255, 255, 255, 255);
	const Gdiplus::Color bgColor = Gdiplus::Color(190, 0, 0, 0);

	addText(m_spritesInGameMenu, m_textsInGameMenu, L": ", 0, 0, m_screenWidth, m_screenHeight, m_policeH2.get(), Gdiplus::Color(0, 0, 0, 0), bgColor, true);

	auto buttonResume = std::make_unique<ButtonResume>(m_pDispositif, L"Resume", resumeX, resumeY, buttonWidth, buttonHeight, m_policeH1.get(), buttonColor, bgColor);
	auto buttonRestart = std::make_unique<ButtonRestart>(m_pDispositif, L"Restart", restartX, restartY, buttonWidth, buttonHeight, m_policeH1.get(), buttonColor, bgColor);
	auto buttonMainMenu = std::make_unique<ButtonMainMenu>(m_pDispositif, L"Main Menu", mainMenuX, mainMenuY, buttonWidth, buttonHeight, m_policeH1.get(), buttonColor, bgColor);

	addButton(m_spritesInGameMenu, m_buttonsInGameMenu, std::move(buttonResume));
	addButton(m_spritesInGameMenu, m_buttonsInGameMenu, std::move(buttonRestart));
	addButton(m_spritesInGameMenu, m_buttonsInGameMenu, std::move(buttonMainMenu));
}

void UI::initInGameItems()
{
	constexpr int width = 100;
	constexpr int height = 48;

	constexpr int spriteSize = 48;

	const Gdiplus::Color buttonColor = Gdiplus::Color(255, 238,130,238);
	const Gdiplus::Color buttonBgColor = Gdiplus::Color(0, 0, 0, 0);

	constexpr int spriteChronoX = 1720;
	constexpr int spriteChronoY = 16;

	constexpr int spriteGoodCollectableX = spriteChronoX;
	constexpr int spriteGoodCollectableY = spriteChronoY + spriteSize + m_spacing;

	constexpr int spriteBadCollectableX = spriteGoodCollectableX;
	constexpr int spriteBadCollectableY = spriteGoodCollectableY + spriteSize + m_spacing;

	constexpr int speedBarWidth = 69;
	constexpr int speedBarHeight = 328;

	constexpr int speedBarX = 1830;
	constexpr int speedBarY = 730;

	///// Time
	addSprite(m_spritesInGameItems, "assets/textures/collectables/chronotimer_trans.dds", spriteChronoX, spriteChronoY, spriteSize, spriteSize);
	addText(m_spritesInGameItems, m_textsInGameItems, L"1:00.00", spriteChronoX + spriteSize + m_spacing, spriteChronoY, width, height, m_policeButton1.get(), buttonColor, buttonBgColor, true);
	m_elapsedTimeIngame = m_textsInGameItems.back().get();

	///// Collectables
	addSprite(m_spritesInGameItems, "assets/textures/collectables/spritepatoune_trans.dds", spriteGoodCollectableX, spriteGoodCollectableY, spriteSize, spriteSize);
	addText(m_spritesInGameItems, m_textsInGameItems, L"0", spriteGoodCollectableX + spriteSize + m_spacing, spriteGoodCollectableY, width, height, m_policeButton1.get(), buttonColor, buttonBgColor, true);
	m_nbGoodCollectablesInGame = m_textsInGameItems.back().get();

	addSprite(m_spritesInGameItems, "assets/textures/collectables/spritepatounered_trans.dds", spriteBadCollectableX, spriteBadCollectableY, spriteSize, spriteSize);
	addText(m_spritesInGameItems, m_textsInGameItems, L"0", spriteBadCollectableX + spriteSize + m_spacing, spriteBadCollectableY, width, height, m_policeButton1.get(), buttonColor, buttonBgColor, true);
	m_nbBadCollectablesInGame = m_textsInGameItems.back().get();

	////// Ship speed
	addText(m_spritesInGameItems, m_textsInGameItems, L"", speedBarX - width / 2 + 33, speedBarY - height - m_spacing / 3, width, height, m_policeH3.get(), buttonColor, buttonBgColor, true);
	m_shipSpeed = m_textsInGameItems.back().get();
	
	////// SpeedBar
	for(int i=0;i<25;++i){
		std::string speedbarPath = "assets/textures/speedbar/sb";
		speedbarPath += std::to_string(i);
		speedbarPath += ".dds";
		addSprite(m_spritesSpeedBar, speedbarPath, speedBarX, speedBarY, speedBarWidth, speedBarHeight);
	}

	////// ProgressBar
	DirectX::XMFLOAT2 pos2D = { 10,1020 };
	addSprite(m_spritesInGameItems, "assets/textures/progressbar/pb0-0.dds", pos2D.x, pos2D.y, 36, 60);
	for (int i = 0; i < m_nbIterationProgressBar; ++i) {
		addSprite(m_spritesProgressBar, "assets/textures/progressbar/pb35-0.dds", pos2D.x + 34, pos2D.y, i*m_lengthProgressBar/ m_nbIterationProgressBar, 60);
		addSprite(m_spritesProgressBarShip, "assets/textures/progressbar/pbX-0.dds", pos2D.x + 34+ i * m_lengthProgressBar / m_nbIterationProgressBar, pos2D.y, 88, 60);
	}
	addSprite(m_spritesInGameItems, "assets/textures/progressbar/pbX89-0.dds", pos2D.x+34+m_lengthProgressBar+88, pos2D.y, 36, 60);


	// Debug
	/////////////////////////////////
/*#ifdef _DEBUG

	//Ship rotation
	addText(m_spritesInGameItems, m_textsInGameItems, L"Ship rot:", 1450, 300, 100, height, m_policeText.get(), buttonColor, buttonBgColor);

	addText(m_spritesInGameItems, m_textsInGameItems, L"Ship rX", 1580, 300, 90, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_shiprX = m_textsInGameItems.back().get();

	addText(m_spritesInGameItems, m_textsInGameItems, L"Ship rY", 1650, 300, 90, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_shiprY = m_textsInGameItems.back().get();

	addText(m_spritesInGameItems, m_textsInGameItems, L"Ship rZ", 1720, 300, 90, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_shiprZ = m_textsInGameItems.back().get();

	addText(m_spritesInGameItems, m_textsInGameItems, L"Ship rW", 1790, 300, 90, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_shiprW = m_textsInGameItems.back().get();

	// Ship position
	addText(m_spritesInGameItems, m_textsInGameItems, L"Ship:", 1590, 150, 60, height, m_policeText.get(), buttonColor, buttonBgColor);

	addText(m_spritesInGameItems, m_textsInGameItems, L"X", 1350, 150, 70, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_shipX = m_textsInGameItems.back().get();

	addText(m_spritesInGameItems, m_textsInGameItems, L"Y", 1420, 150, 70, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_shipY = m_textsInGameItems.back().get();

	addText(m_spritesInGameItems, m_textsInGameItems, L"Z", 1490, 150, 70, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_shipZ = m_textsInGameItems.back().get();

	// Cam position
	addText(m_spritesInGameItems, m_textsInGameItems, L"Cam:", 1590, 210, 60, height, m_policeText.get(), buttonColor, buttonBgColor);

	addText(m_spritesInGameItems, m_textsInGameItems, L"X", 1350, 210, 70, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_camX = m_textsInGameItems.back().get();

	addText(m_spritesInGameItems, m_textsInGameItems, L"Y", 1420, 210, 70, height, m_policeText.get(), buttonColor, buttonBgColor);
	m_camY = m_textsInGameItems.back().get();

	addText(m_spritesInGameItems, m_textsInGameItems, L"Z", 1490, 210, 70, height, m_policeText.get(), buttonColor, buttonBgColor);

	m_camZ = m_textsInGameItems.back().get();
#endif // DEBUG
*/
}

void UI::initEndGameMenu()
{
	constexpr int buttonWidth = 460;
	constexpr int buttonHeight = 60;

	constexpr int timeTextWidth = 200;
	constexpr int timeTextHeight = 76;
	constexpr int newRecordTextHeight = 50;

	constexpr int collectablesTextWidth = 200;
	constexpr int collectablesTextHeight = timeTextHeight;

	// Scores
	constexpr int timeX = 580;
	constexpr int timeY = 120;

	constexpr int goodCollectablesX = timeX + timeTextWidth + 3 * m_spacing;
	constexpr int goodCollectablesY = timeY;

	constexpr int badCollectablesX = goodCollectablesX + collectablesTextWidth + 3 * m_spacing;
	constexpr int badCollectablesY = timeY;

	// Final score
	constexpr int finalScoreWidth = 800;
	constexpr int finalScoreHeight = 100;
	constexpr int finalScoreX = m_centerX - finalScoreWidth / 2;
	constexpr int finalScoreY = timeY + 7 *m_spacing;

	constexpr int newRecordX = m_centerX - buttonWidth / 2;
	constexpr int newRecordY = finalScoreY - buttonHeight - 10;

	// Restart 
	constexpr int restartX = m_centerX - buttonWidth / 2;
	constexpr int restartY = 650;

	// Main menu
	constexpr int mainMenuX = m_centerX - buttonWidth / 2;
	constexpr int mainMenuY = restartY + buttonHeight + m_spacing * 4;

	const Gdiplus::Color buttonColorNormal = Gdiplus::Color(255, 255, 255, 255);
	const Gdiplus::Color buttonColorRed = Gdiplus::Color(255, 255, 0, 0);
	const Gdiplus::Color buttonColorGrey = Gdiplus::Color(255, 160, 160, 160);
	const Gdiplus::Color bgColor = Gdiplus::Color(190, 0, 0, 0);

	// Background
	addText(m_spritesEndGameMenu, m_textsEndGameMenu, L"", 0, 0, m_screenWidth, m_screenHeight, m_policeH2.get(), Gdiplus::Color(0, 0, 0, 0), bgColor, true);

	auto buttonRestart = std::make_unique<ButtonRestart>(m_pDispositif, L"Restart", restartX, restartY, buttonWidth, buttonHeight, m_policeH1.get(), buttonColorNormal, bgColor);
	auto buttonMainMenu = std::make_unique<ButtonMainMenu>(m_pDispositif, L"Main Menu", mainMenuX, mainMenuY, buttonWidth, buttonHeight, m_policeH1.get(), buttonColorNormal, bgColor);

	addButton(m_spritesEndGameMenu, m_buttonsEndGameMenu, std::move(buttonRestart));
	addButton(m_spritesEndGameMenu, m_buttonsEndGameMenu, std::move(buttonMainMenu));

	// Time
	addText(m_spritesEndGameMenu, m_textsEndGameMenu, L"0'00'00", timeX, timeY, timeTextWidth, timeTextHeight, m_policeH1.get(), buttonColorGrey, bgColor, true);
	m_elapsedTimeEndGame = m_textsEndGameMenu.back().get();

	// Collectables
	addText(m_spritesEndGameMenu, m_textsEndGameMenu, L"0", goodCollectablesX, badCollectablesY, collectablesTextWidth, collectablesTextHeight, m_policeH1.get(), buttonColorGrey, bgColor, true);
	m_nbGoodCollectablesEndGame = m_textsEndGameMenu.back().get();

	addText(m_spritesEndGameMenu, m_textsEndGameMenu, L"0", badCollectablesX, timeY, collectablesTextWidth, collectablesTextHeight, m_policeH1.get(), buttonColorGrey, bgColor, true);
	m_nbBadCollectablesEndGame = m_textsEndGameMenu.back().get();

	// Text new record
	addText(m_spritesEndGameMenu, m_textsEndGameMenu, L"New record", newRecordX, newRecordY, buttonWidth, newRecordTextHeight, m_policeButton2.get(), buttonColorRed, bgColor, true);
	m_newRecord = m_textsEndGameMenu.back().get();

	// Final score
	addText(m_spritesEndGameMenu, m_textsEndGameMenu, L"4000 points", finalScoreX, finalScoreY, finalScoreWidth, finalScoreHeight, m_policeH0.get(), buttonColorNormal, bgColor, true);
	m_finalScore = m_textsEndGameMenu.back().get();

	addSprite(m_spritesEndGameMenu, "assets/textures/collectables/chronotimer_withBlackBg.dds", timeX - collectablesTextWidth / 2 + 54,  badCollectablesY + 10, 50, 50);
	addSprite(m_spritesEndGameMenu, "assets/textures/collectables/spritepatounered_withBlackBg.dds", badCollectablesX - collectablesTextWidth / 2 + 100, badCollectablesY + 10, 50, 50);
	addSprite(m_spritesEndGameMenu, "assets/textures/collectables/spritepatoune_withBlackBg.dds", goodCollectablesX - collectablesTextWidth / 2 + 100, badCollectablesY + 10, 50, 50);
}

void UI::updateDisplayedItems()
{
	if (m_scene->isLoading())
	{
		m_currentSprites = &m_spritesLoadingScreen;
		m_currentButtons = &m_buttonsLoadingScreen;
		return;
	}

	if (m_scene->isEndGame())
	{
		m_currentSprites = &m_spritesEndGameMenu;
		m_currentButtons = &m_buttonsEndGameMenu;
	}
	else if (m_scene->isInGame())
	{
		if (m_scene->isPaused())
		{
			m_currentSprites = &m_spritesInGameMenu;
			m_currentButtons = &m_buttonsInGameMenu;
		}
		else if (m_scene->isTimer())
		{
			m_currentSprites = &m_spritesTimerScreen;
			m_currentButtons = &m_buttonsTimerScreen;
		}
		else
		{
			m_currentSprites = &m_spritesInGameItems;
			m_currentButtons = &m_buttonsInGameItems;
		}

	}
	else if (m_scene->isSettingMode())
	{
		m_currentSprites = &m_spritesSettingsMenu;
		m_currentButtons = &m_buttonsSettingsMenu;
	}

	else if (m_scene->isHelpMode())
	{
		m_currentSprites = &m_spritesHelpMenu;
		m_currentButtons = &m_buttonsHelpMenu;
	}

	else if (m_scene->isCreditMode())
	{
		m_currentSprites = &m_spritesCreditMenu;
		m_currentButtons = &m_buttonsCreditMenu;
	}

	else
	{
		m_currentSprites = &m_spritesMainMenu;
		m_currentButtons = &m_buttonsMainMenu;
	}
}

void UI::update()
{
	updateButtons();
	updateTexts();
	updateSprites();
}

bool UI::leftClick() const
{
	return PM3D::InputManager::get().getMouseState().rgbButtons[0] & 0x80;
}

void UI::updateButtons()
{
	DirectX::XMFLOAT2 posMouse = PM3D::InputManager::get().getMousePosition(); // Screen-coordinates

	std::for_each(m_currentButtons->begin(), m_currentButtons->end(), [&](buttonPtr& button) noexcept {
		button->setHovered(false);
		(*button).Ecrire((*button).getCurrentText());
	});

	// Button hovered - test
	auto hoveredButton = std::find_if(m_currentButtons->rbegin(), m_currentButtons->rend(), [&posMouse](const buttonPtr& button) noexcept {
		return button->containsMouse(button->localPosX(static_cast<int>(posMouse.x)), button->localPosY(static_cast<int>(posMouse.y)));
	});

	if (hoveredButton == m_currentButtons->rend())
		return;

	(*hoveredButton)->setHovered(true);
	(*hoveredButton)->Ecrire((*hoveredButton)->getCurrentText());

	if (leftClick())
	{
		m_buttonPressed = (*hoveredButton).get();
		testX = static_cast<int>(posMouse.x);
		testY = static_cast<int>(posMouse.y);
	}
	else
	{
		if (m_buttonPressed) // if true a button was pressed
		{
			m_buttonPressed->clicked(m_scene);
			m_buttonPressed = nullptr;
		}
	}
}

void UI::updateTexts()
{
	///// Time
	long long minutes = m_scene->getElapsedTime() / 60'000; // 60'000 because ellapsedTime is in ms
	long long seconds = (m_scene->getElapsedTime() - minutes * 60'000) / 1'000;
	long long ms = (m_scene->getElapsedTime() - minutes * 60'000 - seconds * 1'000);

	std::wstring time = (minutes > 0) ? std::to_wstring(minutes) + L":" : L"";
	time += std::to_wstring(std::move(seconds)) + L"." + std::to_wstring(std::move(ms / 10));

	m_elapsedTimeIngame->Ecrire(time);
	m_elapsedTimeEndGame->Ecrire(std::move(time));

	///// Timer
	std::wstring timer = (m_scene->getTimer() == 0) ? L"A VOUS" : std::to_wstring(m_scene->getTimer());
	m_timer->Ecrire(timer);

	///// Ship speed
	m_shipSpeed->Ecrire(std::to_wstring(static_cast<int>(m_scene->getShipSpeed())));

	///// Scores
	m_newRecord->Ecrire(m_scene->isNewRecord() ? L"New record" : L"");

	m_nbGoodCollectablesEndGame->Ecrire(std::to_wstring(m_scene->getNbGoodCollectables()));
	m_nbBadCollectablesEndGame->Ecrire(std::to_wstring(m_scene->getNbBadCollectables()));

	m_nbGoodCollectablesInGame->Ecrire(std::to_wstring(m_scene->getNbGoodCollectables()));
	m_nbBadCollectablesInGame->Ecrire(std::to_wstring(m_scene->getNbBadCollectables()));

	m_finalScore->Ecrire(L"Score: " + std::to_wstring(static_cast<int>(m_scene->getResultShipScore())));
	
	///// Settings
	m_volumeMusic->Ecrire(m_scene->getVolumeMusicString());
	m_volumeSound->Ecrire(m_scene->getVolumeSoundString());

	// Debug
	/////////////////////////////////////////////
/*#ifdef _DEBUG
	// Ship position
	m_shipX->Ecrire(std::to_wstring(static_cast<int>(m_scene->getShipPosition().x)));
	m_shipY->Ecrire(std::to_wstring(static_cast<int>(m_scene->getShipPosition().y)));
	m_shipZ->Ecrire(std::to_wstring(static_cast<int>(m_scene->getShipPosition().z)));
	

	// Cam rotation
	m_shiprX->Ecrire(std::to_wstring(m_scene->getShipRotation().x));
	m_shiprY->Ecrire(std::to_wstring(m_scene->getShipRotation().y));
	m_shiprZ->Ecrire(std::to_wstring(m_scene->getShipRotation().z));
	m_shiprW->Ecrire(std::to_wstring(m_scene->getShipRotation().w));
	
	// Cam position
	m_camX->Ecrire(std::to_wstring(static_cast<int>(m_scene->getCamPositionX())));
	m_camY->Ecrire(std::to_wstring(static_cast<int>(m_scene->getCamPositionY())));
	m_camZ->Ecrire(std::to_wstring(static_cast<int>(m_scene->getCamPositionZ())));
#endif // DEBUG*/
}

void UI::updateSprites()
{
	m_currentSpeedBar = min(24,max(0,static_cast<int>(24 * (m_scene->getShipCoefSpeed() - m_scene->getShipCoefMinSpeed()) / (m_scene->getShipCoefMaxSpeed() - m_scene->getShipCoefMinSpeed()))));
	m_currentProgressBar = min(m_nbIterationProgressBar, max(0, m_nbIterationProgressBar * m_scene->getProgression()));
}

void UI::draw()
{
	for (auto sprite = m_currentSprites->rbegin(); sprite != m_currentSprites->rend(); ++sprite)
		(*sprite)->Draw();
	if(m_spritesSpeedBar.size()>0)
	m_spritesSpeedBar[m_currentSpeedBar]->Draw();
	if (m_spritesProgressBar.size() > 0)
	m_spritesProgressBar[m_currentProgressBar]->Draw();
	if (m_spritesProgressBarShip.size() > 0)
		m_spritesProgressBarShip[m_currentProgressBar]->Draw();
}

void UI::addSprite(spriteList& _sprites, const std::string& _filename, const unsigned int _x, const unsigned int _y, const unsigned int _width, const unsigned int _height)
{
	spritePtr afficheur = std::make_unique<PM3D::CAfficheurSprite>(m_pDispositif);
	afficheur->AjouterSprite(_filename, _x * m_screenWidthRatio, _y * m_screenHeightRatio, _width * m_screenWidthRatio, _height * m_screenHeightRatio);
	_sprites.emplace_back(std::move(afficheur));
}

void UI::addText(spriteList& _sprites, textList& _texts, const std::wstring& _text, const unsigned int _x, const unsigned int _y, const unsigned int _width, const unsigned int _height, Gdiplus::Font* _font, const Gdiplus::Color& _color, const Gdiplus::Color& _bgColor, bool _activateBlender)
{
	textPtr afficheurText = std::make_unique<PM3D::CAfficheurTexte>(m_pDispositif, _text, _x * m_screenWidthRatio, _y * m_screenHeightRatio, _width * m_screenWidthRatio, _height * m_screenHeightRatio, _font, _color, _bgColor);

	spritePtr afficheur = std::make_unique<PM3D::CAfficheurSprite>(m_pDispositif, _activateBlender);
	afficheur->AjouterSpriteTexte(afficheurText->GetTextureView(), afficheurText->getX(), afficheurText->getY());

	_texts.emplace_back(std::move(afficheurText));
	_sprites.emplace_back(std::move(afficheur));
}

void UI::addButton(spriteList& _sprites, buttonList& _buttons, std::unique_ptr<Button> _button)
{
	spritePtr afficheur = std::make_unique<PM3D::CAfficheurSprite>(m_pDispositif, true);
	afficheur->AjouterSpriteTexte(_button->GetTextureView(), _button->getX(), _button->getY());

	_buttons.emplace_back(std::move(_button));
	_sprites.emplace_back(std::move(afficheur));
}

void UI::updateScreenSizes()
{
	RECT rectFrame;
	GetWindowRect(m_handler, &rectFrame);

	m_screenWidth = (rectFrame.right - rectFrame.left);
	m_screenHeight = (rectFrame.bottom - rectFrame.top);

	m_screenWidthRatio = m_screenWidth / 1920;
	m_screenHeightRatio = m_screenHeight / 1080;
}
