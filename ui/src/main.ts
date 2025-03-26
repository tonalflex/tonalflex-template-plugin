import { createApp } from 'vue'
import './style.css'
import App from './App.vue'
import { JuceBackend } from './backend/JuceBackend'
import type { IAudioBackend } from './backend/IAudioBackend'

const app = createApp(App)

// Mount the JUCE backend for desktop GUI build
const backend: IAudioBackend = new JuceBackend()
app.provide('audio-backend', backend)

app.mount('#app')
