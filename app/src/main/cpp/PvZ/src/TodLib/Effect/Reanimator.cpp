#include "PvZ/TodLib/Effect/Reanimator.h"
#include "PvZ/Lawn/Common/ConstEnums.h"
#include "PvZ/Lawn/Widget/MainMenu.h"
#include "PvZ/Misc.h"
#include "PvZ/SexyAppFramework/Graphics/Graphics.h"
#include <regex>

bool Reanimation::DrawTrack(Sexy::Graphics *g, int theTrackIndex, int theRenderGroup, TodTriangleGroup *theTriangleGroup) {
    // 修复模仿者植物变白
    if (mFilterEffect != FilterEffect::FILTEREFFECT_NONE) {
        ReanimatorTransform *reanimatorTransform = mReanimatorTransforms + theTrackIndex;
        Sexy::Image *image = reanimatorTransform->mImage;
        ReanimatorTrackInstance *reanimatorTrackInstance = mTrackInstances + theTrackIndex;
        Sexy::Image *mImageOverride = reanimatorTrackInstance->mImageOverride;
        if (image != nullptr) {
            reanimatorTransform->mImage = FilterEffectGetImage(image, mFilterEffect);
        }
        if (mImageOverride != nullptr) {
            reanimatorTrackInstance->mImageOverride = FilterEffectGetImage(mImageOverride, mFilterEffect);
        }
        bool result = old_Reanimation_DrawTrack(this, g, theTrackIndex, theRenderGroup, theTriangleGroup);
        reanimatorTransform->mImage = image;
        reanimatorTrackInstance->mImageOverride = mImageOverride;
        return result;
    }

    return old_Reanimation_DrawTrack(this, g, theTrackIndex, theRenderGroup, theTriangleGroup);
}

int Reanimation_HideTrack(Reanimation *reanim, const char *trackName, bool hide) {
    int trackIndex = reanim->FindTrackIndex(trackName);
    if (trackIndex != -1) {
        ReanimatorTrackInstance *reanimatorTrackInstance = reanim->mTrackInstances + trackIndex;
        reanimatorTrackInstance->mRenderGroup = hide ? RENDER_GROUP_HIDDEN : RENDER_GROUP_NORMAL;
    }
    return trackIndex;
}

void Reanimation_HideTrackById(Reanimation *reanim, int trackIndex, bool hide) {
    if (trackIndex != -1) {
        ReanimatorTrackInstance *reanimatorTrackInstance = reanim->mTrackInstances + trackIndex;
        reanimatorTrackInstance->mRenderGroup = hide ? RENDER_GROUP_HIDDEN : RENDER_GROUP_NORMAL;
    }
}

void Reanimation_HideTrackByPrefix(Reanimation *reanim, const char *trackPrefix, bool hide) {
    ReanimatorDefinition *mDefinition = reanim->mDefinition;
    int mTrackCount = mDefinition->mTrackCount;
    if (mTrackCount <= 0) {
        return;
    }
    ReanimatorTrack *mTracks = mDefinition->mTracks;
    for (int i = 0; i < mTrackCount; ++i) {
        const char *mName = (mTracks + i)->mName;
        if (trackPrefix == nullptr || strstr(mName, trackPrefix) != nullptr) {
            Reanimation_HideTrackById(reanim, i, hide);
        }
    }
}

void Reanimation_SetImageOrigin(Reanimation *reanim, const char *trackName, Sexy::Image *theImage) {
    // 和Reanimation_SetImageOverride不一样的是，这个直接替换原始图像。
    int theTrackIndex = reanim->FindTrackIndex(trackName);
    if (theTrackIndex != -1) {
        ReanimatorTransform *reanimatorTransform = reanim->mReanimatorTransforms + theTrackIndex;
        reanimatorTransform->mImage = theImage;
    }
}

void Reanimation_SetImageDefinition(Reanimation *reanim, const char *trackName, Sexy::Image *theImage) {
    // 和Reanimation_SetImageOrigin不一样的是，这个能对默认动画中没有贴图的生效。
    int theTrackIndex = reanim->FindTrackIndex(trackName);
    if (theTrackIndex != -1) {
        //        ReanimatorFrameTime theFrameTime;
        //        Reanimation_GetFrameTime(reanim, &theFrameTime);
        ReanimatorTrack *reanimatorTrack = reanim->mDefinition->mTracks + theTrackIndex;
        int mTransformCount = reanimatorTrack->mTransformCount;
        for (int i = 0; i < mTransformCount; ++i) {
            reanimatorTrack->mTransforms[i].mImage = theImage;
        }
    }
}


// "addonFiles/compiled/reanim/zombatar_zombie_head.reanim"
static ReanimationParams gNewReanimationParamArray[] = {
    {ReanimationType::REANIM_ZOMBATAR_HEAD, "addonFiles/compiled/reanim/zombatar_zombie_head.reanim", 1},
};

void ReanimatorLoadDefinitions(ReanimationParams *theReanimationParamArray, int theReanimationParamArraySize) {
    if (theReanimationParamArraySize == 0) {
        return old_ReanimatorLoadDefinitions(theReanimationParamArray, theReanimationParamArraySize);
    }
    int newReanimationArraySize = sizeof(gNewReanimationParamArray) / sizeof(gNewReanimationParamArray[0]);
    ReanimationParams *newReanimationParamArray = (ReanimationParams *)malloc((theReanimationParamArraySize + newReanimationArraySize) * sizeof(ReanimationParams));
    for (int i = 0; i < theReanimationParamArraySize; ++i) {
        newReanimationParamArray[i].mReanimationType = theReanimationParamArray[i].mReanimationType;
        newReanimationParamArray[i].mReanimFileName = theReanimationParamArray[i].mReanimFileName;
        newReanimationParamArray[i].mReanimParamFlags = theReanimationParamArray[i].mReanimParamFlags;
    }
    for (int i = 0; i < newReanimationArraySize; ++i) {
        newReanimationParamArray[i + theReanimationParamArraySize].mReanimationType = gNewReanimationParamArray[i].mReanimationType;
        newReanimationParamArray[i + theReanimationParamArraySize].mReanimFileName = gNewReanimationParamArray[i].mReanimFileName;
        newReanimationParamArray[i + theReanimationParamArraySize].mReanimParamFlags = gNewReanimationParamArray[i].mReanimParamFlags;
    }
    return old_ReanimatorLoadDefinitions(newReanimationParamArray, theReanimationParamArraySize + newReanimationArraySize);
}

void Reanimation_SetZombatarReanim(Reanimation *zombatarReanim) {
    Reanimation_HideTrackByPrefix(zombatarReanim, "hats", true);
    Reanimation_HideTrackByPrefix(zombatarReanim, "hair", true);
    Reanimation_HideTrackByPrefix(zombatarReanim, "facialHair", true);
    Reanimation_HideTrackByPrefix(zombatarReanim, "accessories", true);
    Reanimation_HideTrackByPrefix(zombatarReanim, "eyeWear", true);
    Reanimation_HideTrackByPrefix(zombatarReanim, "tidBits", true);
    zombatarReanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
}

void Reanimation_SetZombatarHats(Reanimation *zombatarReanim, unsigned char hats, unsigned char hatsColor) {
    Reanimation_HideTrackByPrefix(zombatarReanim, "hats", true);
    if (hats != 255) {
        char hatsChar[] = "hats_00";
        int hatsLength = strlen(hatsChar);
        if (hatsLength >= 2) {
            // 将数字转换为字符串并替换最后两个字符
            snprintf(&hatsChar[hatsLength - 2], 3, "%02d", hats); // %02d 确保数字有两位，不足则补零
        }
        Reanimation_HideTrackByPrefix(zombatarReanim, hatsChar, false);
        if (hatsColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::HAT, hats)) {
            int theTrackIndex = zombatarReanim->FindTrackIndex(hatsChar);
            ReanimatorTrackInstance *reanimatorTrackInstance = zombatarReanim->mTrackInstances + theTrackIndex;
            reanimatorTrackInstance->mTrackColor = gZombatarAccessoryColor2[hatsColor];
        }
    }
    zombatarReanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
}

void Reanimation_SetZombatarHair(Reanimation *zombatarReanim, unsigned char hair, unsigned char hairColor) {
    Reanimation_HideTrackByPrefix(zombatarReanim, "hair", true);
    if (hair != 255) {
        char hairChar[] = "hair_00";
        int hairLength = strlen(hairChar);
        if (hairLength >= 2) {
            // 将数字转换为字符串并替换最后两个字符
            snprintf(&hairChar[hairLength - 2], 3, "%02d", hair); // %02d 确保数字有两位，不足则补零
        }
        Reanimation_HideTrackByPrefix(zombatarReanim, hairChar, false);
        if (hairColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::HAIR, hair)) {
            int theTrackIndex = zombatarReanim->FindTrackIndex(hairChar);
            ReanimatorTrackInstance *reanimatorTrackInstance = zombatarReanim->mTrackInstances + theTrackIndex;
            reanimatorTrackInstance->mTrackColor = gZombatarAccessoryColor[hairColor];
        }
    }
    zombatarReanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
}

void Reanimation_SetZombatarFHair(Reanimation *zombatarReanim, unsigned char facialHair, unsigned char facialHairColor) {
    Reanimation_HideTrackByPrefix(zombatarReanim, "facialHair", true);
    if (facialHair != 255) {
        char facialHairChar[] = "facialHair_00";
        int facialHairLength = strlen(facialHairChar);
        if (facialHairLength >= 2) {
            // 将数字转换为字符串并替换最后两个字符
            snprintf(&facialHairChar[facialHairLength - 2], 3, "%02d", facialHair); // %02d 确保数字有两位，不足则补零
        }
        Reanimation_HideTrackByPrefix(zombatarReanim, facialHairChar, false);
        if (facialHairColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::FHAIR, facialHair)) {
            int theTrackIndex = zombatarReanim->FindTrackIndex(facialHairChar);
            ReanimatorTrackInstance *reanimatorTrackInstance = zombatarReanim->mTrackInstances + theTrackIndex;
            reanimatorTrackInstance->mTrackColor = gZombatarAccessoryColor[facialHairColor];
        }
    }
    zombatarReanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
}

void Reanimation_SetZombatarAccessories(Reanimation *zombatarReanim, unsigned char accessories, unsigned char accessoriesColor) {
    Reanimation_HideTrackByPrefix(zombatarReanim, "accessories", true);
    if (accessories != 255) {
        char accessoriesChar[] = "accessories_00";
        int accessoriesLength = strlen(accessoriesChar);
        if (accessoriesLength >= 2) {
            unsigned char accessoriesFix;
            switch (accessories) {
                case 5:
                    accessoriesFix = 14;
                    break;
                case 6:
                    accessoriesFix = 5;
                    break;
                case 7:
                    accessoriesFix = 6;
                    break;
                case 8:
                    accessoriesFix = 12;
                    break;
                case 9:
                    accessoriesFix = 7;
                    break;
                case 10:
                    accessoriesFix = 9;
                    break;
                case 11:
                    accessoriesFix = 10;
                    break;
                case 12:
                    accessoriesFix = 11;
                    break;
                case 14:
                    accessoriesFix = 8;
                    break;
                default:
                    accessoriesFix = accessories;
                    break;
            }
            // 将数字转换为字符串并替换最后两个字符
            snprintf(&accessoriesChar[accessoriesLength - 2], 3, "%02d", accessoriesFix); // %02d 确保数字有两位，不足则补零
        }
        Reanimation_HideTrackByPrefix(zombatarReanim, accessoriesChar, false);
        if (accessoriesColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::ACCESSORY, accessories)) {
            int theTrackIndex = zombatarReanim->FindTrackIndex(accessoriesChar);
            ReanimatorTrackInstance *reanimatorTrackInstance = zombatarReanim->mTrackInstances + theTrackIndex;
            reanimatorTrackInstance->mTrackColor = gZombatarAccessoryColor2[accessoriesColor];
        }
    }
    zombatarReanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
}

void Reanimation_SetZombatarEyeWear(Reanimation *zombatarReanim, unsigned char eyeWear, unsigned char eyeWearColor) {
    Reanimation_HideTrackByPrefix(zombatarReanim, "eyeWear", true);
    if (eyeWear != 255) {
        char eyeWearChar[] = "eyeWear_00";
        int eyeWearLength = strlen(eyeWearChar);
        if (eyeWearLength >= 2) {
            // 将数字转换为字符串并替换最后两个字符
            snprintf(&eyeWearChar[eyeWearLength - 2], 3, "%02d", eyeWear); // %02d 确保数字有两位，不足则补零
        }
        Reanimation_HideTrackByPrefix(zombatarReanim, eyeWearChar, false);
        if (eyeWearColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::EYEWEAR, eyeWear)) {
            int theTrackIndex = zombatarReanim->FindTrackIndex(eyeWearChar);
            ReanimatorTrackInstance *reanimatorTrackInstance = zombatarReanim->mTrackInstances + theTrackIndex;
            reanimatorTrackInstance->mTrackColor = gZombatarAccessoryColor2[eyeWearColor];
        }
    }
    zombatarReanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
}

void Reanimation_SetZombatarTidBits(Reanimation *zombatarReanim, unsigned char tidBits, unsigned char tidBitsColor) {
    Reanimation_HideTrackByPrefix(zombatarReanim, "tidBits", true);
    if (tidBits != 255) {
        char tidBitsChar[] = "tidBits_00";
        int tidBitsLength = strlen(tidBitsChar);
        if (tidBitsLength >= 2) {
            // 将数字转换为字符串并替换最后两个字符
            snprintf(&tidBitsChar[tidBitsLength - 2], 3, "%02d", tidBits); // %02d 确保数字有两位，不足则补零
        }
        Reanimation_HideTrackByPrefix(zombatarReanim, tidBitsChar, false);
        if (tidBitsColor != 255 && ZombatarWidget_AccessoryIsColorized(ZombatarWidget::TIDBIT, tidBits)) {
            int theTrackIndex = zombatarReanim->FindTrackIndex(tidBitsChar);
            ReanimatorTrackInstance *reanimatorTrackInstance = zombatarReanim->mTrackInstances + theTrackIndex;
            reanimatorTrackInstance->mTrackColor = gZombatarAccessoryColor2[tidBitsColor];
        }
    }
    zombatarReanim->Update(); // 一次Update是必要的，否则绘制出来是Empty
}

void Reanimation_GetZombatarTrackIndex(Reanimation *zombatarReanim, int *indexArray) {
    ReanimatorDefinition *mDefinition = zombatarReanim->mDefinition;
    int mTrackCount = mDefinition->mTrackCount;
    if (mTrackCount <= 0) {
        return;
    }
    const char *stringArray[] = {"hats", "eyeWear"};
    //    char *stringArray[] = {"hats","hair","facialHair","accessories","eyeWear","tidBits"};
    ReanimatorTrack *mTracks = mDefinition->mTracks;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < mTrackCount; ++j) {
            const char *mName = (mTracks + j)->mName;
            //            LOGD("%s",mName);
            if (strstr(mName, stringArray[i]) != nullptr) {
                ReanimatorTrackInstance *reanimatorTrackInstance = zombatarReanim->mTrackInstances + j;
                //                LOGD("%d",reanimatorTrackInstance->mRenderGroup);
                if (reanimatorTrackInstance->mRenderGroup != -1) {
                    indexArray[i] = j;
                    break;
                }
            }
        }
        indexArray[i] = -1;
    }
}

int Reanimation::GetZombatarHatTrackIndex() {
    int mTrackCount = mDefinition->mTrackCount;
    if (mTrackCount <= 0) {
        return -1;
    }
    std::regex pattern(R"(hats_\d{2})");


    //    char *stringArray[] = {"hats","hair","facialHair","accessories","eyeWear","tidBits"};
    ReanimatorTrack *mTracks = mDefinition->mTracks;
    for (int j = 0; j < mTrackCount; ++j) {
        const char *mName = (mTracks + j)->mName;
        //        LOGD("%s,%d",mName,std::regex_match(mName, pattern));
        if (std::regex_match(mName, pattern)) {
            ReanimatorTrackInstance *reanimatorTrackInstance = mTrackInstances + j;
            if (reanimatorTrackInstance->mRenderGroup != -1) {
                return j;
            }
        }
    }
    return -1;
}

int Reanimation::GetZombatarEyeWearTrackIndex() {
    int mTrackCount = mDefinition->mTrackCount;
    if (mTrackCount <= 0) {
        return -1;
    }
    std::regex pattern(R"(eyeWear_\d{2})");

    ReanimatorTrack *mTracks = mDefinition->mTracks;
    for (int j = 0; j < mTrackCount; ++j) {
        const char *mName = (mTracks + j)->mName;
        //        LOGD("%s,%d",mName,std::regex_match(mName, pattern));
        if (std::regex_match(mName, pattern)) {
            ReanimatorTrackInstance *reanimatorTrackInstance = mTrackInstances + j;
            if (reanimatorTrackInstance->mRenderGroup != -1) {
                return j;
            }
        }
    }
    return -1;
}

void DefinitionGetCompiledFilePathFromXMLFilePath(pvzstl::string &absPath, const pvzstl::string &defPathString) {
    // 从addonFiles读取新增的reanim文件
    if (defPathString.contains("addonFiles")) {
        absPath.emplace(defPathString + ".compiled");
        return;
    }
    return old_DefinitionGetCompiledFilePathFromXMLFilePath(absPath, defPathString);
}

bool Reanimation::ShouldTriggerTimedEvent(float theEventTime) {
    if (mFrameCount == 0 || mLastFrameTime <= 0.0f || mAnimRate <= 0.0f) // 没有动画或倒放或未播放
        return false;

    if (mAnimTime >= mLastFrameTime) // 一般情况下，可触发的范围为 [mLastFrameTime, mAnimTime]
        return theEventTime >= mLastFrameTime && theEventTime < mAnimTime;
    else // 若动画正好完成一次循环而重新进入下一次循环，则可触发的范围为 [0, mAnimTime] ∪ [mLastFrameTime, 1]
        return theEventTime >= mLastFrameTime || theEventTime < mAnimTime;
}

void Reanimation::AssignRenderGroupToTrack(const char *theTrackName, int theRenderGroup) {
    for (int i = 0; i < mDefinition->mTrackCount; i++)
        if (strcasecmp(mDefinition->mTracks[i].mName, theTrackName) == 0) {
            mTrackInstances[i].mRenderGroup = theRenderGroup; // 仅设置首个名称恰好为 theTrackName 的轨道
            return;
        }
}

void Reanimation::Draw(Graphics* g)
{
    DrawRenderGroup(g, RENDER_GROUP_NORMAL);
}

ReanimatorTrackInstance* Reanimation::GetTrackInstanceByName(const char* theTrackName)
{
    return &mTrackInstances[FindTrackIndex(theTrackName)];
}