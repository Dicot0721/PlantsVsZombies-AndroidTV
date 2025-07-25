#include "PvZ/SexyAppFramework/Widget/CustomScrollbarWidget.h"

using namespace Sexy;

void CustomScrollbarWidget::RemoveScrollButtons() {
    // 修复点击图鉴介绍文本的滚动条时闪退的问题。将Remove改为Hide，从而解决缺少mUpButton、mDownButton导致的空指针闪退。
    mUpButton->mVisible = false;
    mDownButton->mVisible = false;
}
