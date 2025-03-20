#pragma once

#include "plugin_processor.h"

namespace audio_plugin {
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  juce::Label headlineLabel, roomSizeLabel, dampingLabel, wetLevelLabel, dryLevelLabel;
  juce::Slider roomSizeSlider, dampingSlider, wetLevelSlider, dryLevelSlider;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment, dampingAttachment,
      wetLevelAttachment, dryLevelAttachment;

  AudioPluginAudioProcessor& processorRef;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
