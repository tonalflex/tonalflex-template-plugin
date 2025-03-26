#include "plugin_editor.h"
#include "plugin_processor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  // Headline Label
  addAndMakeVisible(headlineLabel);
  headlineLabel.setText("Reverb Plugin", juce::dontSendNotification);
  headlineLabel.setJustificationType(juce::Justification::centred);

  // Room Size Slider
  addAndMakeVisible(roomSizeLabel);
  roomSizeLabel.setText("Room Size", juce::dontSendNotification);
  roomSizeLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(roomSizeSlider);
  roomSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "roomSize", roomSizeSlider);

  // Damping Slider
  addAndMakeVisible(dampingLabel);
  dampingLabel.setText("Damping", juce::dontSendNotification);
  dampingLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(dampingSlider);
  dampingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "damping", dampingSlider);

  // Wet Level Slider
  addAndMakeVisible(wetLevelLabel);
  wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
  wetLevelLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(wetLevelSlider);
  wetLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "wetLevel", wetLevelSlider);

  // Dry Level Slider
  addAndMakeVisible(dryLevelLabel);
  dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
  dryLevelLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(dryLevelSlider);
  dryLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "dryLevel", dryLevelSlider);

  // Web View Setup
  addAndMakeVisible(webView);
  // webView.goToURL(juce::WebBrowserComponent::getResourceProviderRoot());
  webView.goToURL("http://localhost:5173");
  setSize(600, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
}

void AudioPluginAudioProcessorEditor::resized() {
  auto bounds = getLocalBounds();

  auto leftPanel = bounds.removeFromLeft(bounds.getWidth() / 2);

  auto headlineBounds = leftPanel.removeFromTop(40);
  headlineLabel.setBounds(headlineBounds);

  auto sliderHeight = 50;
  roomSizeLabel.setBounds(leftPanel.removeFromTop(20));
  roomSizeSlider.setBounds(leftPanel.removeFromTop(sliderHeight));

  dampingLabel.setBounds(leftPanel.removeFromTop(20));
  dampingSlider.setBounds(leftPanel.removeFromTop(sliderHeight));

  wetLevelLabel.setBounds(leftPanel.removeFromTop(20));
  wetLevelSlider.setBounds(leftPanel.removeFromTop(sliderHeight));

  dryLevelLabel.setBounds(leftPanel.removeFromTop(20));
  dryLevelSlider.setBounds(leftPanel.removeFromTop(sliderHeight));

  // Set web view bounds to the right half
  webView.setBounds(bounds);
}

// Get the zipped webview ui
std::optional<juce::WebBrowserComponent::Resource> AudioPluginAudioProcessorEditor::getResource(
    const juce::String& url) {
  const auto urlToRetrieve =
      url == "/" ? juce::String{"index.html"} : url.fromFirstOccurrenceOf("/", false, false);

  int zipSize = 0;
  const void* zipData = BinaryData::getNamedResource("assets.zip", zipSize);
  if (zipData == nullptr)
    return std::nullopt;

  juce::MemoryInputStream streamZip(zipData, static_cast<size_t>(zipSize), false);
  juce::ZipFile archive(streamZip);

  if (auto* entry = archive.getEntry(urlToRetrieve)) {
    auto entryStream = rawToUniquePtr(archive.createStreamForEntry(*entry));
    std::vector<std::byte> result((size_t)entryStream->getTotalLength());
    entryStream->setPosition(0);
    entryStream->read(result.data(), result.size());

    auto mime =
        getMimeForExtension(entry->filename.fromLastOccurrenceOf(".", false, false).toLowerCase());

    return juce::WebBrowserComponent::Resource{std::move(result), std::move(mime)};
  }

  return std::nullopt;
}

const char* AudioPluginAudioProcessorEditor::getMimeForExtension(const juce::String& extension) {
  static const std::unordered_map<juce::String, const char*> mimeMap = {
      {{"htm"}, "text/html"},
      {{"html"}, "text/html"},
      {{"txt"}, "text/plain"},
      {{"jpg"}, "image/jpeg"},
      {{"jpeg"}, "image/jpeg"},
      {{"svg"}, "image/svg+xml"},
      {{"ico"}, "image/vnd.microsoft.icon"},
      {{"json"}, "application/json"},
      {{"png"}, "image/png"},
      {{"css"}, "text/css"},
      {{"map"}, "application/json"},
      {{"js"}, "text/javascript"},
      {{"woff2"}, "font/woff2"}};

  if (const auto it = mimeMap.find(extension.toLowerCase()); it != mimeMap.end())
    return it->second;

  jassertfalse;
  return "";
}

}  // namespace audio_plugin
