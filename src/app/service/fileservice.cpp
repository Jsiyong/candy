//
// Created by Jsiyong on 2021-03-04.
//
#include "fileservice.h"
#include "../../util/fileutil.h"
#include <sys/stat.h>

bool FileService::getFolderByPath(FolderVO &results, const std::string &path, bool enterSubdirectory) {
    int sepIndex = path.rfind('/');
    results.path = path.substr(0, sepIndex);
    results.name = path == "/" ? "/" : path.substr(sepIndex + 1);

    std::list<std::string> paths;
    if (!FileUtil::scanDirectory(path, paths)) {
        return false;
    }

    struct stat statBuf{0};
    for (const std::string &name:paths) {
        std::string sub = path == "/" ? "/" + name : path + "/" + name;
        FileUtil::getFileInfo(sub, statBuf);
        if (S_ISDIR(statBuf.st_mode)) {
            //如果是路径文件
            FolderVO folderVo;
            folderVo.name = name;
            folderVo.path = path;
            //如果递归获取，那么就递归起来
            if (enterSubdirectory) {
                getFolderByPath(folderVo, sub, enterSubdirectory);
            }
            results.folderList.emplace_back(folderVo);
        } else {
            //如果是普通文件
            FileVO fileVo;
            fileVo.name = name;
            fileVo.path = path;
            fileVo.size = statBuf.st_size;
            fileVo.lastModifyTime = statBuf.st_mtim.tv_sec;
            results.fileList.emplace_back(fileVo);
        }
    }
    return true;
}

bool FileService::saveFile(const std::string &path, const std::string &data) {
    return FileUtil::writeFile(path, data);
}

bool FileService::readFile(const std::string &path, std::string &data) {
    return FileUtil::readFile(path, data);
}

bool FileService::createFolder(const std::string &path) {
    return FileUtil::createFolder(path, 777);
}

bool FileService::renameFile(const std::string &src, const std::string &target) {
    return FileUtil::renameFile(src, target);
}

bool FileService::delFile(const std::string &src) {
    //判断是文件还是文件夹
    struct stat statBuf{0};
    FileUtil::getFileInfo(src, statBuf);
    if (S_ISDIR(statBuf.st_mode)) {
        return FileUtil::deleteFolder(src);
    } else {
        return FileUtil::deleteFile(src);
    }
}
