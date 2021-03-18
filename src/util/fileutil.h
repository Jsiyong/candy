//
// Created by jiangsiyong on 2020/12/19.
//

#ifndef CANDY_FILEUTIL_H
#define CANDY_FILEUTIL_H

#include <fcntl.h>
#include <string>
#include <list>

/**
 * 文件相关工具类
 */
struct FileUtil {
    /**
     * 为文件描述符添加标志
     * @param fd
     * @param flag
     * @return
     */
    static int addFlag2Fd(int fd, int flag);

    /**
     * 设置非阻塞形式
     * @param fd
     * @return
     */
    static int setNonBlock(int fd);


    /**
     * 读取文件到dst中
     * @param path 路径
     * @param dst
     * @return
     */
    static bool readFile(const std::string &path, std::string &dst);

    /**
     * 写文件
     * @param path
     * @param src
     * @return
     */
    static bool writeFile(const std::string &path, const std::string &src, mode_t mode = 0777);

    /**
     * 判断文件是否存在
     * @param path
     * @return
     */
    static bool isExists(const std::string &path);

    /**
     * 判断文件是不是常规文件
     * @param path
     * @return
     */
    static bool isRegularFile(const std::string &path);

    /**
     * 查看目录下的情况
     * @param path
     * @param results
     */
    static bool scanDirectory(const std::string &path, std::list<std::string> &results);

    /**
     * 获取文件的信息
     * @return
     */
    static bool getFileInfo(const std::string &path, struct stat &fileInfo);

    /**
     * 创建文件夹
     * @param path
     * @param mode
     * @return
     */
    static bool createFolder(const std::string &path, mode_t mode);

    static bool renameFile(const std::string &src, const std::string &target);

};

#endif //CANDY_FILEUTIL_H
