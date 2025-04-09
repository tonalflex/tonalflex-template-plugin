// vite.config.ts
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import tsconfigPaths from 'vite-tsconfig-paths'
import cssInjectedByJsPlugin from 'vite-plugin-css-injected-by-js'

const isLib = process.env.BUILD_TARGET === 'lib'

export default defineConfig({
  plugins: [
    vue(),
    tsconfigPaths(),
    cssInjectedByJsPlugin(),
  ],
  build: isLib
    // Build NPM library
    ? {
        outDir: 'dist',
        emptyOutDir: true,
        lib: {
          entry: new URL('./src/index.ts', import.meta.url).pathname,
          name: 'PluginUI',
          fileName: (format) => `plugin-ui.${format}.js`
        },
        rollupOptions: {
          external: ['vue', 'juce-framework-frontend'],
          output: {
            globals: {
              vue: 'Vue',
              'juce-framework-frontend': 'JuceFrameworkFrontend'
            }
          }
        }
      }
    // Build JUCE Webview static UI
    : {
        outDir: '../plugin/webview',
        emptyOutDir: true,
        assetsDir: 'assets',
        rollupOptions: {
          input: './index.html',
          output: {
            format: 'iife',
            entryFileNames: 'index.js'
          }
        }
      },
  base: './'
})
