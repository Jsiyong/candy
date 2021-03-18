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
     * @param enterSubdirectory 是否递归获取
     * @return
     */
    static bool getFolderByPath(FolderVO &results, const std::string &path, bool enterSubdirectory = false);

    /**
     * 保存文件
     * @param path 文件路径
     * @param data 文件内容
     * @return
     */
    static bool saveFile(const std::string &path, const std::string &data);

    /**
     * 读取文件
     * @param path
     * @param data
     * @return
     */
    static bool readFile(const std::string &path, std::string &data);

    /**
     * 创建文件夹
     * @param path
     * @return
     */
    static bool createFolder(const std::string &path);

    static bool renameFile(const std::string &src, const std::string &target);
};

#endif //CANDY_FILESERVICE_H
