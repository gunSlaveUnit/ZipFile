//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPVITVEHAFFMAN_ADAPVITEHAFFMANCODER_H
#define ADAPVITVEHAFFMAN_ADAPVITEHAFFMANCODER_H

#include "Coder.h"


class AdapviteHaffmanCoder final: Coder {
    void encode(const std::string &filename) override;
    void decode(const std::string &filename) override;
};


#endif //ADAPVITVEHAFFMAN_ADAPVITEHAFFMANCODER_H
