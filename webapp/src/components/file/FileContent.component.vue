<template>
    <div class="file-wrapper"
         v-loading="loading"
         element-loading-text="上传中">
        <input ref="fileSelector" name="file" type="file" @change="handleUpload">
        <el-button class="upload-button top-button" type="primary" size="medium" @click="clickSelectFile"><i
                class="el-icon-upload2 el-icon--left"></i>上传
        </el-button>
        <el-button class="top-button" size="medium" @click="handleCreateFolder"><i
                class="el-icon-folder-add el-icon--left"></i>新建文件夹
        </el-button>
        <div class="center-items url-paths">
            <div v-for="(item,index) in pathList" :key="index" class="center-items">
                <el-link @click="handleSearchFolder(index)">{{item}}</el-link>
                <el-divider direction="vertical" v-if="index+1!==pathList.length"></el-divider>
            </div>
        </div>

        <el-table
                height="80%"
                :data="fileList"
                style="width: 100%;">
            <el-table-column label="名称" header-align="left" fit>
                <template slot-scope="scope">
                    <div class="center-items">
                        <!--首先是头像显示：1是文件夹 其他情况是文件-->
                        <el-avatar v-if="scope.row.type === 1" shape="square" src="/src/assets/folder.png" :size="30"
                                   style="background: none"></el-avatar>
                        <el-avatar v-else shape="square" src="/src/assets/file.png" :size="30"
                                   style="background: none"></el-avatar>

                        <!--文件夹名称的显示-->
                        <el-button v-if="scope.row.type === 1 && scope.row.optType === 0"
                                   @click="enterFolder(scope.$index, scope.row)"
                                   type="text"
                                   size="small" style="margin-left: 5px;font-size:14px;">
                            {{ scope.row.name }}
                        </el-button>
                        <!--文件名称的显示-->
                        <span v-if="scope.row.type === 0 && scope.row.optType === 0" style="margin-left: 5px;flex: 1">
                            {{ scope.row.name }}
                        </span>
                        <!--新增模式-->
                        <el-input v-if="scope.row.optType === 1" class="name-input" v-model="currentNameToCreate"
                                  placeholder="请输入文件夹名称"></el-input>
                        <el-button v-if="scope.row.optType === 1" icon="el-icon-check" @click="doCreateFolder"
                                   class="after-input-button"></el-button>
                        <el-button v-if="scope.row.optType === 1" icon="el-icon-close" @click="closeCreate"
                                   class="after-input-button"></el-button>

                        <el-input v-if="scope.row.optType === 2" class="name-input" v-model="currentNameToModify"
                                  placeholder="请重新输入名称"></el-input>
                        <el-button v-if="scope.row.optType === 2" icon="el-icon-check"
                                   @click="doRename(scope.$index, scope.row)"
                                   class="after-input-button"></el-button>
                        <el-button v-if="scope.row.optType === 2" icon="el-icon-close"
                                   @click="closeRename(scope.$index, scope.row)"
                                   class="after-input-button"></el-button>
                    </div>
                </template>
            </el-table-column>
            <el-table-column label="大小" width="120" header-align="center" align="center">
                <template slot-scope="scope">
                    <span>{{ scope.row.size }}</span>
                </template>
            </el-table-column>
            <el-table-column label="修改日期" header-align="center" width="160" align="center">
                <template slot-scope="scope">
                    <span>{{ scope.row.lastModifyTime }}</span>
                </template>
            </el-table-column>
            <el-table-column label="操作" width="300" header-align="center" align="center">
                <template slot-scope="scope">
                    <el-button
                            class="operator-button"
                            size="mini"
                            type="primary"
                            v-if="scope.row.optType === 0"
                            width="120"
                            @click="handleRename(scope.$index, scope.row)">重命名
                    </el-button>
                    <el-button
                            class="operator-button"
                            size="mini"
                            type="danger"
                            v-if="scope.row.optType === 0"
                            @click="handleDelete(scope.$index, scope.row)">删除
                    </el-button>
                    <el-button
                            class="operator-button"
                            size="mini"
                            type="success"
                            v-if="scope.row.optType === 0 && scope.row.type === 0"
                            @click="handleDownload(scope.$index, scope.row)">下载
                    </el-button>
                </template>
            </el-table-column>
        </el-table>
    </div>
</template>

<script>
    module.exports = {
        data() {
            return {
                pathList: ['全部文件'],
                fileList: [],
                currentNameToCreate: "",
                currentNameToModify: "",
                loading: false
            }
        },
        methods: {
            handleRename(index, row) {
                console.log(index, row);
                row['optType'] = 2;//编辑是2
                this.currentNameToModify = row.name
            },
            doRename(index, row) {
                axios.post(`${window.$config.addr}/rename`, {
                    'path': this.getCurrentPathFromPathList(),
                    'srcName': row.name,
                    'targetName': this.currentNameToModify
                }).then(res => {
                    if (res.data.code === 0) {
                        this.$message({
                            message: '成功',
                            type: 'success'
                        });
                    }
                    this.currentNameToModify = ""
                    this.fetchData(this.getCurrentPathFromPathList());
                })
            },
            closeRename(index, row) {
                row['optType'] = 0;
                this.currentNameToModify = ""
            },
            handleCreateFolder() {
                let path = this.getCurrentPathFromPathList();
                this.fileList.splice(0, 0, {
                    'optType': 1,//新增是1
                    'type': 1//文件夹类型是1
                })
                this.folderName = ""
            },
            doCreateFolder() {
                axios.post(`${window.$config.addr}/createFolder`, {
                    'path': this.getCurrentPathFromPathList(),
                    'name': this.currentNameToCreate
                }).then(res => {
                    if (res.data.code === 0) {
                        this.$message({
                            message: '成功',
                            type: 'success'
                        });
                    }
                    this.currentNameToCreate = ""
                    this.fetchData(this.getCurrentPathFromPathList());
                })
            },
            closeCreate() {
                //移掉添加的那一行
                this.fileList.splice(0, 1);
                this.currentNameToCreate = ''
            },
            handleUpload() {
                //上传文件
                const inputFile = this.$refs.fileSelector.files[0];
                var forms = new FormData()
                forms.append('file', inputFile)
                this.loading = true
                axios.post(`${window.$config.addr}/upload?path=${this.getCurrentPathFromPathList()}`, forms).then(res => {
                    this.loading = false;//取消加载
                    this.$message({
                        message: '上传成功',
                        type: 'success'
                    });
                    this.fetchData(this.getCurrentPathFromPathList());
                })
            },
            getCurrentPathFromPathList() {
                return this.pathList.length === 1 ? '/' : '/' + this.pathList.slice(1).join('/');
            },
            clickSelectFile() {
                this.$refs.fileSelector.dispatchEvent(new MouseEvent('click'));//触发input框的点击事件
            },
            enterFolder(index, row) {
                let path = row.path === '/' ? '/' + row.name : row.path + '/' + row.name;
                this.fetchData(path);
                this.pathList.push(row.name);
            },
            fetchData(path) {
                axios.get(`${window.$config.addr}/getFolder?path=${path}`).then((res) => {
                    this.fileList = []
                    let folders = res.data.folderList;
                    let files = res.data.fileList;
                    for (let i = 0; i < folders.length; i++) {
                        this.fileList.push({
                            'path': folders[i].path,
                            'name': folders[i].name,
                            'size': folders[i].size,
                            'type': 1,//文件夹
                            'optType': 0
                        });
                    }
                    for (let i = 0; i < files.length; i++) {
                        files[i].type = 0;//普通文件
                        files[i].optType = 0;
                        this.fileList.push(files[i]);
                    }
                })
            },
            handleSearchFolder(index) {
                console.log(index)
                if (index === 0) {
                    this.fetchData('/');
                } else {
                    this.fetchData('/' + this.pathList.slice(1, index + 1).join('/'))
                }
                this.pathList = this.pathList.slice(0, index + 1)
            },
            handleDelete(index, row) {
                console.log(index, row);
                axios.get(`${window.$config.addr}/del?path=${row.path === '/' ? '/' + row.name : row.path + "/" + row.name}`).then((res) => {
                    if (res.data.code === 0) {
                        this.$message({
                            message: '成功',
                            type: 'success'
                        });
                    } else {
                        this.$message({
                            message: res.data.msg,
                            type: 'error'
                        });
                    }
                    this.fetchData(this.getCurrentPathFromPathList());
                })

            },
            handleDownload(index, row) {
                console.log(index, row);
                axios.get(`${window.$config.addr}/download?path=${row.path === '/' ? '/' + row.name : row.path + "/" + row.name}`, {responseType: 'blob'}).then((res) => {
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
        },
        mounted() {
            this.fetchData('/');
        }
    }
</script>
<style scoped>
    .url-paths {
        margin-bottom: 5px;
        margin-top: 5px;
        margin-left: 10px;
    }

    .upload-button {
        margin-left: 10px;
    }

    .top-button {
        margin-top: 10px;
    }

    .name-input {
        margin-left: 5px;
        flex: 1;
    }

    .operator-button {
        margin-left: unset;
    }

    .after-input-button {
        height: 30px;
        width: 30px;
        padding: unset;
        margin-left: 0;
    }

    .el-input__inner {
        height: 30px;
        line-height: 30px;
    }

    input[type=file] {
        display: none;
    }

    .file-wrapper {
        height: 100%;
    }

    .center-items {
        display: flex;
        align-items: center;
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