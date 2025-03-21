// editor.h
#pragma once

#include "plugin_processor.h"
#include <juce_gui_extra/juce_gui_extra.h>  // needed for WebBrowserComponent

namespace audio_plugin {
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       public juce::AudioProcessorParameter::Listener {
public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

private:
    juce::WebBrowserComponent webView;
    AudioPluginAudioProcessor& processorRef;

    juce::Label headlineLabel, roomSizeLabel, dampingLabel, wetLevelLabel, dryLevelLabel;
    juce::Slider roomSizeSlider, dampingSlider, wetLevelSlider, dryLevelSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment, dampingAttachment,
        wetLevelAttachment, dryLevelAttachment;

    void handleParameterChange(const juce::String& paramName, float value);
    void sendParameterToVue(const juce::String& paramName, float value);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin