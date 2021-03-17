<template>
    <el-menu
            default-active="1"
            class="my-menu el-menu-vertical-demo">
        <el-menu-item index="1">
            <i class="el-icon-document-copy"></i>
            <span slot="title">全部文件</span>
        </el-menu-item>
        <el-menu-item index="2" disabled>
            <i class="el-icon-document"></i>
            <span slot="title">系统信息</span>
        </el-menu-item>
        <el-menu-item index="3" disabled>
            <i class="el-icon-setting"></i>
            <span slot="title">个人信息</span>
        </el-menu-item>
    </el-menu>
</template>

<script>
    module.exports = {
        name: 'ComMenu'
    }
</script>
<style scoped>

    .my-menu {
        height: 100%;
        background: #f7f7f7;
        font-weight: bold;
    }

    .my-menu .el-menu-item:hover {
        background: #d5d3d3;
    }
</style>