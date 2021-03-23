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
      <ImagePane/>
      <HelloWorld/>
    </v-main>
  </v-app>
</template>

<script>
import ImagePane from './components/ImagePane.vue';

export default {
  name: 'App',

  components: {
    ImagePane,
  },

  data: () => ({
    //
  }),
};
</script>
