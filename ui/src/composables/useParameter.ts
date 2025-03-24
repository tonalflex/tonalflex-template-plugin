// composables/useParameter.ts
import { ref, watch, onUnmounted, type Ref } from "vue";
import { useAudioBackend } from "@composables/useAudioBackend";
import type {
  ParameterType,
  ParameterMap,
  ParameterValueType,
} from "@backend/IAudioBackend";

type Parameter<T extends ParameterType> = ParameterMap[T];

export function useParameter<T extends ParameterType>(
  identifier: string,
  type: T
): Ref<ParameterValueType[T]> {
  const backend = useAudioBackend();
  const param = backend.getParameterState(identifier, type) as Parameter<T>;

  let get: () => ParameterValueType[T];
  let set: (value: ParameterValueType[T]) => void;

  switch (type) {
    case "slider": {
      const p = param as ParameterMap["slider"];
      get = () => p.getValue() as ParameterValueType[T];
      set = (v) => p.setValue(v as ParameterValueType["slider"]);
      break;
    }
    case "toggle": {
      const p = param as ParameterMap["toggle"];
      get = () => p.getValue() as ParameterValueType[T];
      set = (v) => p.setValue(v as ParameterValueType["toggle"]);
      break;
    }
    case "comboBox": {
      const p = param as ParameterMap["comboBox"];
      get = () => p.getChoiceIndex() as ParameterValueType[T];
      set = (v) => p.setChoiceIndex(v as ParameterValueType["comboBox"]);
      break;
    }
    default:
      throw new Error(`Unsupported parameter type: ${type}`);
  }

  const value = ref(get()) as Ref<ParameterValueType[T]>;

  // Update Backend => UI
  const id = param.valueChangedEvent?.addListener(v => {
    value.value = v as ParameterValueType[T];
  });
  
  if (id !== undefined) {
    onUnmounted(() => param.valueChangedEvent?.removeListener(id));
  }

  // Update UI => Backend
  let debounce: ReturnType<typeof setTimeout> | null = null;

  watch(value, (val, oldVal) => {
    if (val === oldVal) return;
    if (debounce) clearTimeout(debounce);
    debounce = setTimeout(() => {
      set(val);
    }, 10); // debounce time in ms
  });

  return value;
}
