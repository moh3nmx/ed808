<template>
  <div class="breadcrumb">
    <nuxt-link to="/" class="first">Home</nuxt-link>
    <nuxt-link
      v-for="(item, index) in links"
      :key="index"
      :to="item.url">
      {{ item.title }}
    </nuxt-link>
  </div>
</template>

<script>

export default {
    name: 'Breadcrumb',
    props: ['links']
}
</script>
<style lang="scss" scoped>
.breadcrumb{
    padding: 10px 10px;
    a{
        padding: 5px 0px;
        color: #555 !important;
        text-decoration: none;
        &:not(.first):before {
            content: "chevron_right";
            margin: 0 5px 0 0;
            font-family: material icons;
            color: #E91E63;
            font-size: 24px;
            vertical-align: middle;
        }
    }
}
</style>



