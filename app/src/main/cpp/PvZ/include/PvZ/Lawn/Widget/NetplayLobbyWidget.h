/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 */

#ifndef PVZ_LAWN_WIDGET_NETPLAY_LOBBY_WIDGET_H
#define PVZ_LAWN_WIDGET_NETPLAY_LOBBY_WIDGET_H

#include "PvZ/SexyAppFramework/Widget/Widget.h"

class GameButton;
class NetplayRoomListWidget;
class WaitForSecondPlayerDialog;

namespace Sexy {
class ScrollWidget;
class WidgetManager;
} // namespace Sexy

class NetplayLobbyWidget : public Sexy::Widget {
public:
    enum {
        NetplayLobbyWidget_AddServer = 1200,
        NetplayLobbyWidget_ReplayManage = 1201,
        NetplayLobbyWidget_Back = 1202,
        NetplayLobbyWidget_PrimaryAction = 1203,
        NetplayLobbyWidget_RoomOption = 1204,
        NetplayLobbyWidget_LocalBattle = 1205,
    };

    WaitForSecondPlayerDialog *mDialog;
    Sexy::ScrollWidget *mRoomScrollWidget;
    NetplayRoomListWidget *mRoomListWidget;
    GameButton *mAddServerButton;
    GameButton *mCreateRoomButton;
    GameButton *mJoinRoomButton;
    GameButton *mReplayManageButton;
    GameButton *mLocalBattleButton;
    GameButton *mBackButton;
    GameButton *mPrimaryActionButton;
    GameButton *mRoomOptionButton;
    int mSelectedServerListIndex;
    bool mZombieBackground;

    explicit NetplayLobbyWidget(WaitForSecondPlayerDialog *dialog);
    ~NetplayLobbyWidget();

    void AddedToManager(Sexy::WidgetManager *theWidgetManager);
    void RemovedFromManager(Sexy::WidgetManager *theWidgetManager);
    void Draw(Sexy::Graphics *g);
    void MouseDown(int x, int y, int theClickCount);
    void RefreshControls();
    void SelectServer(int listIndex);
    void SelectRoom(int roomIndex);
    int GetRoomCount() const;

protected:
    void _destructor();
    void _destructor2();
};

#endif // PVZ_LAWN_WIDGET_NETPLAY_LOBBY_WIDGET_H
