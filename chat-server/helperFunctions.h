//
// Created by dean on 17-3-19.
//

#ifndef CHAT_SERVER_HELPERFUNCTIONS_H
#define CHAT_SERVER_HELPERFUNCTIONS_H

#include <string.h>

namespace help {
    inline int compareInput(const char input[], const char expectedResult[]) {
        int resultLength = strlen(expectedResult);
        return strlen(input) >= resultLength && strncmp(expectedResult, input, resultLength) == 0;
    }
}

#endif //CHAT_SERVER_HELPERFUNCTIONS_H
