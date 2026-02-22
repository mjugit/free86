/*
	
  Copyright © 2026 Maximilian Jung

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the “Software”), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the
  Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	
*/


#include "../Modules/GfxTk/Include/GfxTk.h"
#include "../Modules/Include/Collection.h"
#include "../Modules/Include/Heap.h"
#include "../Modules/Include/String.h"
#include "../Modules/Include/Collection.h"

#include "Include/Keyboard.h"


typedef struct {
  char Name[48];
  char *Text;
  char *Cursor;
  U32 Size;

  void (*OnKeyDown)(KeyEventArgs *eventArgs);
  void (*OnKeyUp)(KeyEventArgs *eventArgs);
} KShellBuffer;

typedef struct {
  Rgb18 Text;
  Rgb18 UiAccent;
  Rgb18 UiAccentAlt;
  Rgb18 EditorBg;
  Rgb18 Panel;
  Rgb18 TextAlt;
  Rgb18 Toolbar;
  Rgb18 Active;
} KShellTheme;


static const KShellTheme _ThemeBright = {
  .Text	       = (Rgb18) { 0, 0, 0 },
  .UiAccent    = (Rgb18) { 16, 20, 33 },
  .UiAccentAlt = (Rgb18) { 24, 27, 38 },
  .EditorBg    = (Rgb18) { 63, 63, 63 },
  .Panel       = (Rgb18) { 57, 57, 58 },
  .TextAlt     = (Rgb18) { 44, 45, 45 },
  .Toolbar     = (Rgb18) { 50, 53, 59 },
  .Active      = (Rgb18) { 63, 47, 5 }
};


#define COLOR_TEXT 0

#define COLOR_ACCENT 1

#define COLOR_ACCENT_ALT 2

#define COLOR_EDITOR_BG 3

#define COLOR_PANEL 4

#define COLOR_TEXT_ALT 5

#define COLOR_TOOLBAR 6

#define COLOR_ACTIVE 7

#define FONT_TOOLBAR ForgottenBold

#define FONT_STATUSBAR Widget

#define FONT_TAB WidgetBold

#define FONT_TEXT ZxCourier

#define FONT_ITALIC EnviousItalic

#define FONT_BOLD EnviousBold



typedef struct {
  HeapArea *Heap;
  
  Font ToolbarFont;
  Font StatusbarFont;
  Font TabFont;

  Font TextFont;
  Font ItalicFont;
  Font BoldFont;
  KShellTheme *Theme;


  List *Buffers;
  KShellBuffer *ActiveBuffer;
  KeyEventArgs KeyArgs;
} KShellState;



import(Vga);
import(Renderer);
import(String);
import(Collection);
import(Heap);
import(Keyboard);

static KShellState _State;



// ----------------------------------------------------------------------
// Layout
// ----------------------------------------------------------------------

static inline void _SetColorTheme(const KShellTheme *theme) {
  Vga.SetPalette(COLOR_TEXT, theme->Text);
  Vga.UseColor(COLOR_TEXT, COLOR_TEXT);
  
  Vga.SetPalette(COLOR_ACCENT, theme->UiAccent);
  Vga.UseColor(COLOR_ACCENT, COLOR_ACCENT);
  
  Vga.SetPalette(COLOR_ACCENT_ALT, theme->UiAccentAlt);
  Vga.UseColor(COLOR_ACCENT_ALT, COLOR_ACCENT_ALT);
  
  Vga.SetPalette(COLOR_EDITOR_BG, theme->EditorBg);
  Vga.UseColor(COLOR_EDITOR_BG, COLOR_EDITOR_BG);

  Vga.SetPalette(COLOR_PANEL, theme->Panel);
  Vga.UseColor(COLOR_PANEL, COLOR_PANEL);

  Vga.SetPalette(COLOR_TEXT_ALT, theme->TextAlt);
  Vga.UseColor(COLOR_TEXT_ALT, COLOR_TEXT_ALT);

  Vga.SetPalette(COLOR_TOOLBAR, theme->Toolbar);
  Vga.UseColor(COLOR_TOOLBAR, COLOR_TOOLBAR);

  Vga.SetPalette(COLOR_ACTIVE, theme->Active);
  Vga.UseColor(COLOR_ACTIVE, COLOR_ACTIVE);
}

static void _SetupKeyHandlers(void);
static void _KeyDebug(KeyEventArgs *eventArgs);


static void _TextBuffer_HandleInput(KeyEventArgs *eventArgs) {
  char glyph = Keyboard.GetChar(eventArgs->KeyCode, *eventArgs->Modifiers);
  if (!glyph)
    return;

  if (_State.ActiveBuffer) {
    switch (glyph) {
    case '\b':
      if (_State.ActiveBuffer->Cursor <= _State.ActiveBuffer->Text)
	break;
      
      *(_State.ActiveBuffer->Cursor--) = 0;
      *(_State.ActiveBuffer->Cursor) = 0;
      break;
      
    default:
      *(_State.ActiveBuffer->Cursor++) = glyph;
      *_State.ActiveBuffer->Cursor = 0;
      break;
    }
  }

  eventArgs->Handled = true;
}


#define _KSHELL_DEFAULTBUFFER_SIZE 1024

static void _InitializeDefaultBuffer(void) {
  KShellBuffer *defaultBuffer = Heap.Allocate(_State.Heap, sizeof(KShellBuffer));
  *defaultBuffer = (KShellBuffer) {
    .Name = "MyBuffer",
    .Text = Heap.Allocate(_State.Heap, _KSHELL_DEFAULTBUFFER_SIZE),
    .Size = _KSHELL_DEFAULTBUFFER_SIZE,
    .OnKeyDown = &_TextBuffer_HandleInput
  };

  defaultBuffer->Cursor = defaultBuffer->Text;
  Collection.List.Add(_State.Buffers, defaultBuffer);
  _State.ActiveBuffer = defaultBuffer;
}

void KShell_Initialize(VgaConfig *config, HeapArea *heap) {
  Bitmap8x8* toolbarFontBitmap = Renderer.GetFontBitmap(FONT_TOOLBAR);
  Renderer.RenderFont(&_State.ToolbarFont, toolbarFontBitmap, "Toolbar", false);

  Bitmap8x8* statusbarFontBitmap = Renderer.GetFontBitmap(FONT_STATUSBAR);
  Renderer.RenderFont(&_State.StatusbarFont, statusbarFontBitmap, "Statusbar", false);
  
  Bitmap8x8* tabFontBitmap = Renderer.GetFontBitmap(FONT_TAB);
  Renderer.RenderFont(&_State.TabFont, tabFontBitmap, "Tab", false);

  Bitmap8x8* textFontBitmap = Renderer.GetFontBitmap(FONT_TEXT);
  Renderer.RenderFont(&_State.TextFont, textFontBitmap, "Text", false);

  Bitmap8x8* italicFontBitmap = Renderer.GetFontBitmap(FONT_ITALIC);
  Renderer.RenderFont(&_State.ItalicFont, italicFontBitmap, "TextItalic", false);

  Bitmap8x8* boldFontBitmap = Renderer.GetFontBitmap(FONT_BOLD);
  Renderer.RenderFont(&_State.BoldFont, boldFontBitmap, "TextBold", false);
  
  
  _State.Theme = (KShellTheme*)&_ThemeBright;
  _State.Heap = heap;
  _SetColorTheme(_State.Theme);

  // Initialize buffers
  _State.Buffers = Collection.List.Create(_State.Heap);
  _InitializeDefaultBuffer();
  _SetupKeyHandlers();
}


#define _KSHELL_ZERO ((Vector2d) { 0, 0 })


#define _KSHELL_TOOLBAR_HEIGHT 50

static void _DrawToolbar(VgaConfig *config) {
  Vector2d toolbarSize = { config->Resolution.X, _KSHELL_TOOLBAR_HEIGHT };

  Renderer.RenderFilledRect(config, _KSHELL_ZERO, toolbarSize, COLOR_TOOLBAR);
  Renderer.RenderAsciiZ(config, (Vector2d) { 18, 3 }, "free86 debug session", &_State.ToolbarFont, COLOR_ACCENT);
  Renderer.RenderAsciiZ(config, (Vector2d) { 18, 13 }, "GFXTK Renderer", &_State.ToolbarFont, COLOR_ACCENT);
}


#define _KSHELL_STATUSBAR_HEIGHT 14

char text[40];
static void _DrawStatusbar(VgaConfig *config) {
  Vector2d statusbarSize = { config->Resolution.X, _KSHELL_STATUSBAR_HEIGHT };
  Vector2d statusbarStart = { 0, config->Resolution.Y - _KSHELL_STATUSBAR_HEIGHT};
  
  Renderer.RenderFilledRect(config, statusbarStart, statusbarSize, COLOR_ACCENT);
  Vector2d statusbarTextStart = { statusbarStart.X + 3, statusbarStart.Y + 4 };
  
  char statusbarText[128] = { };
  U32 percentFree = (_State.Heap->TotalBytesFree * 100) / _State.Heap->TotalBytes;
  String.Format(statusbarText, "%d of %d bytes free (%d%%)", _State.Heap->TotalBytesFree, _State.Heap->TotalBytes,  percentFree);
  Renderer.RenderAsciiZ(config, statusbarTextStart, statusbarText, &_State.StatusbarFont, COLOR_TEXT_ALT);
  Renderer.RenderAsciiZ(config, (Vector2d) { 10, 30 }, text, &_State.StatusbarFont, COLOR_TEXT);
}


#define _KSHELL_BORDER_LEFT 15

#define _KSHELL_BORDER_RIGHT 320

#define _KSHELL_BORDER_TOP 5

#define _KSHELL_BORDER_BOTTOM 10

#define _KSHELL_BORDER_WIDTH 3

#define _KSHELL_TABNAME_HEIGHT 12

#define _KSHELL_TABNAME_WIDTH  100

#define _KSHELL_TABHEADER_HEIGHT 14



static void _DrawContentArea(VgaConfig *config) {
  U16 areaStartY = _KSHELL_TOOLBAR_HEIGHT + _KSHELL_BORDER_TOP + _KSHELL_TABHEADER_HEIGHT;
  U16 borderTotal = _KSHELL_BORDER_LEFT + _KSHELL_BORDER_RIGHT;
  U16 areaEndY = config->Resolution.Y - _KSHELL_BORDER_BOTTOM - _KSHELL_STATUSBAR_HEIGHT;
  
  Vector2d areaStart = { _KSHELL_BORDER_LEFT, areaStartY };
  Vector2d areaSize = { config->Resolution.X - borderTotal, areaEndY - areaStartY };

  // Editor background
  Renderer.RenderFilledRect(config, areaStart, areaSize, COLOR_EDITOR_BG);
  // Border
  Renderer.RenderRect(config, areaStart, areaSize, _KSHELL_BORDER_WIDTH, COLOR_PANEL);
  // Header
  Renderer.RenderFilledRect(config, areaStart, (Vector2d) { areaSize.X, _KSHELL_TABHEADER_HEIGHT}, COLOR_PANEL);
  
  if (_State.ActiveBuffer) {
    Vector2d headerTextStart = (Vector2d) {
      areaStart.X + _KSHELL_BORDER_WIDTH,
      areaStart.Y + _KSHELL_BORDER_WIDTH
    };
    
    Renderer.RenderAsciiZ(config, headerTextStart, _State.ActiveBuffer->Name, &_State.TabFont, COLOR_TEXT);
    Vector2d bodyTextStart = (Vector2d) {
      areaStart.X + _KSHELL_BORDER_WIDTH + 30,
      areaStart.Y + _KSHELL_BORDER_WIDTH + _KSHELL_TABHEADER_HEIGHT
    };
    
    Renderer.RenderAsciiZ(config, bodyTextStart, _State.ActiveBuffer->Text, &_State.TextFont, COLOR_TEXT);
  }
}

void KShell_DrawLayout(VgaConfig *config) {
  Renderer.FillScreen(config, COLOR_ACCENT_ALT);
  _DrawToolbar(config);
  _DrawStatusbar(config);

  _DrawContentArea(config);
}




typedef void (*KShell_KeyEventHandler)(KeyEventArgs *eventArgs);

static List *_GlobalKeyHandlers;
static KeyEventArgs _KeyArgs;

static void _NotifyKeyPressIterate(void *state) {
  if (_State.KeyArgs.Handled)
    return;

  KShell_KeyEventHandler eventHandler = *((KShell_KeyEventHandler*)state);
  eventHandler(&_State.KeyArgs);
}

void KShell_NotifyKeyPress(KeyEventArgs eventArgs) {
  _State.KeyArgs = eventArgs;
  
  // Try global handler first
  if (_GlobalKeyHandlers->Count)
      Collection.List.ForEach(_GlobalKeyHandlers, &_NotifyKeyPressIterate);
  if (_State.KeyArgs.Handled)
    return;

  // Call buffer handler
  if (_State.ActiveBuffer) {
    if (!_State.KeyArgs.WasKeyPress && _State.ActiveBuffer->OnKeyDown)
      _State.ActiveBuffer->OnKeyDown(&_State.KeyArgs);
    else if (_State.ActiveBuffer->OnKeyUp)
      _State.ActiveBuffer->OnKeyUp(&_State.KeyArgs);
  }
}


static void _KeyDebug(KeyEventArgs *eventArgs) {
  String.Format(text, "KeyCode: %d", eventArgs->KeyCode);
}

static void _SetupKeyHandlers(void) {
  _GlobalKeyHandlers = Collection.List.Create(_State.Heap);
  Collection.List.Add(_GlobalKeyHandlers, _KeyDebug);
}
