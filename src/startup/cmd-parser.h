//
// Created by jiangsiyong on 2020/12/15.
//

#ifndef CANDY_CMD_PARSER_H
#define CANDY_CMD_PARSER_H

/**
 * 命令行解析
 */
struct CmdParser {

    /**
     * 参数解析
     * @param argc 参数个数
     * @param argv 每个参数
     *
     * -h --help 帮助
     * -p --port 端口
     * -d --daemon 后台运行
     * -f --foreground 前台运行
     * -a --addr 监听地址
     *
     *
     */
    static void parse(int argc, char **argv);

private:
    /**
     * 打印帮助
     */
    static void printHelp();
};


#endif //CANDY_CMD_PARSER_H
