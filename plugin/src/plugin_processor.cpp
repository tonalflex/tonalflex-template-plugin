#include "plugin_processor.h"

#if !HEADLESS
#include "plugin_editor.h"
#endif

// Small tolerance for floating-point comparisons
constexpr float epsilon = 1e-5f;

namespace audio_plugin {
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
      parameters(*this, nullptr, "PARAMETERS", [] {
        return juce::AudioProcessorValueTreeState::ParameterLayout{
            std::make_unique<juce::AudioParameterFloat>("roomSize", "Room Size", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("damping", "Damping", 0.0f, 1.0f, 0.3f),
            std::make_unique<juce::AudioParameterFloat>("wetLevel", "Wet Level", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("dryLevel", "Dry Level", 0.0f, 1.0f, 0.8f)};
      }()) {
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {
  releaseResources();
  juce::Logger::writeToLog("[Processor] Destructor called");
}

const juce::String AudioPluginAudioProcessor::getName() const {
  return "Plugin";  // Replace with plugin name
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
  return false;
}
bool AudioPluginAudioProcessor::producesMidi() const {
  return false;
}
bool AudioPluginAudioProcessor::isMidiEffect() const {
  return false;
}
double AudioPluginAudioProcessor::getTailLengthSeconds() const {
  return 2.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1;
}
int AudioPluginAudioProcessor::getCurrentProgram() {
  return 0;
}
void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}
const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}
void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  juce::ignoreUnused(samplesPerBlock);

  // Initialize parameters
  reverbParams.roomSize = *parameters.getRawParameterValue("roomSize");
  reverbParams.damping = *parameters.getRawParameterValue("damping");
  reverbParams.wetLevel = *parameters.getRawParameterValue("wetLevel");
  reverbParams.dryLevel = *parameters.getRawParameterValue("dryLevel");

  reverb.setParameters(reverbParams);
  reverb.reset();

  // Ignore build warnings for unused variables
  juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources() {
  juce::Logger::writeToLog("[Processor] releaseResources() called");
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;

  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();
  auto numSamples = buffer.getNumSamples();

  // Clear unused output channels
  for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, numSamples);

  // Update reverb parameters only if they change
  if (std::abs(reverbParams.roomSize - *parameters.getRawParameterValue("roomSize")) > epsilon ||
      std::abs(reverbParams.damping - *parameters.getRawParameterValue("damping")) > epsilon ||
      std::abs(reverbParams.wetLevel - *parameters.getRawParameterValue("wetLevel")) > epsilon ||
      std::abs(reverbParams.dryLevel - *parameters.getRawParameterValue("dryLevel")) > epsilon) {
    reverbParams.roomSize = *parameters.getRawParameterValue("roomSize");
    reverbParams.damping = *parameters.getRawParameterValue("damping");
    reverbParams.wetLevel = *parameters.getRawParameterValue("wetLevel");
    reverbParams.dryLevel = *parameters.getRawParameterValue("dryLevel");
    reverb.setParameters(reverbParams);
  }

  // Auto-Detect Mono/Stereo input and process accordingly
  if (totalNumInputChannels == 1 && totalNumOutputChannels >= 2) {
    auto* monoChannel = buffer.getWritePointer(0);

    // Duplicate the mono channel into the second channel for stereo processing
    auto* rightChannel = buffer.getWritePointer(1);
    std::copy(monoChannel, monoChannel + numSamples, rightChannel);

    // Mono Input => Stereo Output
    reverb.processStereo(monoChannel, rightChannel, numSamples);
  } else if (totalNumInputChannels >= 2) {
    // Stereo Input => Stereo Output
    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), numSamples);
  } else if (totalNumInputChannels == 1 && totalNumOutputChannels == 1) {
    // Mono Input => Mono Output
    reverb.processMono(buffer.getWritePointer(0), numSamples);
  }
}

bool AudioPluginAudioProcessor::hasEditor() const {
  return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
#if !HEADLESS
  return new AudioPluginAudioProcessorEditor(*this);
#else
  return nullptr;
#endif
}

void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
  juce::ignoreUnused(destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
  juce::ignoreUnused(data, sizeInBytes);
}
}  // namespace audio_plugin

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}
