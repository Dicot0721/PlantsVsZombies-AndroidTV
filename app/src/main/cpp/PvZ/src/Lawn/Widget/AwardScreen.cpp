#include "PvZ/Lawn/Widget/AwardScreen.h"
#include "PvZ/Lawn/Board/Board.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/SexyAppFramework/Widget/GameButton.h"
#include "PvZ/Symbols.h"

void AwardScreen::MouseDown(int x, int y, int theClickCount) {
    Rect aRect = {mStartButton->mX, mStartButton->mY, mStartButton->mWidth, mStartButton->mHeight};
    if (aRect.Contains(x, y)) {
        mApp->PlaySample(*Sexy_SOUND_TAP_Addr);
    }
}

void AwardScreen::MouseUp(int x, int y, int theClickCount) {
    Rect aRect = {mStartButton->mX, mStartButton->mY, mStartButton->mWidth, mStartButton->mHeight};
    if (aRect.Contains(x, y)) {
        StartButtonPressed();
    }
}
