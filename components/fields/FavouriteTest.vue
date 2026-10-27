<template>
  <div class="example-1 card">
    <div class="wrapper">
      <nuxt-link :to="'/node/' + nid">
        <div class="card-image">
          <div class="img" :style="`background-image:url(${picture != null ? picture : '/images/default.jpg'})`"></div>
        </div>
      </nuxt-link>
      <div class="date" v-if="claps && comments">
        <span>
          <img src="/images/clap-white.png" alt="" style="padding-bottom: 2px" width="20px">
          <md-tooltip md-direction="bottom">Claps</md-tooltip>
          {{ claps }}
        </span>
        <span>
          <md-icon class="stat-2">chat_bubble_outline
            <md-tooltip md-direction="bottom">Comments</md-tooltip>
          </md-icon>{{ comments }}
        </span>
      </div>
      <div class="data">
        <div class="content">
          <span class="author"><md-icon style="color: #f44336; font-size: 16px !important;">subject</md-icon>{{ topic }}</span>
          <h1 class="title"><nuxt-link :to="'/node/' + nid">{{ title }}</nuxt-link></h1>
          <p class="text">{{ metaDesc }}</p>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import axios from 'axios'

  export default {
    name: 'FavouriteTest',
    props: ['title', 'picture', 'metaDesc', 'topic', 'nid', 'claps', 'comments'],
  }
</script>

<style scoped lang="scss">
  @import url(https://fonts.googleapis.com/css?family=Open+Sans:300,400,700);
  // Variables
  $regal-blue: #034378;
  $san-juan: #2d4e68;
  $bermuda: #77d7b9;
  $white: #fff;
  $black: #000;
  $open-sans: 'Open Sans',
  sans-serif;
  // clear-fix mixin
  @mixin cf {
    &::before,
    &::after {
      content: '';
      display: table;
    }
    &::after {
      clear: both;
    }
  }

  * {
    box-sizing: border-box;
  }

  body {
    background-image: linear-gradient(to right, $regal-blue, $san-juan);
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    height: 100vh;
    font-family: $open-sans;
  }

  a {
    text-decoration: none;
  }

  h1 {
    font-family: "Roboto Slab";
    font-weight: 700;
    font-size: 14px;
    text-align: left;
  }

  // Base styles
  .card {
    float: left;
    .menu-content {
      @include cf;
      margin: 0;
      padding: 0;
      list-style-type: none;
      li {
        display: inline-block;
      }
      a {
        color: $white;
      }
      span {
        position: absolute;
        left: 50%;
        top: 0;
        font-size: 10px;
        font-weight: 700;
        font-family: 'Open Sans';
        transform: translate(-50%, 0);
      }
    }
    .wrapper {
      background-color: $white;
      /*min-height: 400px;*/
      position: relative;
      overflow: hidden;
      box-shadow: 0 2px 2px 0 rgba(0,0,0,.14), 0 3px 1px -2px rgba(0,0,0,.2), 0 1px 5px 0 rgba(0,0,0,.12);
      &:hover {
        .data {
          transform: translateY(0);
        }
      }
    }
    .data {
      position: absolute;
      bottom: 0;
      width: 100%;
      transform: translateY(calc(70px + 1em));
      transition: transform 0.3s;
      .content {
        padding: 1em;
        position: relative;
        z-index: 1;
        text-align: left;
      }
    }
    .author {
      text-align: left !important;
      font-size: 10px;
      font-weight: 600;
      color: #f44336;
      text-transform: uppercase;
    }
    .title {
      margin-top: 10px;
    }
    .text {
      height: 70px;
      margin: 0;
      font-size: 14px;
      text-align: left;
    }
    input[type='checkbox'] {
      display: none;
    }
    input[type='checkbox']:checked + .menu-content {
      transform: translateY(-60px);
    }
  }
  .card-image {
    margin: 0 !important;
    border-radius: 0 !important;
    z-index: 0 !important;
  }
  .img {
    padding-bottom: 110% !important;
  }

  // First example styles
  .example-1 {
    .wrapper {
      border-radius: 6px;
    }
    .date {
      position: absolute;
      top: 0;
      left: 0;
      font-size: 14px;
      background-color: #673AB7;
      color: $white;
      padding: 0.8em;
      span {
        text-align: center;
      }
      .day {
        font-weight: 700;
        font-size: 24px;
        text-shadow: 2px 3px 2px rgba($black, 0.18);
      }
      .month {
        text-transform: uppercase;
      }
      .month,
      .year {
        font-size: 12px;
      }
    }

    .content {
      background-color: $white;
      box-shadow: 0 5px 30px 10px rgba($black, 0.3);
    }
    .title {
      a {
        color: #3C4858;
      }
    }
  }
  .stat-2 {
    font-weight: 400;
    color: #e2e2e2a6 !important;
    font-size: 18px !important;
  }
</style>
