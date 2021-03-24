<template>
  <v-container>
    <v-card
        class="mx-auto"
        tile
    >
      <v-list flat>
        <v-subheader>HISTORIES</v-subheader>
        <v-list-item-group
          v-model="selectedItem"
          color="primary"
          @change="onSelect(selectedItem)"
        >
          <v-list-item
            v-for="(item, i) in histories"
            :key="i"
          >
            <v-list-item-content>
              <v-list-item-title v-text="item"></v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list-item-group>
      </v-list>
    </v-card>
  </v-container>
</template>

<script>
export default {
  name: 'HistoryPane',

  props: {
    histories: {
      type: Array,
      default: () => []
    },
    onSelect: {
      type: Function,
      required: true,
      default: () => {}
    }
  },

  data: () => ({
    selectedItem: -1
  }),
}
</script>
