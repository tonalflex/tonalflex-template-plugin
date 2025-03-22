<template>
  <div class="flex flex-col gap-6 items-center">
    <SliderControl label="Room Size" v-model="roomSize" />
    <SliderControl label="Damping" v-model="damping" />
    <SliderControl label="Wet Level" v-model="wetLevel" />
    <SliderControl label="Dry Level" v-model="dryLevel" />
  </div>
</template>

<script setup lang="ts">
import { inject } from "vue";
import type { IAudioBackend } from "../backend/IAudioBackend";
import SliderControl from "./SliderControl.vue";
import { useSliderParameter } from "../composables/useSliderParameter";

// Inject audio backend specified in `index.ts` or `main.ts`
const backend = inject<IAudioBackend>("audio-backend");
if (!backend) throw new Error("No audio backend provided");

// Plugin Parameters
const roomSize = useSliderParameter("roomSize", backend);
const damping = useSliderParameter("damping", backend);
const wetLevel = useSliderParameter("wetLevel", backend);
const dryLevel = useSliderParameter("dryLevel", backend);
</script>
