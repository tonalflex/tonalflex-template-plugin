<template>
  <div class="juce-slider">
    <span class="text-sm font-medium text-center w-full">
      {{ identifier }}: {{ paramValue.toFixed(3) }}
    </span>
    <input
      type="range"
      :min="0"
      :max="1"
      :step="0.001"
      v-model="paramValue"
      @input="updateJUCE"
      class="w-48"
    />
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from "vue";
import * as Juce from "juce-framework-frontend";

// defineProps<{ identifier: string }>();
const identifier = "roomSize";

const paramValue = ref(0);
let sliderState: any = null;

onMounted(() => {
  sliderState = Juce.getSliderState(identifier);
  // Set the initial value
  paramValue.value = sliderState.getNormalisedValue();
  // Listen for JUCE backend updates
  sliderState.valueChangedEvent.addListener(() => {
    console.log("Value changed", sliderState.getNormalisedValue());
    paramValue.value = sliderState.getNormalisedValue();
  });
});

// Update JUCE when slider is moved
const updateJUCE = () => {
  sliderState.setNormalisedValue(paramValue.value);
};
</script>

<style scoped>
.juce-slider {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10px;
}
</style>
