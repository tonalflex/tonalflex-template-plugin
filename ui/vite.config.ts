import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import tsconfigPaths from 'vite-tsconfig-paths'
import cssInjectedByJsPlugin from 'vite-plugin-css-injected-by-js';

// https://vite.dev/config/
export default defineConfig({
  plugins: [
    vue(), 
    tsconfigPaths(),          // Allow @ import aliases
    cssInjectedByJsPlugin(), // Inject CSS compiled with JS
  ],
  build: {
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
})
