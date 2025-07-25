#include "PvZ/Lawn/BaseGamepadControls.h"
#include "PvZ/GlobalVariable.h"

void BaseGamepadControls::GetGamepadVelocity(float *horizontal, float *vertical) {
    // 如果horizontal或vertical不为零，则会在重型武器中播放小推车移动动画。
    // 所以这里将vertical设置为非零，以播放动画；同时horizontal设置为0，以免小推车移动。
    *horizontal = 0.0f;
    *vertical = gPlayerIndex == TouchPlayerIndex::TOUCHPLAYER_NONE ? 0.0f : 100.0f;
}