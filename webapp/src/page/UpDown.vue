<template>
    <div class="up-down-wrapper">
        <div class="url-path-wrapper">
            <el-button type="success" icon="el-icon-back" size="mini" circle></el-button>
            <el-button type="info" icon="el-icon-right" size="mini" circle></el-button>
            <div class="url-path-inner">
                <el-breadcrumb separator="/">
                    <el-breadcrumb-item>
                        <el-link>首页</el-link>
                    </el-breadcrumb-item>
                    <el-breadcrumb-item>
                        <el-link>活动管理</el-link>
                    </el-breadcrumb-item>
                    <el-breadcrumb-item>
                        <el-link>活动列表</el-link>
                    </el-breadcrumb-item>
                    <el-breadcrumb-item>
                        <el-link>活动详情</el-link>
                    </el-breadcrumb-item>
                </el-breadcrumb>
            </div>
            <el-button type="primary" icon="el-icon-upload2" circle size="mini"></el-button>
        </div>
        <splitpanes class="default-theme" class="up-down" :class="{'constructor-hidden':!constructorShow}">
            <pane>
                <file-constructor class="left"
                                  :change-selected-node="changeSelectedNode"
                                  :toggle-constructor="toggleConstructor"
                                  :constructor-show="constructorShow"></file-constructor>
            </pane>
            <pane>
                <folder-content class="right" :file-list="fileList"></folder-content>
            </pane>

        </splitpanes>
    </div>
</template>

<script scoped>
    const {Splitpanes, Pane} = splitpanes

    module.exports = {
        name: 'UpDown',
        components: {
            'file-constructor': httpVueLoader('../components/updown/FileConstructor.component.vue'),
            'folder-content': httpVueLoader('../components/updown/FolderContent.component.vue'),
            Splitpanes, Pane
        },
        data() {
            return {
                fileList: [],//选中节点
                constructorShow: true,//是否隐藏左边列表
                currentRoute: window.location.pathname
            }
        },
        methods: {
            initData() {
                let search = window.location.search;
            },
            //点击选中节点
            changeSelectedNode(node) {
                this.fileList = node.data.fileList;
                // window.location.href = "";
            },
            //显示隐藏左边列表
            toggleConstructor() {
                this.constructorShow = !this.constructorShow;
            }
        }
    }
</script>

<style scoped>
    .up-down-wrapper {
        height: calc(100% - 61px);
    }

    .url-path-wrapper {
        pointer-events: all;
        display: flex;
        align-items: center;
    }

    .url-path-inner {
        background: #fff;
        border-radius: 4px;
        padding-top: 10px;
        padding-bottom: 10px;
        text-align: left;
        width: 90%;
        border: 1px solid transparent;
        transition: border-color .2s cubic-bezier(.645, .045, .355, 1);
        cursor: text;
    }

    .url-path-inner:hover {
        border: 1px solid #d0cfcf;
        transition: border-color .2s cubic-bezier(.645, .045, .355, 1);
    }

    .up-down {
        height: 100%;
        display: flex;
    }

    .up-down .left {
        width: 20%;
        transition: all 0.5s;
    }

    .up-down .right {
        width: 80%;
        transition: all 0.5s;
    }

    .up-down.constructor-hidden .left {
        width: 0;
    }

    .up-down.constructor-hidden .right {
        width: 100%;
    }

    ::-webkit-scrollbar {
        width: 6px !important;
        background: transparent !important;
    }

    ::-webkit-scrollbar-thumb {
        border-radius: 10px !important;
        background: #6d986a !important;
    }

    ::-webkit-scrollbar-track {
        background: transparent !important;
    }

    .splitpanes.default-theme .splitpanes__pane {
        background-color: transparent;
    }

    .splitpanes__splitter {
        pointer-events: all;
    }

    .splitpanes {
        height: calc(100% - 61px);
    }

    .el-button + .el-button {
        margin-left: unset;
    }

    .el-breadcrumb {
        font-size: unset;
        line-height: unset;
        margin-left: 10px;
    }
</style>