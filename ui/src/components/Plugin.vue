<template>
  <div class="container">
    <img src="@/assets/logo.png" alt="Logo" class="logo" />
    <SliderControl label="Room Size" v-model="roomSize" />
    <SliderControl label="Damping" v-model="damping" />
    <SliderControl label="Wet Level" v-model="wetLevel" />
    <SliderControl label="Dry Level" v-model="dryLevel" />

    <button @click="handleClick" style="margin-top: 20px">Call Function</button>

    <p v-if="pluginFunction.loading.value">Calling plugin function…</p>
    <p v-if="pluginFunction.result.value">{{ pluginFunction.result }}</p>
    <p v-if="pluginFunction.error.value">{{ pluginFunction.error }}</p>
  </div>
</template>

<script setup lang="ts">
import SliderControl from "@components/controls/SliderControl.vue";
import { useParameter } from "@composables/useParameter";
import { useFunction } from "@composables/useFunction";

// Plugin Parameters
const roomSize = useParameter("roomSize", "slider");
const damping = useParameter("damping", "slider");
const wetLevel = useParameter("wetLevel", "slider");
const dryLevel = useParameter("dryLevel", "slider");

// Plugin Native Functions
const pluginFunction = useFunction("exampleNativeFunction");

const handleClick = () => {
  // Pass param vaules as args to the native function
  pluginFunction.invoke(
    roomSize.value,
    damping.value,
    wetLevel.value,
    dryLevel.value
  );
};
</script>

<style scoped>
.logo {
  width: 50vw;
  max-width: 400px;
  height: auto;
}

.container {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
  align-items: center;
}
</style>
