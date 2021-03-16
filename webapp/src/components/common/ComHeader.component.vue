<template>
    <div class="header">
        <div class="block system-name">
            <el-avatar shape="square" :size="50" src="/src/assets/logo.png"></el-avatar>
            <div>Candy</div>
        </div>
    </div>
</template>

<script>
    module.exports = {
        name: 'ComHeader',
        data() {
            return {}
        }
    }
</script>
<style scoped>
    .header {
        color: rgb(245 255 255);
        height: 60px;
        border-bottom: 1px solid;
        box-shadow: 0 2px 6px 0 rgba(0, 0, 0, .05);
        position: relative;
        z-index: 1;
    }


    .system-name {
        height: 100%;
        width: 400px;
        position: absolute;
        left: 2%;
        font-size: 30px;
        font-weight: bold;
        display: flex;
        align-items: center;
        justify-content: left;
        color: #0c0c0f;
    }

    .el-avatar > img {
        background: white;
    }
</style>