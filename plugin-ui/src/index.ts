// Export main component
export { default as Plugin } from './components/Plugin.vue';

// Export Audio Backend interface
export type * from './backend/IAudioBackend';

// Export Mock Backend (For implementation testing)
export { MockBackend } from './backend/MockBackend';
