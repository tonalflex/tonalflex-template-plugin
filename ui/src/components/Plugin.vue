<template>
  <div class="flex flex-col gap-6 items-center">
    <SliderControl label="Room Size" v-model="roomSize" />
    <SliderControl label="Damping" v-model="damping" />
    <SliderControl label="Wet Level" v-model="wetLevel" />
    <SliderControl label="Dry Level" v-model="dryLevel" />

    <button @click="handleClick" style="margin-top: 20px">Call Function</button>

    <p v-if="loading">Calling native function…</p>
    <p v-if="result">{{ result }}</p>
    <p v-if="error">{{ error }}</p>
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
const { result, loading, error, invoke } = useFunction("exampleNativeFunction");

const handleClick = () => {
  // Pass args to the native function
  invoke(roomSize.value, damping.value, wetLevel.value, dryLevel.value);
};
</script>
