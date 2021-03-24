<template>
  <v-container>
    <v-row class="text-center">
      <v-col cols="9">
        <div class="slider-outer">
          <div class="slider-inner" :key="index" v-for="(slide, index) in slides">
            <v-img
              class="slide-image"
              contain
              :src="imagePath(slide)"
              :key="index"
              v-show="preloading || currentSlideIndex == index"
            />
          </div>
        </div>
        <v-slider
          v-model="currentSlideIndex"
          hint="Im a hint"
          min="0"
          :max="slides.length - 1"
          @change="onChange"
        ></v-slider>
      </v-col>

      <v-col cols="3">
        <span class="mr-2">{{ currentSlide.score }}</span>
        <v-textarea
          name="log"
          auto-grow
          dense
          readonly
          :value="currentLog"
        ></v-textarea>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import axios from 'axios'

export default {
  name: 'SlidePane',

  props: {
    directory: {
      type: String,
      default: ""
    },
    slides: {
      type: Array,
      default: () => []
    }
  },

  data: () => ({
    preloading: true,
    currentSlideIndex: -1,
    logs: {},
    currentLog: ""
  }),

  computed: {
    currentSlide: function() {
      if (this.currentSlideIndex >= 0 && this.currentSlideIndex < this.slides.length) {
        return this.slides[this.currentSlideIndex]
      } else {
        return {}
      }
    }
  },

  watch: {
    directory: {
      immediate: true,
      handler: function() {
        this.logs = {}
        this.preload()
        setTimeout(function() {
          this.currentSlideIndex = 0
          this.onChange(0)
        }.bind(this), 50)
      }
    }
  },

  methods: {
    preload() {
        this.preloading = true
        setTimeout(function() {
          this.preloading = false
        }.bind(this), 1000)
    },
    imagePath(item) {
      return '/results/' + this.directory + '/' + item.image
    },
    onChange(index) {
      if (index < 0 || index >= this.slides.length) return
      this.currentLog = this.logs[index]
      if (!this.currentLog) {
        axios.get('/results/' + this.directory + '/' + this.slides[index].log)
        .then(function(response) {
          this.logs[index] = response.data
          if (index == this.currentSlideIndex) {
            this.currentLog = this.logs[index]
          }
        }.bind(this))
      }
    }
  }

}
</script>

<style lang="scss" scoped>

.slider-outer {
  position: relative;
  width: 600px;
  height: 600px;
  overflow: hidden;
  margin: 0 auto 20px;
}

.slider-inner {
  position: absolute;
  width: 600px;
  height: 600px;
}

.slide-image {
  object-fit: cover;
}

.fade-enter-active, .fade-leave-active {
  transition: all 0.2s ease;
}

.fade-enter, .fade-leave-to {
  opacity: 0;
}

</style>
