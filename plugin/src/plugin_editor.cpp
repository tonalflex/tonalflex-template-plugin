#include "plugin_editor.h"
#include "plugin_processor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  /**
   * Initialize WebView UI
   *
   * - The WebView UI is built using a web-framework (see the `/ui` directory in the project root).
   * - The compiled HTML/CSS/JS assets are embedded into the plugin via JUCE's BinaryData system.
   * - This Web UI is rendered inside the plugin editor using JUCE's WebBrowserComponent.
   * - Communication between the C++ backend and the WebView is handled via native integration.
   */
  webView = std::make_unique<juce::WebBrowserComponent>(
      juce::WebBrowserComponent::Options{}
          .withNativeIntegrationEnabled()  // (C++ <=> JS bridge, events, etc.)

          // Explicitly use WebView2 backend on Windows for modern HTML/CSS/JS support
          // JUCE defaults to WebKit on macOS/Linux
          .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
          .withWinWebView2Options(
              juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(
                  juce::File::getSpecialLocation(juce::File::tempDirectory)))

          // Provide WebView UI resources from JUCE BinaryData (HTML/CSS/JS, etc.)
          .withResourceProvider([this](const auto& url) { return getResource(url); },
                                juce::URL{"http://localhost:5173/"}.getOrigin())

          // Add support for control focus tracking in the WebView (parameter automation)
          .withOptionsFrom(controlParameterIndexReceiver)

          // Bind parameter relays for two-way communication (C++ <=> JS)
          .withOptionsFrom(roomSizeRelay)
          .withOptionsFrom(dampingRelay)
          .withOptionsFrom(wetLevelRelay)
          .withOptionsFrom(dryLevelRelay)

          // Example: register a JUCE C++ function callable from JS for debugging/testing
          .withNativeFunction(
              "exampleNativeFunction",
              [](const juce::Array<juce::var>& args,
                 juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                juce::Logger::writeToLog("exampleNativeFunction called from WebView");
                for (int i = 0; i < args.size(); ++i)
                  juce::Logger::writeToLog("Arg " + juce::String(i) + ": " + args[i].toString());
                completion("Hello from JUCE native function!");
              })

          // Inject debug message into browser console on load
          .withUserScript(R"(console.log("JUCE C++ Backend is running!");)"));

  // Set size of desktop plugin window (pixels)
  setSize(600, 600);

  // Ensure WebView is added after full construction (avoids timing issues)
  juce::MessageManager::callAsync([this]() {
    addAndMakeVisible(*webView);
    webView->goToURL(juce::WebBrowserComponent::getResourceProviderRoot() + "index.html");
  });

  /**
   * Initialize Native JUCE UI (for development and demo purposes)
   *
   * - This traditional JUCE-based GUI exists alongside the WebView UI.
   * - Useful for testing parameter bindings or fallback scenarios.
   * - This section is not required in production and can be removed or disabled.
   */
  addAndMakeVisible(headlineLabel);
  headlineLabel.setText("Template Plugin", juce::dontSendNotification);
  headlineLabel.setJustificationType(juce::Justification::centred);

  addAndMakeVisible(roomSizeLabel);
  roomSizeLabel.setText("Room Size", juce::dontSendNotification);
  roomSizeLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(roomSizeSlider);
  roomSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "roomSize", roomSizeSlider);

  addAndMakeVisible(dampingLabel);
  dampingLabel.setText("Damping", juce::dontSendNotification);
  dampingLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(dampingSlider);
  dampingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "damping", dampingSlider);

  addAndMakeVisible(wetLevelLabel);
  wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
  wetLevelLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(wetLevelSlider);
  wetLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "wetLevel", wetLevelSlider);

  addAndMakeVisible(dryLevelLabel);
  dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
  dryLevelLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(dryLevelSlider);
  dryLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.getParameters(), "dryLevel", dryLevelSlider);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {
  webView->setVisible(false);
  webView->stop();
  juce::Logger::writeToLog("~AudioPluginAudioProcessorEditor destroyed");
}

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

  webView->setBounds(bounds);  // Set web view bounds to the right half
}

// Get the WebView UI resources from BinaryData
std::optional<juce::WebBrowserComponent::Resource> AudioPluginAudioProcessorEditor::getResource(
    const juce::String& url) {
  juce::Logger::writeToLog("Requested URL: " + url);

  // Extract filename and normalize to match BinaryData naming
  juce::String filename = juce::URL(url).getFileName().trim();
  juce::String resourceName = filename.removeCharacters("-").replaceCharacter('.', '_');

  int size = 0;
  const char* data = BinaryData::getNamedResource(resourceName.toRawUTF8(), size);

  if (data == nullptr || size <= 0) {
    juce::Logger::writeToLog("Resource not found or empty: " + resourceName);
    return std::nullopt;
  }

  std::vector<std::byte> content(static_cast<size_t>(size));
  std::memcpy(content.data(), data, static_cast<size_t>(size));

  juce::String ext = filename.fromLastOccurrenceOf(".", false, false).toLowerCase();
  juce::String mime = getMimeForExtension(ext);
  if (mime.isEmpty())
    mime = "application/octet-stream";

  juce::Logger::writeToLog("Returning resource: " + resourceName + " (" + mime + ")");
  return juce::WebBrowserComponent::Resource{std::move(content), mime};
}

// Map file extensions to MIME types for serving embedded resources in the WebView UI
juce::String AudioPluginAudioProcessorEditor::getMimeForExtension(const juce::String& extension) {
  static const std::unordered_map<juce::String, juce::String> mimeMap = {
      {"htm", "text/html"},
      {"html", "text/html"},
      {"txt", "text/plain"},
      {"jpg", "image/jpeg"},
      {"jpeg", "image/jpeg"},
      {"svg", "image/svg+xml"},
      {"ico", "image/vnd.microsoft.icon"},
      {"json", "application/json"},
      {"png", "image/png"},
      {"css", "text/css"},
      {"map", "application/json"},
      {"js", "text/javascript"},
      {"woff2", "font/woff2"}};

  const auto lower = extension.toLowerCase();

  if (const auto it = mimeMap.find(lower); it != mimeMap.end())
    return it->second;

  jassertfalse;
  return "application/octet-stream";
}

}  // namespace audio_plugin
