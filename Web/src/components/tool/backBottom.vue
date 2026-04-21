<template>
  <div class="back-bottom" @click="$emit('click')">
    <el-icon><ArrowDown /></el-icon>
  </div>
</template>

<script>
export default {
  name: 'BackBottom'
};
</script>

<style>
.back-bottom {
  position: fixed;
  bottom: 65px;
  right: calc(50% - 15px);
  background-color: #00000030;
  color: #fff;
  width: 30px;
  height: 30px;
  border-radius: 50%;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
}
</style>
