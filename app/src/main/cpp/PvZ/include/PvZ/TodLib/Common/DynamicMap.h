#ifndef PLANTSVSZOMBIES_ANDROIDTV_DYNAMICMAP_H
#define PLANTSVSZOMBIES_ANDROIDTV_DYNAMICMAP_H

#include <unordered_map>

class DynamicMap {
public:
    void Add(short x, short y) {
        mapping_[x] = y; // 覆盖或新增
    }

    bool Lookup(short x, short& out) const {
        auto it = mapping_.find(x);
        if (it == mapping_.end())
            return false;
        out = it->second;
        return true;
    }

    short Lookup(short x) {
        auto it = mapping_.find(x);
        if (it == mapping_.end())
            return it->second;
        return 0;
    }

    // 清空所有映射关系
    void ClearAll() {
        mapping_.clear();
    }

private:
    std::unordered_map<short, short> mapping_;
};

#endif // PLANTSVSZOMBIES_ANDROIDTV_DYNAMICMAP_H
