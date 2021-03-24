<template>
  <v-app>
    <v-app-bar
      app
      color="primary"
      dark
    >
      <span class="mr-2">Marathon runner</span>

      <span class="mr-2">{{ active_history }}</span>

    </v-app-bar>

    <v-main>
      <MainPane :onSelectHistory="onSelectHistory" />
    </v-main>
  </v-app>
</template>

<script>
import MainPane from './components/MainPane.vue';

export default {
  name: 'App',

  components: {
    MainPane,
  },

  data: () => ({
    active_history: ""
  }),

  methods: {
    onSelectHistory(history) {
      this.active_history = history
    }
  }
};
</script>
