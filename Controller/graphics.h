
//alignment values
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTRE 2

#define VALIGN_TOP 0
#define VALIGN_BOTTOM 1
#define VALIGN_CENTRE 2


extern unsigned int colour;
extern unsigned int backcolour;
extern unsigned char backmode;
extern unsigned char linespacing;
extern unsigned char align;
extern unsigned char valign;
extern unsigned char wordwrap;
extern unsigned char autofontsize;
extern int fontsize;
extern unsigned char Trigger;
extern char chname[16][16];
extern uint8_t level[16];
extern uint8_t volume[16];
extern uint8_t pan[16];
extern uint8_t link[16];

extern volatile uint8_t EncValue;
extern volatile uint8_t EncChange;
extern volatile uint8_t KeyState;
extern volatile uint8_t KeyHit;
extern volatile uint8_t LastKey;


void SenseEncoder(void);
void SenseKeys(void);
void TftInit(void);
void HslToRgbw(uint8_t *opr, uint8_t *og, uint8_t *ob, uint8_t h, uint8_t s, uint8_t spread, uint8_t dim);
void ColTemp(uint8_t *opr, uint8_t *og, uint8_t *ob, uint8_t h, uint8_t dim);
uint8_t CalcWhite(uint8_t r, uint8_t g, uint8_t b);
void OutputRemap(void);
void setfont(char size, char style);
unsigned int stringheight(char *c);
unsigned int linewidth(char *c);
void opclrscr(void);
void UserRectClr(void);
void putstr(char *c);
void putstr_align(char *c);
void setxy(int x, int y);
void LogoDisplay(void);
void SymbolDisplay(int px, int py, char v);
void BitmapDisplay(uint8_t n, uint32_t txp, uint32_t typ, uint8_t overlay);
void FadeDisplay(uint32_t st, uint32_t end);
void ColourDisplay(void);
void ScrollDisplay(void);
void WaveDisplay(void);
void ShowPointer(uint8_t val);
void ErasePointer(void);
void ShowChanBox(uint8_t chan, uint8_t active);
void ShowChanVolume(uint8_t ch, uint8_t v);
void ShowChanBalance(uint8_t ch, uint8_t v);
void ShowAudioLevel(uint8_t ch, uint8_t v);

void disp_menuclear(void);
void disp_putmenuline(char y, const char *s, char style);
void disp_putmenuip(char y, char sel, uint32_t ip);
void disp_menu_scrollbar(char typ);
void disp_info(char typ, char *s);