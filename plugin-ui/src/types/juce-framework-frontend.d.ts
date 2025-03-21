declare module "juce-framework-frontend" {
  export function getSliderState(name: string): SliderState;
  export function getToggleState(name: string): ToggleState;
  export function getComboBoxState(name: string): ComboBoxState;
  export function getNativeFunction(name: string): (...args: any[]) => Promise<any>;
  export function getBackendResourceAddress(path: string): string;

  export class ControlParameterIndexUpdater {
    constructor(attribute: string);
    handleMouseMove(event: MouseEvent): void;
  }

  interface SliderState {
    setNormalisedValue(value: number): void;
    getNormalisedValue(): number;
    getScaledValue(): number;
    sliderDragStarted(): void;
    sliderDragEnded(): void;
    valueChangedEvent: ListenerList;
    propertiesChangedEvent: ListenerList;
    properties: {
      start: number;
      end: number;
      skew: number;
      name: string;
      label: string;
      numSteps: number;
      interval: number;
      parameterIndex: number;
    };
  }

  interface ToggleState {
    setValue(value: boolean): void;
    getValue(): boolean;
    valueChangedEvent: ListenerList;
    propertiesChangedEvent: ListenerList;
    properties: {
      name: string;
      parameterIndex: number;
    };
  }

  interface ComboBoxState {
    setChoiceIndex(index: number): void;
    getChoiceIndex(): number;
    valueChangedEvent: ListenerList;
    propertiesChangedEvent: ListenerList;
    properties: {
      name: string;
      parameterIndex: number;
      choices: string[];
    };
  }

  interface ListenerList {
    addListener(fn: () => void): number;
    removeListener(id: number): void;
  }
}
