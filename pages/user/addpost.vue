<template>
  <div class="container raised">
    <addPost viewMode="wild"></addPost>
  </div>
</template>

<script>
  import addPost from "@/components/user/addPost"
  export default {
    name: "AddPost",
    layout:'userpanel',
    components:{
      addPost
    }
  }
</script>

<style scoped>

</style>
