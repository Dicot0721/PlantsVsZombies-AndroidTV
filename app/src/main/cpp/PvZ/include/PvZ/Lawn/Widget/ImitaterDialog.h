#ifndef PVZ_LAWN_IMITATER_DIALOG_H
#define PVZ_LAWN_IMITATER_DIALOG_H

#include "LawnDialog.h"
#include "PvZ/Lawn/Common/ConstEnums.h"

class ImitaterDialog : public LawnDialog {
public:
    SeedType SeedHitTest(int x, int y) { return reinterpret_cast<SeedType (*)(ImitaterDialog *, int, int)>(ImitaterDialog_SeedHitTestAddr)(this, x, y); }

    void MouseDown(int x, int y, int theCount);
};


inline void (*old_ImitaterDialog_ImitaterDialog)(ImitaterDialog *instance, int a2);

inline void (*old_ImitaterDialog_ShowToolTip)(ImitaterDialog *instance);

inline bool (*old_ImitaterDialog_KeyDown)(ImitaterDialog *a, int a2);

inline void (*old_ImitaterDialog_MouseDown)(ImitaterDialog *a, int x, int y, int theCount);


void ImitaterDialog_ImitaterDialog(ImitaterDialog *instance, int playerIndex);

void ImitaterDialog_ShowToolTip(ImitaterDialog *instance);

bool ImitaterDialog_KeyDown(ImitaterDialog *a, int a2);

#endif // PVZ_LAWN_IMITATER_DIALOG_H
