/*
 * Copyright (C) 2023-2026  PvZ TV Touch Team
 *
 * This file is part of PlantsVsZombies-AndroidTV.
 */

#include "PvZ/Lawn/Widget/NetplayLobbyWidget.h"

#include "Homura/MemberUtils.h"
#include "PvZ/GlobalVariable.h"
#include "PvZ/Lawn/LawnApp.h"
#include "PvZ/Lawn/Widget/GameButton.h"
#include "PvZ/Lawn/Widget/WaitForSecondPlayerDialog.h"
#include "PvZ/NetPlay.h"
#include "PvZ/SexyAppFramework/Graphics/Font.h"
#include "PvZ/SexyAppFramework/Widget/ScrollWidget.h"
#include "PvZ/TodLib/Common/TodCommon.h"
#include "PvZ/TodLib/Common/TodStringFile.h"

#include <cstring>
#include <algorithm>
#include <mutex>

using namespace Sexy;

namespace {
constexpr int kLeftPanelX = 40;
constexpr int kPanelYOffset = 15;
constexpr int kLeftPanelY = 120 + kPanelYOffset;
constexpr int kLeftPanelWidth = 330;
constexpr int kPanelHeight = 490;
constexpr int kRightPanelX = 390;
constexpr int kRightPanelY = 120 + kPanelYOffset;
constexpr int kRightPanelWidth = 850;
constexpr int kSelectedServerLatencyX = kRightPanelX + 20;
constexpr int kSelectedServerLatencyY = 170 + kPanelYOffset;
constexpr int kServerRowY = 230 + kPanelYOffset;
constexpr int kServerRowHeight = 55;
constexpr int kRoomScrollX = 410;
constexpr int kRoomScrollY = 193 + kPanelYOffset;
constexpr int kRoomScrollWidth = 810;
constexpr int kRoomScrollHeight = 330;
constexpr int kRoomCardWidth = 390;
constexpr int kRoomCardHeight = 100;
constexpr int kRoomCardGap = 15;
constexpr int kMaxSpectatorNamesShown = 6;
constexpr int kLobbyStatusY = 630;
constexpr int kRoomExitedStatusY = 640;
constexpr int kRoomActionY = 545;
constexpr int kRoomActionWidth = 190;
constexpr int kRoomActionHeight = 45;
constexpr int kRoomActionGap = 15;

void DrawPanel(Graphics *g, const Rect &rect) {
    g->SetColor(Color(66, 36, 20, 210));
    g->FillRect(rect);
    g->SetColor(Color(210, 164, 91, 230));
    g->DrawRect(rect);
}

pvzstl::string BuildRoomTag(const ServerRoomItem &room) {
    if (room.protocolVersion != 0 && room.protocolVersion != NETPLAY_VERSION) {
        return TodStringTranslate(room.protocolVersion < NETPLAY_VERSION ? "[SERVER_ROOM_VERSION_ERROR_LOWER]" : "[SERVER_ROOM_VERSION_ERROR_HIGHER]");
    }
    if (room.gaming) {
        return TodStringTranslate("[TAG_GAMING]");
    }
    if (room.full && room.spectateAllowed) {
        return TodStringTranslate("[SPECTATE]");
    }
    if (room.full) {
        return TodStringTranslate("[TAG_FULL]");
    }
    return TodStringTranslate(room.hostProbeDone ? "[P2P_READY]" : "[P2P_NOT_READY]");
}

pvzstl::string BuildSpectatorsText(const WaitForSecondPlayerDialog *dialog) {
    pvzstl::string text = StrFormat(TodStringTranslate("[SPECTATORS_NUM]").c_str(), dialog->mServerSpectatorCount);
    if (dialog->mServerSpectatorCount <= 0) {
        text += "-";
        return text;
    }

    for (int i = 0; i < dialog->mServerSpectatorCount && i < kMaxSpectatorNamesShown; ++i) {
        if (i > 0) {
            text += ", ";
        }
        text += dialog->mServerSpectatorNames[i][0] != '\0' ? dialog->mServerSpectatorNames[i] : "-";
    }
    if (dialog->mServerSpectatorCount > kMaxSpectatorNamesShown) {
        text += ", ...";
    }
    return text;
}

bool IsRoomActive(const WaitForSecondPlayerDialog *dialog) {
    return dialog != nullptr && (dialog->mIsCreatingRoom || dialog->mIsJoiningRoom || dialog->mServerHosting || dialog->mServerJoined || dialog->mServerSpectating);
}

void DrawActiveRoomInfo(Graphics *g, const WaitForSecondPlayerDialog *dialog) {
    const int centerX = kRightPanelX + kRightPanelWidth / 2;
    if (dialog->mUIMode == UIMode::MODE2_WIFI) {
        const char *title = dialog->mIsCreatingRoom ? "[ROOM_CREATED_FMT]" : "[JOINING_MANUAL]";
        const char *playerName = (dialog->mApp != nullptr && dialog->mApp->mPlayerInfo != nullptr && dialog->mApp->mPlayerInfo->mName != nullptr) ? dialog->mApp->mPlayerInfo->mName : "-";
        const pvzstl::string text = dialog->mIsCreatingRoom ? StrFormat(TodStringTranslate(title).c_str(), playerName) : TodStringTranslate(title);
        TodDrawString(g, text, centerX, 300, FONT_DWARVENTODCRAFT18, Color(255, 238, 175), DS_ALIGN_CENTER);

        const pvzstl::string detail = dialog->mIsCreatingRoom
            ? (IsRemoteServer() ? StrFormat(TodStringTranslate("[OTHER_JOINED_FMT]").c_str(), gSecondPlayerName) : TodStringTranslate("[WAIT_OTHER_JOIN]"))
            : (IsRemoteClient() ? StrFormat(TodStringTranslate("[JOINED_MANUAL_FMT]").c_str(), gSecondPlayerName) : TodStringTranslate("[JOINING_MANUAL]"));
        TodDrawString(g, detail, centerX, 355, FONT_HOUSEOFTERROR20, Color(235, 220, 185), DS_ALIGN_CENTER);
        return;
    }

    const char *roomName = dialog->mServerHosting ? dialog->mServerHostedRoomName : dialog->mServerJoinedRoomName;
    if (roomName[0] == '\0') {
        roomName = "-";
    }
    const char *roleKey = dialog->mServerHosting ? "[HOST]" : (dialog->mServerSpectating ? "[SPECTATING]" : "[CLIENT]");
    const char *localName = (dialog->mApp != nullptr && dialog->mApp->mPlayerInfo != nullptr && dialog->mApp->mPlayerInfo->mName != nullptr) ? dialog->mApp->mPlayerInfo->mName : "-";
    const char *hostName = dialog->mServerHosting ? localName : (gServerHostName[0] != '\0' ? gServerHostName : "-");
    pvzstl::string guestName = "-";
    if (dialog->mServerHosting) {
        guestName = dialog->mServerHostHasGuest ? (gSecondPlayerName[0] != '\0' ? gSecondPlayerName : "-") : TodStringTranslate("[WAIT_OTHER_JOIN]");
    } else if (dialog->mServerJoined) {
        guestName = localName;
    } else if (gSecondPlayerName[0] != '\0') {
        guestName = gSecondPlayerName;
    }

    TodDrawString(g, roleKey, centerX, 245, FONT_DWARVENTODCRAFT24, Color(255, 238, 175), DS_ALIGN_CENTER);
    TodDrawString(g, roomName, centerX, 285, FONT_DWARVENTODCRAFT18, Color(210, 255, 180), DS_ALIGN_CENTER);
    TodDrawString(g, StrFormat("%s: %s", TodStringTranslate("[HOST]").c_str(), hostName), centerX, 325, FONT_HOUSEOFTERROR20, Color(235, 220, 185), DS_ALIGN_CENTER);
    TodDrawString(g, StrFormat("%s: %s", TodStringTranslate("[CLIENT]").c_str(), guestName.c_str()), centerX, 365, FONT_HOUSEOFTERROR20, Color(235, 220, 185), DS_ALIGN_CENTER);

    if (dialog->mServerHostSpectateAllowed || dialog->mServerJoinedSpectateAllowed || dialog->mServerSpectating || dialog->mServerSpectatorCount > 0) {
        TodDrawStringWrapped(g, BuildSpectatorsText(dialog), Rect(kRightPanelX + 145, 385, 560, 65), FONT_HOUSEOFTERROR16, Color(235, 220, 185), DS_ALIGN_CENTER, false);
    }
}
} // namespace

class NetplayRoomListWidget : public Widget {
public:
    NetplayLobbyWidget *mOwner;

    explicit NetplayRoomListWidget(NetplayLobbyWidget *owner) {
        Widget::_constructor();
        static void *sVTable[122];
        static std::once_flag sVTableInit;
        std::call_once(sVTableInit, [this] {
            std::memcpy(sVTable, vTable, sizeof(sVTable));
            sVTable[0] = (void *)homura::ExtractMemFuncPtr(&NetplayRoomListWidget::_destructor);
            sVTable[1] = (void *)homura::ExtractMemFuncPtr(&NetplayRoomListWidget::_destructor2);
            sVTable[36] = (void *)homura::ExtractMemFuncPtr(&NetplayRoomListWidget::Draw);
            sVTable[78] = (void *)homura::ExtractMemFuncPtr(&NetplayRoomListWidget::MouseDown);
        });
        vTable = sVTable;
        mOwner = owner;
    }

    ~NetplayRoomListWidget() {
        Widget::_destructor();
    }

    void Draw(Graphics *g) {
        if (mOwner == nullptr || mOwner->mDialog == nullptr) {
            return;
        }
        WaitForSecondPlayerDialog *dialog = mOwner->mDialog;
        const int count = mOwner->GetRoomCount();

        if (count <= 0) {
            if (IsRoomActive(dialog)) {
                return;
            }
            const char *message = dialog->mUIMode == UIMode::MODE3_SERVER ? "[SERVER_NO_ROOMS_TIP]" : "[NO_AVAILABLE_ROOMS]";
            TodDrawString(g, message, kRoomScrollWidth / 2, 150, FONT_HOUSEOFTERROR20, Color(255, 230, 170), DS_ALIGN_CENTER);
            return;
        }

        for (int i = 0; i < count; ++i) {
            const int column = i % 2;
            const int row = i / 2;
            const int x = column * (kRoomCardWidth + kRoomCardGap);
            const int y = row * (kRoomCardHeight + kRoomCardGap);
            const int selected = dialog->mUIMode == UIMode::MODE2_WIFI ? dialog->mSelectedServerIndex : dialog->mSelectedRoomIndex_Server;
            const bool isSelected = i == selected;

            g->SetColor(isSelected ? Color(104, 82, 35, 245) : Color(92, 58, 37, 230));
            g->FillRect(Rect(x, y, kRoomCardWidth, kRoomCardHeight));
            g->SetColor(isSelected ? Color(255, 224, 92) : Color(151, 111, 72));
            g->DrawRect(Rect(x, y, kRoomCardWidth, kRoomCardHeight));

            if (dialog->mUIMode == UIMode::MODE2_WIFI) {
                const auto &room = gServers[i];
                TodDrawString(g, room.name, x + kRoomCardWidth / 2, y + 38, FONT_DWARVENTODCRAFT18, Color(255, 244, 195), DS_ALIGN_CENTER);
                TodDrawString(g, StrFormat("%s:%d", room.ip, room.tcpPort), x + kRoomCardWidth / 2, y + 72, FONT_HOUSEOFTERROR16, Color(208, 208, 208), DS_ALIGN_CENTER);
            } else {
                const ServerRoomItem &room = dialog->mServerRooms[i];
                TodDrawString(g, room.name, x + kRoomCardWidth / 2, y + 38, FONT_DWARVENTODCRAFT18, Color(255, 244, 195), DS_ALIGN_CENTER);
                TodDrawString(g, BuildRoomTag(room), x + kRoomCardWidth / 2, y + 72, FONT_HOUSEOFTERROR16, Color(190, 225, 190), DS_ALIGN_CENTER);
            }
        }
    }

    void MouseDown(int x, int y, int theClickCount) {
        (void)theClickCount;
        if (mOwner == nullptr) {
            return;
        }
        const int column = x / (kRoomCardWidth + kRoomCardGap);
        const int row = y / (kRoomCardHeight + kRoomCardGap);
        if (column < 0 || column > 1 || x - column * (kRoomCardWidth + kRoomCardGap) >= kRoomCardWidth || y - row * (kRoomCardHeight + kRoomCardGap) >= kRoomCardHeight) {
            return;
        }
        mOwner->SelectRoom(row * 2 + column);
    }

protected:
    void _destructor() {
        Widget::_destructor();
    }
    void _destructor2() {
        delete this;
    }
};

NetplayLobbyWidget::NetplayLobbyWidget(WaitForSecondPlayerDialog *dialog) {
    Widget::_constructor();
    static void *sVTable[122];
    static std::once_flag sVTableInit;
    std::call_once(sVTableInit, [this] {
        std::memcpy(sVTable, vTable, sizeof(sVTable));
        sVTable[0] = (void *)homura::ExtractMemFuncPtr(&NetplayLobbyWidget::_destructor);
        sVTable[1] = (void *)homura::ExtractMemFuncPtr(&NetplayLobbyWidget::_destructor2);
        sVTable[29] = (void *)homura::ExtractMemFuncPtr(&NetplayLobbyWidget::AddedToManager);
        sVTable[30] = (void *)homura::ExtractMemFuncPtr(&NetplayLobbyWidget::RemovedFromManager);
        sVTable[36] = (void *)homura::ExtractMemFuncPtr(&NetplayLobbyWidget::Draw);
        sVTable[78] = (void *)homura::ExtractMemFuncPtr(&NetplayLobbyWidget::MouseDown);
    });
    vTable = sVTable;

    mDialog = dialog;
    Resize(LawnApp::FULLSCREEN_RECT.mX, LawnApp::FULLSCREEN_RECT.mY, LawnApp::FULLSCREEN_RECT.mWidth, LawnApp::FULLSCREEN_RECT.mHeight);
    mClip = true;
    mSelectedServerListIndex = 0;
    mZombieBackground = Rand(2);

    mRoomScrollWidget = new ScrollWidget();
    mRoomScrollWidget->Resize(kRoomScrollX, kRoomScrollY, kRoomScrollWidth, kRoomScrollHeight);
    mRoomScrollWidget->SetScrollMode(ScrollWidget::SCROLL_VERTICAL);
    mRoomScrollWidget->EnableBounce(false);
    mRoomListWidget = new NetplayRoomListWidget(this);
    mRoomListWidget->Resize(0, 0, kRoomScrollWidth, kRoomScrollHeight);

    mAddServerButton = MakeButton(NetplayLobbyWidget_AddServer, dialog, this, "[CONNECT_CUSTOM_SERVER]");
    mAddServerButton->Resize(60, 545 + kPanelYOffset, 290, 50);
    mCreateRoomButton = MakeButton(WaitForSecondPlayerDialog::WaitForSecondPlayerDialog_Right, dialog, this, "[CREATE_ROOM_BUTTON]");
    mCreateRoomButton->Resize(410, 545 + kPanelYOffset, 390, 50);
    mJoinRoomButton = MakeButton(WaitForSecondPlayerDialog::WaitForSecondPlayerDialog_Left, dialog, this, "[JOIN_ROOM_BUTTON]");
    mJoinRoomButton->Resize(830, 545 + kPanelYOffset, 390, 50);
    mReplayManageButton = MakeButton(NetplayLobbyWidget_ReplayManage, dialog, this, "[REPLAY_MANAGE]");
    mReplayManageButton->Resize(40, 650, 230, 50);
    mLocalBattleButton = MakeButton(NetplayLobbyWidget_LocalBattle, dialog, this, "[PLAY_OFFLINE]");
    mLocalBattleButton->Resize(525, 650, 230, 50);
    mBackButton = MakeButton(NetplayLobbyWidget_Back, dialog, this, "[BACK]");
    mBackButton->Resize(1010, 650, 230, 50);
    mPrimaryActionButton = MakeButton(NetplayLobbyWidget_PrimaryAction, dialog, this, "[START_GAME]");
    mPrimaryActionButton->Resize(410, 465 + kPanelYOffset, 390, 48);
    mRoomOptionButton = MakeButton(NetplayLobbyWidget_RoomOption, dialog, this, "[ENABLE_SPECTATE]");
    mRoomOptionButton->Resize(830, 465 + kPanelYOffset, 390, 48);
    mPrimaryActionButton->SetVisible(false);
    mRoomOptionButton->SetVisible(false);

    TodLoadResources("DelayLoad_Almanac");
}

NetplayLobbyWidget::~NetplayLobbyWidget() {
    _destructor();
}

void NetplayLobbyWidget::_destructor() {
    delete mRoomOptionButton;
    delete mPrimaryActionButton;
    delete mBackButton;
    delete mLocalBattleButton;
    delete mReplayManageButton;
    delete mJoinRoomButton;
    delete mCreateRoomButton;
    delete mAddServerButton;
    delete mRoomListWidget;
    delete mRoomScrollWidget;
    Widget::_destructor();
}

void NetplayLobbyWidget::_destructor2() {
    delete this;
}

void NetplayLobbyWidget::AddedToManager(WidgetManager *theWidgetManager) {
    WidgetContainer::AddedToManager(theWidgetManager);
    AddWidget(mRoomScrollWidget);
    mRoomScrollWidget->AddWidget(mRoomListWidget);
    AddWidget(mAddServerButton);
    AddWidget(mCreateRoomButton);
    AddWidget(mJoinRoomButton);
    AddWidget(mReplayManageButton);
    AddWidget(mLocalBattleButton);
    AddWidget(mBackButton);
    AddWidget(mPrimaryActionButton);
    AddWidget(mRoomOptionButton);
}

void NetplayLobbyWidget::RemovedFromManager(WidgetManager *theWidgetManager) {
    WidgetContainer::RemovedFromManager(theWidgetManager);
    mRoomScrollWidget->RemoveWidget(mRoomListWidget);
    RemoveWidget(mRoomScrollWidget);
    RemoveWidget(mAddServerButton);
    RemoveWidget(mCreateRoomButton);
    RemoveWidget(mJoinRoomButton);
    RemoveWidget(mReplayManageButton);
    RemoveWidget(mLocalBattleButton);
    RemoveWidget(mBackButton);
    RemoveWidget(mPrimaryActionButton);
    RemoveWidget(mRoomOptionButton);
}

int NetplayLobbyWidget::GetRoomCount() const {
    if (mDialog == nullptr || mDialog->mIsCreatingRoom || mDialog->mIsJoiningRoom || mDialog->mServerHosting || mDialog->mServerJoined || mDialog->mServerSpectating) {
        return 0;
    }
    if (mDialog->mUIMode == UIMode::MODE2_WIFI) {
        return std::max(0, gScannedServerCount);
    }
    if (mDialog->mUIMode == UIMode::MODE3_SERVER && mDialog->mServerConnected) {
        return std::max(0, mDialog->mServerRoomCount);
    }
    return 0;
}

void NetplayLobbyWidget::RefreshControls() {
    if (mDialog == nullptr) {
        return;
    }
    mDialog->RefreshButtons();
    mJoinRoomButton->SetLabel(*mDialog->mLeftButton->mLabel);
    mJoinRoomButton->mDisabled = mDialog->mLeftButton->mDisabled;
    mCreateRoomButton->SetLabel(*mDialog->mRightButton->mLabel);
    mCreateRoomButton->mDisabled = mDialog->mRightButton->mDisabled;

    const bool lanRoomActive = mDialog->mIsCreatingRoom || mDialog->mIsJoiningRoom;
    const bool serverRoomActive = mDialog->mServerHosting || mDialog->mServerJoined || mDialog->mServerSpectating;
    const bool roomActive = lanRoomActive || serverRoomActive;
    const bool serverIdleDisconnected = mDialog->mUIMode == UIMode::MODE3_SERVER && !mDialog->mServerConnected && !mDialog->mServerConnecting && !serverRoomActive;
    if (serverIdleDisconnected) {
        mCreateRoomButton->SetLabel("[CREATE_ROOM_BUTTON]");
        mCreateRoomButton->mDisabled = true;
    }

    // These two controls are fixed parts of the lobby layout. Context-sensitive
    // room actions are presented separately inside the room panel below.
    mAddServerButton->SetLabel("[CONNECT_CUSTOM_SERVER]");
    mAddServerButton->mDisabled = lanRoomActive || serverRoomActive || mDialog->mServerConnecting;
    mReplayManageButton->SetLabel("[REPLAY_MANAGE]");
    mReplayManageButton->mDisabled = lanRoomActive || serverRoomActive || mDialog->mServerConnected || mDialog->mServerConnecting;
    mLocalBattleButton->SetLabel("[PLAY_OFFLINE]");
    mLocalBattleButton->mDisabled = roomActive || mDialog->mServerConnecting;

    const bool showPrimaryAction = mDialog->mIsCreatingRoom || serverRoomActive;
    mPrimaryActionButton->SetVisible(showPrimaryAction);
    if (showPrimaryAction) {
        mPrimaryActionButton->SetLabel(*mDialog->mLawnYesButton->mLabel);
        mPrimaryActionButton->mDisabled = mDialog->mLawnYesButton->mDisabled;
    }

    const bool showRoomOption = mDialog->mUIMode == UIMode::MODE3_SERVER && mDialog->mServerHosting;
    mRoomOptionButton->SetVisible(showRoomOption);
    if (showRoomOption) {
        mRoomOptionButton->SetLabel(*mDialog->mLawnNoButton->mLabel);
        mRoomOptionButton->mDisabled = mDialog->mLawnNoButton->mDisabled;
    }

    if (roomActive) {
        GameButton *roomActions[4];
        int roomActionCount = 0;
        if (showPrimaryAction) {
            roomActions[roomActionCount++] = mPrimaryActionButton;
        }
        if (showRoomOption) {
            roomActions[roomActionCount++] = mRoomOptionButton;
        }
        // Hosts keep "kick guest" before "leave room". Guests and spectators
        // put the role-switch action before "leave room".
        if (mDialog->mServerJoined || mDialog->mServerSpectating) {
            roomActions[roomActionCount++] = mCreateRoomButton;
            roomActions[roomActionCount++] = mJoinRoomButton;
        } else {
            roomActions[roomActionCount++] = mJoinRoomButton;
            roomActions[roomActionCount++] = mCreateRoomButton;
        }

        const int actionsWidth = roomActionCount * kRoomActionWidth + (roomActionCount - 1) * kRoomActionGap;
        const int actionsX = kRoomScrollX + (kRoomScrollWidth - actionsWidth) / 2;
        for (int i = 0; i < roomActionCount; ++i) {
            roomActions[i]->Resize(actionsX + i * (kRoomActionWidth + kRoomActionGap), kRoomActionY, kRoomActionWidth, kRoomActionHeight);
        }
    } else {
        mCreateRoomButton->Resize(410, 545 + kPanelYOffset, 390, 50);
        mJoinRoomButton->Resize(830, 545 + kPanelYOffset, 390, 50);
        mPrimaryActionButton->Resize(410, 465 + kPanelYOffset, 390, 48);
        mRoomOptionButton->Resize(830, 465 + kPanelYOffset, 390, 48);
    }
}

void NetplayLobbyWidget::Draw(Graphics *g) {
    RefreshControls();
    const int roomRows = (GetRoomCount() + 1) / 2;
    const int roomContentHeight = std::max(kRoomScrollHeight, roomRows * kRoomCardHeight + std::max(0, roomRows - 1) * kRoomCardGap);
    if (mRoomListWidget->mHeight != roomContentHeight) {
        mRoomListWidget->Resize(0, 0, kRoomScrollWidth, roomContentHeight);
        // ScrollWidget caches its scroll range. Room lists are refreshed by the
        // networking layer, so explicitly update the cached range when their
        // content height changes.
        mRoomScrollWidget->ClientSizeChanged();
    }
    g->DrawImage(mZombieBackground ? IMAGE_ALMANAC_ZOMBIEBACK : IMAGE_ALMANAC_PLANTBACK, 0, 0);
    g->SetColor(Color(25, 15, 10, 75));
    g->FillRect(Rect(0, 0, mWidth, mHeight));
    DrawPanel(g, Rect(kLeftPanelX, kLeftPanelY, kLeftPanelWidth, kPanelHeight));
    DrawPanel(g, Rect(kRightPanelX, kRightPanelY, kRightPanelWidth, kPanelHeight));

    TodDrawString(g, "[NETPLAY_LOBBY_TITLE]", mWidth / 2, 115, addonFonts.JN_BOBO_HEI36, Color(255, 248, 195), DS_ALIGN_CENTER);
    TodDrawString(g, "[MODE_SERVER_TITLE]", kLeftPanelX + kLeftPanelWidth / 2, 170 + kPanelYOffset, FONT_DWARVENTODCRAFT18, Color(255, 226, 154), DS_ALIGN_CENTER);
    TodDrawString(g, "[AVAILABLE_ROOMS]", kRightPanelX + kRightPanelWidth / 2, 170 + kPanelYOffset, FONT_DWARVENTODCRAFT18, Color(255, 226, 154), DS_ALIGN_CENTER);

    if (IsRoomActive(mDialog)) {
        DrawActiveRoomInfo(g, mDialog);
    }

    const int targetCount = mDialog != nullptr ? mDialog->GetLobbyServerTargetCount() : 0;
    const int itemCount = 1 + targetCount;
    mSelectedServerListIndex = std::clamp(mSelectedServerListIndex, 0, std::max(0, itemCount - 1));
    for (int i = 0; i < itemCount; ++i) {
        const int y = kServerRowY + i * kServerRowHeight;
        const bool selected = i == mSelectedServerListIndex;
        g->SetColor(selected ? Color(95, 105, 45, 245) : Color(82, 58, 42, 225));
        g->FillRect(Rect(kLeftPanelX + 18, y - 30, kLeftPanelWidth - 36, 45));
        g->SetColor(selected ? Color(155, 225, 70) : Color(145, 105, 72));
        g->DrawRect(Rect(kLeftPanelX + 18, y - 30, kLeftPanelWidth - 36, 45));

        pvzstl::string label;
        if (i == 0) {
            label = TodStringTranslate("[WIFI_VS]");
        } else {
            char address[32]{};
            mDialog->GetLobbyServerTargetAddress(i - 1, address, sizeof(address));
            if (i <= 2) {
                label = StrFormat(TodStringTranslate("[OFFICIAL_SERVER_NAME]").c_str(), i, address);
            } else {
                label = StrFormat(TodStringTranslate("[CUSTOM_SERVER_NAME]").c_str(), i - 2, address);
            }
        }
        TodDrawString(g, label, kLeftPanelX + kLeftPanelWidth / 2, y, FONT_HOUSEOFTERROR16, selected ? Color(185, 255, 105) : Color(255, 238, 195), DS_ALIGN_CENTER);
    }

    if (mDialog != nullptr && mSelectedServerListIndex > 0 && mSelectedServerListIndex <= targetCount) {
        // Target probes are stopped and cleared after connecting. From that
        // point the room-list query round trip is the current server latency.
        const int latency = mDialog->mServerConnected ? mDialog->mServerLatencyMs : mDialog->mServerTargetLatencyMs[mSelectedServerListIndex - 1];
        const pvzstl::string latencyText = latency >= 0 ? StrFormat("%dms", latency) : "--ms";
        TodDrawString(g, latencyText, kSelectedServerLatencyX, kSelectedServerLatencyY, FONT_HOUSEOFTERROR16, Color(235, 220, 185), DS_ALIGN_LEFT);
    }

    if (mDialog != nullptr) {
        pvzstl::string status;
        if (mDialog->mUIMode == UIMode::MODE3_SERVER) {
            status = mDialog->mServerStatusText;
        } else if (mDialog->mIsCreatingRoom) {
            status = TodStringTranslate("[WAIT_OTHER_JOIN]");
        } else if (mDialog->mIsJoiningRoom) {
            status = TodStringTranslate("[JOINING_MANUAL]");
        } else {
            status = TodStringTranslate(gScannedServerCount > 0 ? "[AVAILABLE_ROOMS]" : "[SCANNING_ROOMS]");
        }
        const bool roomExited = status == TodStringTranslate("[STATUS_ROOM_EXITED]");
        TodDrawString(g, status, kRightPanelX + kRightPanelWidth / 2, roomExited ? kRoomExitedStatusY : kLobbyStatusY, FONT_HOUSEOFTERROR16, Color(235, 220, 185), DS_ALIGN_CENTER);
    }
}

void NetplayLobbyWidget::MouseDown(int x, int y, int theClickCount) {
    (void)theClickCount;
    const int firstTop = kServerRowY - 30;
    if (x < kLeftPanelX + 18 || x >= kLeftPanelX + kLeftPanelWidth - 18 || y < firstTop) {
        return;
    }
    const int index = (y - firstTop) / kServerRowHeight;
    if (y - (firstTop + index * kServerRowHeight) >= 45) {
        return;
    }
    SelectServer(index);
}

void NetplayLobbyWidget::SelectServer(int listIndex) {
    if (mDialog == nullptr || listIndex < 0 || listIndex > mDialog->GetLobbyServerTargetCount()) {
        return;
    }
    if (listIndex != mSelectedServerListIndex && mDialog->ServerHostRoomLocked()) {
        return;
    }
    if (listIndex == mSelectedServerListIndex && ((listIndex == 0 && mDialog->mUIMode == UIMode::MODE2_WIFI) || (listIndex > 0 && (mDialog->mServerConnected || mDialog->mServerConnecting)))) {
        return;
    }
    mSelectedServerListIndex = listIndex;
    mRoomScrollWidget->ScrollToMin(false);
    mDialog->mApp->PlaySample(SOUND_GRAVEBUTTON);
    if (listIndex == 0) {
        mDialog->SetMode(UIMode::MODE2_WIFI);
    } else {
        mDialog->SetMode(UIMode::MODE3_SERVER);
        mDialog->ConnectLobbyServerTarget(listIndex - 1);
    }
}

void NetplayLobbyWidget::SelectRoom(int roomIndex) {
    if (mDialog == nullptr || roomIndex < 0 || roomIndex >= GetRoomCount()) {
        return;
    }
    if (mDialog->mUIMode == UIMode::MODE2_WIFI) {
        mDialog->mSelectedServerIndex = roomIndex;
    } else {
        mDialog->mSelectedRoomIndex_Server = roomIndex;
    }
    mDialog->mApp->PlaySample(SOUND_GRAVEBUTTON);
}
