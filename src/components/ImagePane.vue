<template>
  <v-container>
    <v-row class="text-center">
      <v-col cols="9">
        <div class="slider-outer">
          <div class="slider-inner" :key="index" v-for="(slide, index) in slides">
            <v-img
              class="slide-image"
              contain
              :src="slide.image"
              :key="index"
              v-show="preloading || currentSlideIndex == index"
            />
          </div>
        </div>
      </v-col>

      <v-col cols="3">
        <span class="mr-2">{{ slides[currentSlideIndex].memo }}</span>
      </v-col>

      <v-col cols="12">
        <v-slider
          v-model="currentSlideIndex"
          hint="Im a hint"
          min="0"
          max="16"
        ></v-slider>
      </v-col>

    </v-row>
  </v-container>
</template>

<script>
  export default {
    name: 'ImagePane',

    mounted: function() {
      setTimeout(function() {
        this.preloading = false
      }.bind(this), 2000)
    },

    data: () => ({
      hoge: 'fuga',
      preloading: true,
      currentSlideIndex: 0,
      slides: [
        {image: '/0000/0001.svg', memo: 'SCORE: 0.9272186390'},
        {image: '/0000/0002.svg', memo: 'SCORE: 0.9461761388'},
        {image: '/0000/0003.svg', memo: 'SCORE: 0.9576622611'},
        {image: '/0000/0004.svg', memo: 'SCORE: 0.9577123120'},
        {image: '/0000/0005.svg', memo: 'SCORE: 0.9578038986'},
        {image: '/0000/0006.svg', memo: 'SCORE: 0.9584905915'},
        {image: '/0000/0007.svg', memo: 'SCORE: 0.9596771750'},
        {image: '/0000/0008.svg', memo: 'SCORE: 0.9599103288'},
        {image: '/0000/0009.svg', memo: 'SCORE: 0.9586252157'},
        {image: '/0000/0010.svg', memo: 'SCORE: 0.9608280499'},
        {image: '/0000/0011.svg', memo: 'SCORE: 0.9627588833'},
        {image: '/0000/0012.svg', memo: 'SCORE: 0.9630019665'},
        {image: '/0000/0013.svg', memo: 'SCORE: 0.9762911114'},
        {image: '/0000/0014.svg', memo: 'SCORE: 0.9762911114'},
        {image: '/0000/0015.svg', memo: 'SCORE: 0.9762911114'},
        {image: '/0000/0016.svg', memo: 'SCORE: 0.9762911114'},
        {image: '/0000/0017.svg', memo: 'SCORE: 0.9762911114'},
      ],
    }),
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
