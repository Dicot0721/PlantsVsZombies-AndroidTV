#ifndef PVZ_SEXYAPPFRAMEWORK_MISC_FLAGS_H
#define PVZ_SEXYAPPFRAMEWORK_MISC_FLAGS_H

namespace Sexy {

class FlagsMod {
public:
    int mAddFlags;
    int mRemoveFlags;

    FlagsMod() {
        mAddFlags = 0;
        mRemoveFlags = 0;
    }
};

} // namespace Sexy

#endif // PVZ_SEXYAPPFRAMEWORK_MISC_FLAGS_H
