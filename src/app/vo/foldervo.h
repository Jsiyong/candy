//
// Created by Jsiyong on 2021-03-04.
//

#ifndef CANDY_FOLDERVO_H
#define CANDY_FOLDERVO_H

#include <string>
#include <vector>
#include "../../protocol/json/serialization.h"

/**
 * 文件夹信息
 */
struct FolderItem {
    std::string name = "";//文件夹名称
    long long lastModifyTime = 0;//创建时间
};

Serialization(FolderItem, name, lastModifyTime)

/**
 * 文件信息
 */
struct File {
    std::string name = "";//文件的名称
    long long lastModifyTime = 0;//创建时间
    long long size = 0;//大小
};

Serialization(File, name, lastModifyTime, size)

/**
 * 返回前端查看的文件夹信息
 */
struct FolderVO {
    std::string path = "";//所在路径
    std::vector<FolderItem> folderList;//文件夹列表
    std::vector<File> fileList;//文件列表
};

Serialization(FolderVO, path, folderList, fileList)

#endif //CANDY_FOLDERVO_H
