<template>
  <div class="container">
    <div v-if="loading" class="events-pane loading">
      <md-progress-spinner
        :md-diameter="100"
        :md-stroke="5"
        md-mode="indeterminate"/>
    </div>
    <div class="md-headline">
      <h5>Products and Services</h5>
      <div class="more-link">
        <!--<router-link tag="md-button" to="/contents?type=4058">See All</router-link>-->
        <!-- this link is temporary. it will change after events page create -->
        <nuxt-link tag="md-button" to="/contents?type=4599">See All</nuxt-link>
      </div>
    </div>
    <div class="products-row md-layout md-gutter">
      <carousel
        :per-page="perPage"
        :navigationEnabled="true"
        navigationNextLabel="<i class='mdi mdi-chevron-right'></i>"
        navigationPrevLabel="<i class='mdi mdi-chevron-left'></i>"
        :paginationEnabled="false"
      >
        <slide v-for="product in products" :key="product.nid">
          <div class="card card-product">
            <nuxt-link :to="product.nid == '-1' ? '/seismosoft' : `/node/${product.nid}`">
              <div class="card-image">
                <div class="img" :style="`background-image: url(${product.picture})`"></div>
                <div class="colored-shadow" :style="`background-image: url(${product.picture}); opacity: 1;`"></div>
              </div>
              <div class="card-content">
                <!-- <h6 class="category text-rose">Trending</h6> -->
                <h4 class="card-title">
                  {{ product.title }}
                </h4>
                <!-- <div class="card-description">
                  Dolce &amp; Gabbana's 'Greta' tote has been crafted in Italy from hard-wearing red textured-leather.
                </div> -->
                <div class="footer">
                  <div class="author">
                    <nuxt-link :to="`/user/${product.uid}`">
                      <img :src="product.author_pic" alt="..." class="avatar img-raised">
                      <span>{{ product.author_name }}</span>
                      <!-- <div class="ripple-container">
                        <div class="ripple ripple-on ripple-out" style="left: 438px; top: 16930px; background-color: rgb(60, 72, 88); transform: scale(12.75);"></div>
                      </div> -->
                    </nuxt-link>
                  </div>
                  <!-- <div class="stats">
                      <md-icon>schedule</md-icon> 5 min read
                  </div> -->
                  <div class="price">
                    <h4>{{ product.nid != '-1' ? 'FREE' : ' ' }}</h4>
                  </div>
                  <!-- <div class="stats">
                    <button type="button" rel="tooltip" title="" class="btn btn-just-icon btn-simple btn-rose" data-original-title="Saved to Wishlist">
                      <i class="material-icons">favorite</i>
                    </button>
                  </div> -->
                </div>
              </div>
            </nuxt-link>
          </div>
        </slide>
      </carousel>
    </div>
  </div>
</template>

<script>
export default {
  name: 'Products',
  data(){
    return {
      products: [],
      loading: true,
      windowWidth: 0
    }
  },
  computed: {
    perPage: function () {
      if(this.windowWidth <= 600)
        return 1
      else if(this.windowWidth > 600 && this.windowWidth < 992)
        return 2
      else
        return 4
    }
  },
  mounted(){
    fetch('https://temp.ed808.com/api/latin/contents/list/services?parameter[limits]=3')
      .then(response => response.json())
      .then((data) => {
        this.products = data
        this.loading = false
        let sesimo = {
          "nid": "-1",
          "title": "SeismoSoft: Earthquake Engineering Solutions",
          "uid": "530",
          "created": "June 3, 2019",
          "changed": "July 2, 2019",
          "picture": "https://temp.ed808.com/api/sites/default/files/styles/300x200/public/fields/image/node_19893_620498b1.png?itok=lbqTvS3C",
          "author_name": "Stelios Antoniou",
          "author_pic": "https://temp.ed808.com/api/sites/default/files/styles/200x200/public/pictures/picture-530-1560515090.jpg?itok=UX8IXTkI",
        }
        this.products.push(sesimo)
    })
    this.windowWidth = window.innerWidth
    window.addEventListener('resize', () => {
      this.windowWidth = window.innerWidth
    });
  }
}
</script>

<style lang="scss" scoped>
  .container {
    padding: 15px;
  }
  .products-row {
    width: 100%;
    padding: 0 36px;
    .VueCarousel-inner {
      display: flex;
      flex-direction: row;
      backface-visibility: hidden;
      flex-basis: unset !important;

      .VueCarousel-slide {
        padding: 10px;
        min-width: 250px;
      }
    }
  }
  .card.card-product {
    transition: .4s;
    &:hover {
      box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23);
    }
  }
  .card.card-signup {
    display: inline-block;
    position: relative;
    width: 100%;
    margin-bottom: 30px;
    border-radius: 6px;
    color: rgba(0, 0, 0, 0.87);
    background: #fff;
    box-shadow: 0 2px 2px 0 rgba(0, 0, 0, 0.14), 0 3px 1px -2px rgba(0, 0, 0, 0.2), 0 1px 5px 0 rgba(0, 0, 0, 0.12);

    .footer {
      margin-top: 15px;
      padding-bottom: 15px;

      a {
        background-color: transparent;
        color: #9c27b0;
        padding: 18px 5px;
        display: inline-block;
      }
    }


    .card-content {
      padding: 0px 25px 0px 60px;

      .input-with-icon {
        position: relative;

        .icon-prefix {
          position: absolute;
          left: -35px;
          top: 21px;
          font-size: 24px;
          color: #757575;
        }

        .md-input {}
      }

      .interested-topics {
        position: relative;
        text-align: justify;
        height: auto;

        .topic {
          width: calc(25% - 10px);
          margin: 0 5px;
          margin-bottom: 10px;
          border-radius: 5px;
          float: left;
          background-position: center;
          background-size: cover;
          position: relative;

          @media screen and (max-width: 720px) {
            width: calc(50% - 10px);
          }

          &:after {
            content: '';
            padding-bottom: 100%;
            border-radius: 5px;
            background-color: rgba(0, 0, 0, 0.4);
            display: block;
            transition-duration: 0.5s;
          }

          &.topic-active {
            &:after {
              background-color: rgba(0, 0, 0, 0.7);
            }
          }

          span.inside-text {
            position: absolute;
            text-align: left;
            font-weight: 500;
            bottom: 10px;
            left: 10px;
            right: 10px;
            vertical-align: middle;
            color: #dbdbdb;
            transition-duration: 0.5s;

            .mdi {
              font-size: 30px;
            }
          }

          span.check-text {
            position: absolute;
            /*top: calc( 50% - 13px );*/
            color: #fff;
            top: 10px;
            left: 10px;
            right: 10px;
            text-align: right;
            transition-duration: 0.5s;

            .mdi {
              font-size: 30px;
            }
          }

          &:hover {
            cursor: pointer;

            &:after {
              background-color: rgba(0, 0, 0, 0.7);
            }

            span.inside-text {
              color: #fff;
            }
          }
        }
      }
    }

    &.card-topics {
      .card-content {
        padding: 0 10px;

      }
    }

    .header {
      box-shadow: 0 16px 38px -12px rgba(0, 0, 0, 0.56), 0 4px 25px 0px rgba(0, 0, 0, 0.12), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
      margin-left: 20px;
      margin-right: 20px;
      margin-top: -40px;
      padding: 20px 0;
      border-radius: 3px;
      position: relative;

      .md-progress-bar {
        position: absolute;
        bottom: 0;
        left: 0;
        right: 0;
        border-bottom-right-radius: 3px;
        border-bottom-left-radius: 3px;
        overflow: hidden;
        background-color: rgba(255, 255, 255, 0.50);

      }

      .md-progress-bar.md-theme-default.md-indeterminate .md-progress-bar-track:after,
      .md-progress-bar.md-theme-default.md-indeterminate .md-progress-bar-fill:after,
      .md-progress-bar.md-theme-default.md-query .md-progress-bar-track:after,
      .md-progress-bar.md-theme-default.md-query .md-progress-bar-fill:after {
        background-color: #92278f66 !important;
      }
    }

    .header-primary,
    .content-primary {
      background: linear-gradient(60deg, #AB47BC, #7b1fa2);

      .card-title {
        color: #fff;
        font-weight: 700;
        font-family: 'Roboto Slab';
      }
    }

    .social-line {
      margin-top: 15px;
      text-align: center;
      padding: 0;
    }

    .btn-just-icon {
      .mdi {
        color: #fff;
        font-size: 20px;
      }
    }
  }

</style>
