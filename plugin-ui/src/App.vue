<script setup lang="ts">
import Plugin from "./components/Plugin.vue";
/**
 * This is the local development shell for the Plugin UI.
 *
 * ⚠️ Note: This file is NOT included in the final npm build.
 * It is only used by main.ts to preview and test the Plugin component.
 *
 * Any UI changes made here (e.g., styling, layout, logic) will NOT
 * affect the actual plugin code that is distributed via npm.
 *
 * During development, `main.ts` mounts this component (App.vue) to the DOM
 * so you can run and test the plugin UI in isolation (via `vite dev`).
 *
 * In contrast, `index.ts` is the actual entry point for the npm package.
 * It should export only the components and interfaces that are meant to be reused.
 */
</script>

<template>
  <Plugin />
</template>
