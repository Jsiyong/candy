<template>
  <div class="header">
    <div class="system-name">文件糖糖biu~</div>
  </div>
</template>

<script>
module.exports =  {
  name: 'ComHeader'
}
</script>
<style>
  .header {
    background:#95B2B1;
    height: 60px;
    border-bottom: 1px solid;
    border-image: linear-gradient(to right,transparent, #7aada1,transparent) 1;
    box-shadow: 0 1px 9px rgba(61, 89, 83, 0.48);
    position: relative;
    pointer-events: all;
  }
  .system-name {
    height: 100%;
    width: 400px;
    position: absolute;
    left: 50%;
    transform: translate(-50%);
    font-size: 30px;
    font-weight: bold;
    display: flex;
    align-items: center;
    justify-content: center;
  }
</style>