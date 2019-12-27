#include <iostream>
#include "pvz.h"
#include <unistd.h>
int main(int argc, const char * argv[]) {
    PvZ pvz;
    // 原版补丁
    pvz.PatchClickCard();
    pvz.PatchUseCard();
    pvz.PatchRedNutExplode();
    // 作弊
    pvz.EasyCheat(true);
    return 0;
}
