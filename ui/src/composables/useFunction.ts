import { ref } from "vue";
import { useAudioBackend } from "@composables/useAudioBackend";

export function useFunction<T = any>(name: string) {
  const backend = useAudioBackend();
  const result = ref<T | null>(null);
  const loading = ref(false);
  const error = ref<unknown>(null);

  const invoke = async (...args: any[]): Promise<T | null> => {
    loading.value = true;
    error.value = null;

    try {
      const fn = backend.getPluginFunction(name);

      // Timeout after 2 seconds
      const timeoutPromise = new Promise<null>((_, reject) =>
        setTimeout(() => reject(new Error(`No response from ${name}`)), 2000)
      );
  
      const resultPromise = fn(...args);
      const res = await Promise.race([resultPromise, timeoutPromise]) as T;
  
      result.value = res;
      return res;
    } catch (err) {
      console.error(`Error invoking native function "${name}"`, err);
      error.value = err;
      return null;
    } finally {
      loading.value = false;
    }
  };

  return {
    result,
    loading,
    error,
    invoke,
  };
}
