//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPVITVEHAFFMAN_CODER_H
#define ADAPVITVEHAFFMAN_CODER_H

#include <string>


class Coder {
public:
    virtual void encode(const std::string& filename) = 0;
    virtual void decode(const std::string& filename) = 0;
    virtual ~Coder();
};


#endif //ADAPVITVEHAFFMAN_CODER_H
