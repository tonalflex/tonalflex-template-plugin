#pragma once

#include "plugin_processor.h"
#include <juce_gui_extra/juce_gui_extra.h>  // needed for WebBrowserComponent
#include "BinaryData.h"                     // needed for assets_zip

struct SinglePageBrowser : juce::WebBrowserComponent {
  using WebBrowserComponent::WebBrowserComponent;

  // Prevent page loads from navigating away from our single page web app
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

  juce::Label headlineLabel, roomSizeLabel, dampingLabel, wetLevelLabel, dryLevelLabel;
  juce::Slider roomSizeSlider, dampingSlider, wetLevelSlider, dryLevelSlider;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment,
      dampingAttachment, wetLevelAttachment, dryLevelAttachment;

  //==============================================================================

  juce::WebControlParameterIndexReceiver controlParameterIndexReceiver;

  juce::WebSliderRelay roomSizeRelay{"roomSize"};
  juce::WebSliderRelay dampingRelay{"damping"};
  juce::WebSliderRelay wetLevelRelay{"wetLevel"};
  juce::WebSliderRelay dryLevelRelay{"dryLevel"};

  juce::WebBrowserComponent webView{
      juce::WebBrowserComponent::Options{}
          .withUserScript(R"(console.log("JUCE C++ Backend is running!");)")
          .withNativeIntegrationEnabled()
          .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
          .withWinWebView2Options(
              juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(
                  juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)))
          .withOptionsFrom(roomSizeRelay)
          .withOptionsFrom(dampingRelay)
          .withOptionsFrom(wetLevelRelay)
          .withOptionsFrom(dryLevelRelay)
          .withOptionsFrom(controlParameterIndexReceiver)
          .withResourceProvider([this](const auto& url) { return getResource(url); },
                                juce::URL{"http://localhost:5173/"}.getOrigin())};

  std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String& url);

  const char* getMimeForExtension(const juce::String& extension);

  juce::WebSliderParameterAttachment roomSizeWebAttachment{
      *processorRef.parameters.getParameter("roomSize"), roomSizeRelay, nullptr};
  juce::WebSliderParameterAttachment dampingWebAttachment{
      *processorRef.parameters.getParameter("damping"), dampingRelay, nullptr};
  juce::WebSliderParameterAttachment wetLevelWebAttachment{
      *processorRef.parameters.getParameter("wetLevel"), wetLevelRelay, nullptr};
  juce::WebSliderParameterAttachment dryLevelWebAttachment{
      *processorRef.parameters.getParameter("dryLevel"), dryLevelRelay, nullptr};

  //==============================================================================

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
