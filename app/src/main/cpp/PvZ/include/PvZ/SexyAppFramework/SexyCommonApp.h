//
// Created by Administrator on 2026/1/15.
//

#ifndef PLANTSVSZOMBIES_ANDROIDTV_SEXYCOMMONAPP_H
#define PLANTSVSZOMBIES_ANDROIDTV_SEXYCOMMONAPP_H


#include "SexyApp.h"
namespace Sexy {


class __SexyCommonApp : public __SexyApp {
public:
    static void getGameInfo(pvzstl::string *theStrings, __SexyCommonApp *app) {
        return reinterpret_cast<void (*)(pvzstl::string *, __SexyCommonApp *)>(Sexy_SexyCommonApp_getGameInfoAddr)(theStrings, app);
    }
};

class SexyCommonApp : public __SexyCommonApp {};

} // namespace Sexy


#endif // PLANTSVSZOMBIES_ANDROIDTV_SEXYCOMMONAPP_H
