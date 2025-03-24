# TonalFlex Plugin UI

## Setup Backend

In src/main.ts, you can test the plugin using the shipped mock backend implementation.

The backend of your choice should implement the IAudioBackend interface in order to work with the plugin ui.

**Vue.js example:**

```ts
import { createApp } from "vue";
import "./style.css";
import App from "./App.vue";
import type { IAudioBackend } from "@tonalflex/template-plugin";
import { MockBackend } from "@tonalflex/template-plugin";

const app = createApp(App);

// Mount your backend that implements IAudioBackend interface
const backend: IAudioBackend = new MockBackend();
app.provide("audio-backend", backend);

app.mount("#app");
```
