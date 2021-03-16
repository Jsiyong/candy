<template>
    <splitpanes class="default-theme">
        <pane min-size="20" max-size="20" size="20">
            <com-menu></com-menu>
            <!--            <folder-constructor class="left"-->
            <!--                                :change-selected-node="changeSelectedNode"-->
            <!--                                :toggle-constructor="toggleConstructor"-->
            <!--                                :constructor-show="constructorShow"></folder-constructor>-->
        </pane>
        <pane>
            <file-content :file-list="fileList"></file-content>
        </pane>
    </splitpanes>
</template>

<script scoped>
    const {Splitpanes, Pane} = splitpanes

    module.exports = {
        name: 'UpDown',
        components: {
            'file-content': httpVueLoader('../components/updown/FileContent.component.vue'),
            // 'folder-constructor': httpVueLoader('../components/updown/FolderConstructor.component.vue'),
            'com-menu': httpVueLoader('../components/common/ComMenu.component.vue'),
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

    .el-breadcrumb {
        font-size: unset;
        line-height: unset;
        margin-left: 10px;
    }
</style>