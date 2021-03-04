//
// Created by Jsiyong on 2021-03-04.
//

#ifndef CANDY_FILESERVICE_H
#define CANDY_FILESERVICE_H

#include "../vo/foldervo.h"

struct FileService {

    /**
     * 通过文件夹路径获取文件夹内部信息
     * @param path
     * @return
     */
    static FolderVO getFolderByPath(const std::string &path);
};

#endif //CANDY_FILESERVICE_H
