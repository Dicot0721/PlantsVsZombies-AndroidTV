#include "PvZ/Lawn/System/PlayerInfo.h"
#include "PvZ/Lawn/LawnApp.h"

void LawnPlayerInfo::AddCoins(int theAmount) {
    // 用于 购物愉快 成就
    int aNumCoin = mCoins + theAmount;
    if (aNumCoin > 99999) {
        aNumCoin = 99999;
    }
    if (aNumCoin <= 0) {
        aNumCoin = 0;
    }
    mCoins = aNumCoin;
    if (theAmount < 0) {
        mUsedCoins -= theAmount;
        if (mUsedCoins >= 2500) {
            LawnApp *gLawnApp = (LawnApp *)*gLawnApp_Addr;
            gLawnApp->GrantAchievement(AchievementId::ACHIEVEMENT_SHOP);
        }
    }
}