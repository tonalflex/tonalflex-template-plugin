<template>
  <div class="juce-slider">
    <span class="text-sm font-medium text-center w-full">
      {{ label }}: {{ modelValue.toFixed(2) }}
    </span>
    <input
      type="range"
      :min="0"
      :max="1"
      :step="0.01"
      :value="modelValue"
      @input="onInput"
      class="w-48"
    />
  </div>
</template>

<script setup lang="ts">
defineProps<{
  label: string;
  modelValue: number;
}>();

const emit = defineEmits<{
  (e: "update:modelValue", value: number): void;
}>();

function onInput(e: Event) {
  const target = e.target as HTMLInputElement;
  emit("update:modelValue", parseFloat(target.value));
}
</script>

<style scoped>
.juce-slider {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10px;
}
</style>
