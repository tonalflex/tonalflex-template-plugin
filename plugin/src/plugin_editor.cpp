// editor.cpp
#include "plugin_editor.h"
#include "plugin_processor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), 
      processorRef(p), 
      webView(juce::WebBrowserComponent::Options()
                  .withNativeIntegrationEnabled()
                  .withNativeFunction("setParameter", [this](const juce::Array<juce::var>& args, juce::WebBrowserComponent::NativeFunctionCompletion complete) {
                      if (args.size() >= 2) {
                          juce::String paramName = args[0].toString();
                          float value = args[1];
                          handleParameterChange(paramName, value);
                          complete(juce::var("success")); // Return a result to JS
                      } else {
                          complete(juce::var("error: insufficient arguments"));
                      }
                  })) {
    setSize(400, 300);

    addAndMakeVisible(headlineLabel);
    headlineLabel.setText("Reverb Plugin", juce::dontSendNotification);
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

    addAndMakeVisible(webView);
    webView.goToURL("http://localhost:5173");

    for (auto* param : processorRef.getParameterTree().getParameters(false)) {
        param->addListener(this);
    }
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {
    for (auto* param : processorRef.getParameterTree().getParameters(false)) {
        param->removeListener(this);
    }
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

    webView.setBounds(bounds);
}

void AudioPluginAudioProcessorEditor::handleParameterChange(const juce::String& paramName, float value) {
    auto& params = processorRef.getParameters();
    if (paramName == "roomSize") params.getParameter("roomSize")->setValueNotifyingHost(value);
    else if (paramName == "damping") params.getParameter("damping")->setValueNotifyingHost(value);
    else if (paramName == "wetLevel") params.getParameter("wetLevel")->setValueNotifyingHost(value);
    else if (paramName == "dryLevel") params.getParameter("dryLevel")->setValueNotifyingHost(value);
}

void AudioPluginAudioProcessorEditor::sendParameterToVue(const juce::String& paramName, float value) {
    juce::String jsCode = "if (window.updateVue) window.updateVue('" + paramName + "', " + juce::String(value) + ");";
    webView.evaluateJavascript(jsCode, nullptr);
}

void AudioPluginAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue) {
    auto params = processorRef.getParameterTree().getParameters(false);
    if (parameterIndex >= 0 && parameterIndex < params.size()) {
        auto* param = dynamic_cast<juce::AudioProcessorParameterWithID*>(params[parameterIndex]);
        if (param) {
            juce::String paramName = param->getParameterID();
            sendParameterToVue(paramName, newValue);
        }
    }
}
}  // namespace audio_plugin