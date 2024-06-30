
# MemSubLoader
<p align="center">
  <img width="128" height="128" src="https://raw.githubusercontent.com/senolem/MemSubLoader/main/logo/logo.png">
</p>

MemSubLoader is a software that reads memory and loads subtitles for corresponding audio. It's made for old games that don't have any subtitles or when it's hard to edit them.

MemSubLoader should be compatible from Windows XP SP2 to Windows 11 latest build, thanks to using the old low-level Win32 API. This API is supposed to be available starting from Windows NT, but the support range is determined by the usage of the GDI+ library used to render the subtitles (requires Windows XP) and the usage of extended window styles (requires Windows XP SP2).

## Features
The current features are :

- Loading subtitles for games
- Settings for text style
- Ability to save and load configuration
- Autoload configuration

## Download
You can download the latest version here :

https://github.com/Urocu/MemSubLoader/releases/latest

## How it works
MemSubLoader uses json format to store subtitles, addresses, configuration and other information needed to work.

Subtitles info includes ID, text and identifier. Identifier is used to load specific configuration

Addresses are used to read specific places in the game's memory to determine when it should load subtitles and which text to load.

Configuration has info for how to display the text. More on that in the later part.

## Dependencies
We tried to keep external dependencies to a minimum. MemSubLoader currently depends on :
- [JsonCpp](https://github.com/open-source-parsers/jsoncpp "JsonCpp") for loading/saving the configuration
- [OutlineText](https://github.com/shaovoon/outline-text "OutlineText") for rendering outlines and shadows

## How to use
MemSubLoader is composed of three window, each one serving a different purpose :

###  Main window

<p align="center">
	<img src="https://github.com/senolem/MemSubLoader/blob/main/doc/main.png?raw=true">
</p>

- **File menu (1)** : In this menu you can either load or save a configuration. You can also set a configuration to be loaded when the app starts, but you will have to save a configuration first.

- **Subtitles menu (2)** : From this menu you can access the configurator window.

- **Game & subtitles path (3)** : You can click on each button to respectively select a game executable and a subtitles file. Both buttons open an explorer dialog from which you can select a file.

- **Start game button (4)** : After selecting a game executable and a subtitles file, clicking on this button will load the subtitles file and display a topmost window in which subtitles are rendered.

------------

###  Configurator window

<p align="center">
	<img src="https://github.com/senolem/MemSubLoader/blob/main/doc/configurator.png?raw=true">
</p>

- **Identifier list (1)** : List all identifiers registered in the configuration. Each identifier is relied to a subtitles configuration where you can customize subtitles appearance and positioning on a per-identifier basis. Note that the DEFAULT identifier cannot be deleted as it used by default configuration for dialogs that doesn't specify any identifier.

- **Identifier control buttons (2)** : You can perform various actions on your identifier list, as such as editing, creating, deleting or duplicating a configuration. You can also copy an identifier subtitles configuration to the DEFAULT one.

------------

###  Settings window
<p align="center">
	<img src="https://github.com/senolem/MemSubLoader/blob/main/doc/settings.png?raw=true">
</p>

- **Font customization (1)** : In this group you can select a font color (including the alpha blending, in others terms *transparency*), as well as the font.
*Note : Windows XP will replace the font color by the selected one in the font dialog, so make sure to select the color after selecting the font.*

- **Font alignment (2)** : Customize font horizontal and vertical alignment.

- **Font outline (3)** : Similarly to font customization, you can customize outline color (including alpha blending), as well as the outline width.

- **Font shadows (4)** : Similarly to outline customization, you can customize shadows color (including alpha blending), but you can also set how far from the text the shadow will be rendered. You can optionally enable shadows diffusing, but you will need to tweak settings before getting something that looks great. General recommandations are thickness from 8 to 12, and color alpha between 12 and 32.

- **Area positioniong and sizing (5)** : The area is where text is rendered on your screen. For 640x480 resolution we recommend a 100% width and at least 20% height for great readability. Checking *Area preview* will draw a red outline around the area to help you positionning it.
