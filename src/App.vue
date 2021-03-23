<template>
  <v-app>
    <v-app-bar
      app
      color="primary"
      dark
    >
      <span class="mr-2">Marathon runner</span>

    </v-app-bar>

    <v-main>
      <MainPane/>
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
    //
  }),
};
</script>
