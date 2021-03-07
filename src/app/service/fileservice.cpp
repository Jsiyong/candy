//
// Created by Jsiyong on 2021-03-04.
//
#include "fileservice.h"
#include "../../util/fileutil.h"
#include <sys/stat.h>

bool FileService::getFolderByPath(FolderVO &results, const std::string &path, bool enterSubdirectory) {
    int sepIndex = path.rfind('/');
    results.path = path.substr(0, sepIndex);
    results.name = path.substr(sepIndex + 1);

    std::list<std::string> paths;
    if (!FileUtil::scanDirectory(path, paths)) {
        return false;
    }

    struct stat statBuf{0};
    for (const std::string &name:paths) {
        std::string sub = path + "/" + name;
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
