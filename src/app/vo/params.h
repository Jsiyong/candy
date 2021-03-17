//
// Created by Jsiyong on 2021-03-04.
//

#ifndef CANDY_PARAMS_H
#define CANDY_PARAMS_H

#include <string>
#include "../../protocol/json/serialization.h"

struct FolderCreateParam {
    std::string path;
    std::string name;
};

Serialization(FolderCreateParam, path, name)

#endif //CANDY_PARAMS_H
