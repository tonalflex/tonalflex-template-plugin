// composables/useParameter.ts
import { ref, watch } from "vue";
import { useAudioBackend } from "@composables/useAudioBackend";
import type {
  ParameterType,
  ParameterValueType,
  SliderParameter,
  ToggleParameter,
  ComboBoxParameter,
} from "@backend/IAudioBackend";

export function useParameter<T extends ParameterType>(
  identifier: string,
  type: T
) {
  const backend = useAudioBackend();
  const state = backend.getParameterState(identifier, type);

  const value = ref<ParameterValueType[T]>(undefined as unknown as ParameterValueType[T]);

  if (type === "slider") {
    const slider = state as SliderParameter;
    value.value = slider.getValue() as ParameterValueType[T];
    slider.valueChangedEvent?.addListener(() => {
      value.value = slider.getValue() as ParameterValueType[T];
    });
    watch(value, (newVal) => {
      slider.setValue(newVal as number);
    });
  }

  else if (type === "toggle") {
    const toggle = state as ToggleParameter;
    value.value = toggle.getValue() as ParameterValueType[T];
    toggle.valueChangedEvent?.addListener(() => {
      value.value = toggle.getValue() as ParameterValueType[T];
    });
    watch(value, (newVal) => {
      toggle.setValue(newVal as boolean);
    });
  }

  else if (type === "comboBox") {
    const combo = state as ComboBoxParameter;
    value.value = combo.getChoiceIndex() as ParameterValueType[T];
    combo.valueChangedEvent?.addListener(() => {
      value.value = combo.getChoiceIndex() as ParameterValueType[T];
    });
    watch(value, (newVal) => {
      combo.setChoiceIndex(newVal as number);
    });
  }

  return value;
}
