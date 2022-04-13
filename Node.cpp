//
// Created by gunSlaveUnit on 13.04.2022.
//

#include "Node.h"

uint32_t Node::updateWeights() {
    if (s != '\0')
        return w;
    else {
        uint32_t weight = 0;
        if (l)
            weight += l->updateWeights();
        if (r)
            weight += r->updateWeights();

        w = weight;

        return w;
    }
}
