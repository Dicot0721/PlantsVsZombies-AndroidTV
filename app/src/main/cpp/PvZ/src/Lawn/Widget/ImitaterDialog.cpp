#include "PvZ/Lawn/Widget/ImitaterDialog.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/SeedChooserScreen.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include "PvZ/SexyAppFramework/Misc/KeyCodes.h"

void ImitaterDialog_ImitaterDialog(LawnDialog *instance, int playerIndex) {
    // 记录当前游戏状态
    return old_ImitaterDialog_ImitaterDialog(instance, playerIndex);
}

void ImitaterDialog_ShowToolTip(LawnDialog *instance) {
    // 在触控模式下不显示ToolTip
    if (!keyboardMode)
        return;
    return old_ImitaterDialog_ShowToolTip(instance);
}

bool ImitaterDialog_KeyDown(LawnDialog *a, int a2) {
    // 修复不选择模仿者卡片并退出时的闪退
    if (a2 == Sexy::KEYCODE_ESCAPE || a2 == Sexy::KEYCODE_ESCAPE2) {
        LawnApp *lawnApp = a->mApp;
        SeedChooserScreen *seedChooserScreen = lawnApp->mSeedChooserScreen;
        Sexy_Widget_RemoveWidget(seedChooserScreen, a);
        (*(void (**)(LawnApp *, Sexy::Widget *))(*(uint32_t *)lawnApp + 188))(lawnApp, a);
        seedChooserScreen->mImitaterDialogOpened = 0;
        return true;
    }
    return old_ImitaterDialog_KeyDown(a, a2);
}

void ImitaterDialog_MouseDown(LawnDialog *a, int x, int y, int theCount) {
    SeedType seedType = ImitaterDialog_SeedHitTest(a, x, y);
    if (seedType == SeedType::SEED_NONE) {
        return;
    }
    LawnApp *lawnApp = a->mApp;
    SeedChooserScreen *seedChooserScreen = lawnApp->mSeedChooserScreen;

    if (!seedChooserScreen->SeedNotAllowedToPick(seedType)) {
        ChosenSeed *chosenSeed = &(seedChooserScreen->mChosenSeeds[SeedType::SEED_IMITATER]);
        chosenSeed->mImitaterType = seedType;
        chosenSeed->mSeedState = ChosenSeedState::SEED_IN_CHOOSER;
        seedChooserScreen->GetSeedPositionInChooser(SeedType::SEED_IMITATER, chosenSeed->mX, chosenSeed->mY);
        //        SeedChooserScreen_ClickedSeedInChooser(*(uint32_t *) (a[184] + 2244), seedChooserScreen + 835, -1);//模仿者选种子闪退，就是因为这里参数为-1而不是0或者1
        //        int mIsZombie = a[194];
        seedChooserScreen->ClickedSeedInChooser(chosenSeed, m1PChoosingSeeds ? 0 : 1);
        seedChooserScreen->UpdateImitaterButton();
        //        (*(void (**)(int, int)) (*(uint32_t *) a[184] + 428))(a[184], a[179]);
        Sexy_Widget_RemoveWidget(seedChooserScreen, a);
        (*(void (**)(LawnApp *, Sexy::Widget *))(*(uint32_t *)lawnApp + 188))(lawnApp, a);
        seedChooserScreen->mImitaterDialogOpened = 0;
    }
}
