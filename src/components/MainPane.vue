<template>
  <v-container>
    <v-row class="text-center">
      <v-col cols="9">
        <SlidePane :directory="directory" :slides="slides" />
      </v-col>
      <v-col cols="3">
        <HistoryPane :histories="histories" :on-select="onSelect" />
      </v-col>
    </v-row>

  </v-container>
</template>

<script>
import axios from 'axios'
import SlidePane from './SlidePane.vue';
import HistoryPane from './HistoryPane.vue';

export default {
  name: 'MainPane',

  mounted: function() {
    axios.get("/results/all.json").then(response => (this.histories = response.data))
  },

  components: {
    SlidePane,
    HistoryPane,
  },

  data: () => ({
    histories: [],
    directory: "",
    slides: [],
  }),

  methods: {
    onSelect(index) {
      let history = this.histories[index]
      this.directory = history
      axios.get("/results/" + history + "/info.json").then(response => (this.slides = response.data))
    }
  }
};
</script>
