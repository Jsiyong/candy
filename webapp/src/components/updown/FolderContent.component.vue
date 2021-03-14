<template>
    <div class="folder-content">
        <div class="folder-bg">
            <el-table
                    height="100%"
                    :data="fileList"
                    style="width: 100%;">
                <el-table-column type="expand">
                    <template slot-scope="props">
                        <el-form label-position="left" inline class="file-table-expand">
                            <el-form-item label="所在路径">
                                <span>{{ props.row.path }}</span>
                            </el-form-item>
                            <el-form-item label="名称">
                                <span>{{ props.row.name }}</span>
                            </el-form-item>
                            <el-form-item label="修改日期">
                                <i class="el-icon-time"></i>
                                <span style="margin-left: 10px">{{ props.row.lastModifyTime }}</span>
                            </el-form-item>
                        </el-form>
                    </template>
                </el-table-column>
                <el-table-column label="名称" header-align="center">
                    <template slot-scope="scope">
                        <el-tag size="medium">{{ scope.row.name }}</el-tag>
                    </template>
                </el-table-column>
                <el-table-column label="大小" width="180" header-align="center" align="center">
                    <template slot-scope="scope">
                        <span style="margin-left: 10px">{{ scope.row.size }}</span>
                    </template>
                </el-table-column>
                <el-table-column label="操作" width="300" header-align="center" align="center">
                    <template slot-scope="scope">
                        <el-button
                                size="mini"
                                type="primary"
                                @click="handleEdit(scope.$index, scope.row)">编辑
                        </el-button>
                        <el-button
                                size="mini"
                                type="danger"
                                @click="handleDelete(scope.$index, scope.row)">删除
                        </el-button>
                        <el-button
                                size="mini"
                                type="success"
                                @click="handleDownload(scope.$index, scope.row)">下载
                        </el-button>
                    </template>
                </el-table-column>
            </el-table>
        </div>
    </div>
</template>

<script>
    module.exports = {
        name: 'FolderContent',
        props: ["fileList"],
        data() {
            return {}
        },
        methods: {
            handleDelete(index, row) {
                console.log(index, row);
            },
            handleEdit(index, row) {
                console.log(index, row);
            },
            handleDownload(index, row) {
                console.log(index, row);
                axios.get(`${window.$config.addr}/download?path=${row.path + "/" + row.name}`, {responseType: 'blob'}).then((res) => {
                    console.log(res)
                    // console.log(res.data)
                    // new Blob([data])用来创建URL的file对象或者blob对象
                    let url = window.URL.createObjectURL(new Blob([res.data]));
                    // 生成一个a标签
                    let link = document.createElement("a");
                    link.style.display = "none";
                    link.href = url;
                    // 生成时间戳
                    let timestamp = new Date().getTime();
                    link.download = row.name;
                    document.body.appendChild(link);
                    link.click();
                    document.body.removeChild(link); //下载完成移除元素
                    window.URL.revokeObjectURL(url); //释放内存
                })
            }
        },
        created() {
        }
    }
</script>
<style>
    .folder-content {
        margin: 30px;
        background: linear-gradient(to left, #89ba86, #89ba86) left top no-repeat,
        linear-gradient(to bottom, #89ba86, #89ba86) left top no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) right top no-repeat,
        linear-gradient(to bottom, #89ba86, #89ba86) right top no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) left bottom no-repeat,
        linear-gradient(to bottom, #89ba86, #89ba86) left bottom no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) right bottom no-repeat,
        linear-gradient(to left, #89ba86, #89ba86) right bottom no-repeat;
        background-size: 3px 50px, 50px 3px, 3px 50px, 50px 3px;
        pointer-events: all;
    }

    .folder-content .folder-bg {
        background: #525d4e0f;
        width: calc(100% - 20px);
        height: calc(100% - 20px);
        margin: 10px;
        display: flex;
    }

    .file-table-expand {
        font-size: 0;
    }

    .file-table-expand label {
        width: 10%;
        color: #99a9bf;
    }

    .file-table-expand .el-form-item {
        margin-right: 0;
        margin-bottom: 0;
        width: 100%;
    }
</style>