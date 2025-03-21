<!-- App.vue -->
<template>
  <div>
    <h1>Reverb Plugin</h1>
    <div class="slider">
      <label>Room Size: {{ roomSize }}</label>
      <input type="range" min="0" max="1" step="0.01" v-model="roomSize" @input="updateParameter('roomSize', roomSize)" />
    </div>
    <div class="slider">
      <label>Damping: {{ damping }}</label>
      <input type="range" min="0" max="1" step="0.01" v-model="damping" @input="updateParameter('damping', damping)" />
    </div>
    <div class="slider">
      <label>Wet Level: {{ wetLevel }}</label>
      <input type="range" min="0" max="1" step="0.01" v-model="wetLevel" @input="updateParameter('wetLevel', wetLevel)" />
    </div>
    <div class="slider">
      <label>Dry Level: {{ dryLevel }}</label>
      <input type="range" min="0" max="1" step="0.01" v-model="dryLevel" @input="updateParameter('dryLevel', dryLevel)" />
    </div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      roomSize: 0.5,
      damping: 0.5,
      wetLevel: 0.5,
      dryLevel: 0.5
    };
  },
  methods: {
    updateParameter(paramName, value) {
      if (window.__JUCE__ && window.__JUCE__.backend) {
        window.__JUCE__.backend.callNativeFunction("setParameter", [paramName, value]).then(result => {
          console.log(`Native call result: ${result}`);
        }).catch(err => {
          console.error(`Native call error: ${err}`);
        });
      }
    },
    updateFromJuce(paramName, value) {
      console.log(window.__JUCE__.initialisationData);
      if (paramName === "roomSize") this.roomSize = value;
      else if (paramName === "damping") this.damping = value;
      else if (paramName === "wetLevel") this.wetLevel = value;
      else if (paramName === "dryLevel") this.dryLevel = value;
    }
  },
  mounted() {
    window.updateVue = this.updateFromJuce;
  }
};
</script>

<style>
body {
  font-family: Arial, sans-serif;
  padding: 20px;
  background-color: #f0f0f0;
  color:grey;
}
.slider {
  margin: 20px 0;
}
label {
  display: block;
  text-align: center;
}
input[type="range"] {
  width: 100%;
}
</style>