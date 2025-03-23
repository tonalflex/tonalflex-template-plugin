// composables/useSliderParameter.ts
import { ref, onMounted, watch } from "vue";
import { useAudioBackend } from "./useAudioBackend";
import type { SliderParameter } from "../backend/IAudioBackend";

export function useSliderParameter(identifier: string) {
  const backend = useAudioBackend();
  const value = ref<number>(0);
  let state: SliderParameter;

  onMounted(() => {
    state = backend.getParameterState(identifier, "slider");

    // Initialize 
    value.value = state.getValue();

    // Backend → UI
    state.valueChangedEvent?.addListener(() => {
      value.value = state.getValue();
    });
  
    // UI → Backend
    watch(value, (newVal) => {
      state.setValue(newVal);
    });
  });

  return value;
}
