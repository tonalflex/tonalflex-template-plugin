import { inject } from "vue";
import type { IAudioBackend } from "@backend/IAudioBackend";

let backend: IAudioBackend | undefined;

export function useAudioBackend(): IAudioBackend {
  if (!backend) {
    backend = inject<IAudioBackend>("audio-backend");
    if (!backend) {
      throw new Error("No audio backend provided");
    }
  }
  return backend;
}
