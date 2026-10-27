<template>
  <div class="error-cont">
    <div class="container">
      <h1>{{ error.statusCode  }}</h1>
      <h3 v-if="error.statusCode === 404">
        {{ error.message }}
      </h3>
      <h3 v-else>{{error.message}}</h3>
      <p>The page you're looking for doesn't exist or an error occurred.</p>
      <a @click="$router.go(-1)">Go Back!</a>
    </div>
  </div>

</template>

<script>
  export default {
    props: ['error'],
    layout: 'static', // you can set a custom layout for the error page
    mounted(){
      this.$store.state.backGround = '/images/295.jpg'
    }
  }
</script>

<style lang="scss" scoped>
  .container {
    position: absolute;
    top: 25%;
    left: 15%;
    right: 15%;
    z-index: 10;
    border-radius: 5px;
    background: white;
    width: 70%;
    margin-left: auto;
    margin-right: auto;
    padding: 50px 0;
  }
  a {
    cursor: pointer;
  }
</style>
