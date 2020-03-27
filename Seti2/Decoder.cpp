#include "Decoder.h"

bool decod(Polynom b, Polynom g) {
    Polynom tmp = b.mod(g);
    if (tmp.getWeight() == 0) {
        return false;
    }
    return true;
}

