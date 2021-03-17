<template>
    <div class="file-wrapper"
         v-loading="loading"
         element-loading-text="上传中">
        <input ref="fileSelector" name="file" type="file" @change="handleUpload">
        <el-button type="primary" @click="clickSelectFile"><i class="el-icon-upload2 el-icon--left"></i>上传</el-button>
        <el-button @click="handleCreateFolder"><i class="el-icon-folder-add el-icon--left"></i>新建文件夹</el-button>
        <div class="url-path">
            <div v-for="(item,index) in pathList" :key="index" class="url-path">
                <el-link @click="handleSearchFolder(index)">{{item}}</el-link>
                <el-divider direction="vertical" v-if="index+1!==pathList.length"></el-divider>
            </div>
        </div>
        <el-table
                height="100%"
                :data="fileList"
                style="width: 100%;">
            <el-table-column label="名称" header-align="center">
                <template slot-scope="scope">
                    <div v-if="scope.row.type === 1">
                        <el-avatar shape="square" src="/src/assets/folder.png" style="background: none"></el-avatar>
                        <el-button
                                @click="enterFolder(scope.$index, scope.row)"
                                type="text"
                                size="small">
                            {{ scope.row.name }}
                        </el-button>
                    </div>
                    <div v-else-if="scope.row.type === 2">
                        <el-avatar shape="square" src="/src/assets/folder.png" style="background: none"></el-avatar>
                        <el-input v-model="folderName" placeholder="请输入文件夹名称"></el-input>
                        <el-button icon="el-icon-check" @click="doCreateFolder"></el-button>
                        <el-button icon="el-icon-close" @click="closeCreate"></el-button>
                    </div>
                    <div v-else>
                        <el-avatar shape="square" src="/src/assets/file.png" style="background: none"></el-avatar>
                        <span>{{ scope.row.name }}</span>
                    </div>
                </template>
            </el-table-column>
            <el-table-column label="大小" width="180" header-align="center" align="center">
                <template slot-scope="scope">
                    <span style="margin-left: 10px">{{ scope.row.size }}</span>
                </template>
            </el-table-column>
            <el-table-column label="修改日期" header-align="center">
                <template slot-scope="scope">
                    <span style="margin-left: 10px">{{ scope.row.lastModifyTime }}</span>
                </template>
            </el-table-column>
            <el-table-column label="操作" width="300" header-align="center" align="center">
                <template slot-scope="scope">
                    <el-button
                            size="mini"
                            type="primary"
                            v-if="scope.row.type !== 2"
                            @click="handleEdit(scope.$index, scope.row)">重命名
                    </el-button>
                    <el-button
                            size="mini"
                            type="danger"
                            v-if="scope.row.type !== 2"
                            @click="handleDelete(scope.$index, scope.row)">删除
                    </el-button>
                    <el-button
                            size="mini"
                            type="success"
                            v-if="!scope.row.type"
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
                folderName: "",
                loading: false
            }
        },
        methods: {
            doCreateFolder() {
                axios.post(`${window.$config.addr}/createFolder`, {
                    'path': this.getCurrentPathFromPathList(),
                    'name': this.folderName
                }).then(res => {
                    this.$message({
                        message: '成功',
                        type: 'success'
                    });
                    this.fetchData(this.getCurrentPathFromPathList());
                })
            },
            closeCreate() {
                this.fileList.splice(0, 1)
            },
            handleCreateFolder() {
                let path = this.getCurrentPathFromPathList();
                this.fileList.splice(0, 0, {
                    'type': 2
                })
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
                            'type': 1
                        });
                    }
                    for (let i = 0; i < files.length; i++) {
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
        },
        mounted() {
            this.fetchData('/');
        }
    }
</script>
<style scoped>
    input[type=file] {
        display: none;
    }

    .file-wrapper {
        height: 100%;
    }

    .url-path {
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