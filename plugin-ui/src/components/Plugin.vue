<template>
  <div class="flex flex-col gap-6 items-center">
    <SliderControl label="Room Size" v-model="roomSize" />
    <SliderControl label="Damping" v-model="damping" />
    <SliderControl label="Wet Level" v-model="wetLevel" />
    <SliderControl label="Dry Level" v-model="dryLevel" />
  </div>
</template>

<script setup lang="ts">
import SliderControl from "@components/controls/SliderControl.vue";
import { useParameter } from "@composables/useParameter";

const roomSize = useParameter("roomSize", "slider");
const damping = useParameter("damping", "slider");
const wetLevel = useParameter("wetLevel", "slider");
const dryLevel = useParameter("dryLevel", "slider");
</script>
