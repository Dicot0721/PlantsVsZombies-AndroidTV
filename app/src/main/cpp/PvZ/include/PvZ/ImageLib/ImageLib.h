#ifndef PVZ_IMAGELIB_IMAGE_LIB_H
#define PVZ_IMAGELIB_IMAGE_LIB_H

#include <string>

namespace ImageLib {

class Image {
public:
    int *vTable;         // 0
    int mWidth;          // 1
    int mHeight;         // 2
    unsigned int *mBits; // 3
    int *mText1;         // 4
    int *mText2;         // 5
    bool unkBool;        // 24
    int unk1[256];       // 7 ~ 262
    int unkInt;          // 263
}; // 264个整数

} // namespace ImageLib


#endif // PVZ_IMAGELIB_IMAGE_LIB_H
