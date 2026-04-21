<template>
  <div class="loading-dot">
    <span></span>
    <span></span>
    <span></span>
  </div>
</template>

<style scoped>
.loading-dot {
  display: flex;
  margin: 0 0 6px 5px;
}
.loading-dot span {
    display: block;
    width: 10px;
    height: 10px;
    background-color: #f9f9f9;
    border-radius: 50%;
    opacity: 0;
    animation: loadingAnimation .9s infinite;
}

.loading-dot span:nth-child(1) {
    animation-delay: 0s;
    margin-right: 5px;
}

.loading-dot span:nth-child(2) {
    animation-delay: .2s;
    margin-right: 5px;
}

.loading-dot span:nth-child(3) {
    animation-delay: .3s;
}

@keyframes loadingAnimation {
    0%, 100% {
        opacity: 0;
    }
    50% {
        opacity: 1;
    }
}
</style>