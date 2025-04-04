#pragma once

#include "plugin_processor.h"
#include <juce_gui_extra/juce_gui_extra.h>  // For WebBrowserComponent (used for WebView)
#include "BinaryData.h"                     // Contains embedded Web UI assets (HTML/CSS/JS)

// Restricts WebView loading to internal resources only
struct SinglePageBrowser : juce::WebBrowserComponent {
  using WebBrowserComponent::WebBrowserComponent;
  bool pageAboutToLoad(const juce::String& newURL) override {
    return newURL == juce::String("http://localhost:5173/") || newURL == getResourceProviderRoot();
  }
};

namespace audio_plugin {
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

  int getControlParameterIndex(juce::Component&) override {
    return controlParameterIndexReceiver.getControlParameterIndex();
  }

private:
  AudioPluginAudioProcessor& processorRef;

  //==============================================================================
  // WebView UI
  //==============================================================================
  std::unique_ptr<juce::WebBrowserComponent> webView;

  juce::WebControlParameterIndexReceiver controlParameterIndexReceiver;

  juce::WebSliderRelay roomSizeRelay{"roomSize"};
  juce::WebSliderRelay dampingRelay{"damping"};
  juce::WebSliderRelay wetLevelRelay{"wetLevel"};
  juce::WebSliderRelay dryLevelRelay{"dryLevel"};

  juce::WebSliderParameterAttachment roomSizeWebAttachment{
      *processorRef.parameters.getParameter("roomSize"), roomSizeRelay, nullptr};
  juce::WebSliderParameterAttachment dampingWebAttachment{
      *processorRef.parameters.getParameter("damping"), dampingRelay, nullptr};
  juce::WebSliderParameterAttachment wetLevelWebAttachment{
      *processorRef.parameters.getParameter("wetLevel"), wetLevelRelay, nullptr};
  juce::WebSliderParameterAttachment dryLevelWebAttachment{
      *processorRef.parameters.getParameter("dryLevel"), dryLevelRelay, nullptr};

  std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String& url);
  juce::String getMimeForExtension(const juce::String& extension);

  //==============================================================================
  // Native JUCE UI
  //==============================================================================
  juce::Label headlineLabel, roomSizeLabel, dampingLabel, wetLevelLabel, dryLevelLabel;
  juce::Slider roomSizeSlider, dampingSlider, wetLevelSlider, dryLevelSlider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment,
      dampingAttachment, wetLevelAttachment, dryLevelAttachment;

  //==============================================================================

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
