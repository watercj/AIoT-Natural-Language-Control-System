// import { defineConfig } from 'vite'
// import vue from '@vitejs/plugin-vue'

// // https://vitejs.dev/config/
// export default defineConfig({
//   plugins: [vue()],
//   base: 'assets', 
// })


import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
// import { visualizer } from "rollup-plugin-visualizer";
import path from "path";

// https://vitejs.dev/config/
export default defineConfig({
  server: {
    host: 'localhost',
    port: 8080,
  },
  // plugins: [vue(), visualizer()],
  plugins: [vue()],
  base: "./", // index.html 中靜態檔案路徑前綴
  resolve: {
    alias: {
      "@": path.resolve(__dirname, "./src"),
    },
  },
  build: {
    // 靜態檔案打包後資料夾名稱
    assetsDir: "static",
    rollupOptions: {
      input: {
        index: path.resolve(__dirname, "index.html"),
      },
      output: {
        // 靜態檔案打包後路徑配置
        chunkFileNames: "static/js/[name]-[hash].js",
        entryFileNames: "static/js/[name]-[hash].js",
        assetFileNames: "static/[ext]/name-[hash].[ext]",
      },
    },
  },
});
