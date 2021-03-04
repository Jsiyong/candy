//
// Created by Jsiyong on 2021-03-04.
//
#include "fileservice.h"
#include "../../util/fileutil.h"
#include <sys/stat.h>

FolderVO FileService::getFolderByPath(const std::string &path) {
    FolderVO folderVo;
    folderVo.path = path;

    std::list<std::string> folders;
    std::list<std::string> files;
    struct stat statbuf;
    if (!FileUtil::scanDirectory(path, folders, files)) {
        return folderVo;
    }

    for (const auto &folderPath:folders) {
        FolderItem item;
        FileUtil::getFileInfo(path, statbuf);
        item.lastModifyTime = statbuf.st_mtim.tv_sec;
        item.name = folderPath;
        folderVo.folderList.push_back(item);
    }

    for (const auto &filePath:files) {
        File item;
        FileUtil::getFileInfo(path, statbuf);
        item.lastModifyTime = statbuf.st_mtim.tv_sec;
        item.size = statbuf.st_size;
        item.name = filePath;
        folderVo.fileList.push_back(item);
    }
    return folderVo;
}
