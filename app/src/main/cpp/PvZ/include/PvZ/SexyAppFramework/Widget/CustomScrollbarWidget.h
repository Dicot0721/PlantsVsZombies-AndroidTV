#ifndef PVZ_SEXYAPPFRAMEWORK_WIDGET_CUSTOM_SCROLLBAR_WIDGET_H
#define PVZ_SEXYAPPFRAMEWORK_WIDGET_CUSTOM_SCROLLBAR_WIDGET_H

#include "ScrollbarWidget.h"

namespace Sexy {

// 没有新增任何成员，6，这个类不知道什么意义
class CustomScrollbarWidget : public ScrollbarWidget {
public:
    // 大小90个整数

    void RemoveScrollButtons();
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_WIDGET_CUSTOM_SCROLLBAR_WIDGET_H
