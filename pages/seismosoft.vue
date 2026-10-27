<template>
  <div class="service-details">
    <div class="page-header">
      <h2>SeismoSoft: Earthquake Engineering Solutions</h2>
    </div>
    <div class="container-raised">
      <p>
        ED808 has partnered with
        <a href="https://www.seismosoft.com/resellers">Seismosoft</a>, a leading developer of earthquake engineering software solutions.
        Founded in 2002, Seismosoft aims to provide the earthquake engineering community with access to powerful, state-of-the-art analytical tools, focusing not just on technical advancement but also ensuring ease-of-use.
      </p>
      <div class="software">
        <img src="/images/sbuild.png" alt />
        <div class="details">
          <h3>SeismoBuild</h3>
          <p>is an innovative Finite Elements package for the seismic assessment and strengthening of reinforced concrete framed structures that is targeted to the design office. The program is capable of fully carrying out the Code defined assessment methodologies from the structural modelling, through to the required analyses, and the corresponding member checks.</p>
          <a href="https://www.seismosoft.com/seismobuild-prod">
            <md-button class="md-raised md-primary">More Details</md-button>
          </a>
        </div>
      </div>

      <div class="software">
        <img src="/images/sstruct-r.png" alt />
        <div class="details">
          <h3>SeismoStruct</h3>
          <p>is an award-winning program developed for the accurate analytical assessment of different classes of structures, such as buildings, bridges or industrial plants, subjected to earthquake strong motion. It features a number of verified nonlinear static and dynamic analysis methods (pushover, incremental dynamic analysis, etc.) so as to meet the analytical requirements posed by the modern performance-based seismic assessment and design philosophy.</p>
          <a href="https://www.seismosoft.com/seismostruct-prod">
            <md-button class="md-raised md-primary">More Details</md-button>
          </a>
        </div>
      </div>

      <div class="software">
        <img src="/images/ssignal.png" alt />
        <div class="details">
          <h3>SeismoSignal</h3>
          <p>constitutes a simple, yet efficient, package for the processing of strong-motion data. Amongst other things, it allows for the derivation of elastic and constant ductility inelastic response spectra, computation of Fourier amplitude spectra, filtering of high and low frequency record content and estimation of other important seismological parameters, such as the Arias Intensity and the significant and effective durations.</p>
          <a href="https://www.seismosoft.com/seismosignal-prod">
            <md-button class="md-raised md-primary">More Details</md-button>
          </a>
        </div>
      </div>

      <div class="software">
        <img src="/images/smatch.png" alt />
        <div class="details">
          <h3>SeismoMatch</h3>
          <p>is an application capable of adjusting earthquake records, through wavelet addition, to match a specific target response spectrum. Users have the opportunity to simultaneously match a number of accelerograms, and then obtain a mean matched spectrum whose maximum misfit respects a pre-defined tolerance. This software can thus be used in combination with records selection tools and records appropriateness verification algorithms to define adequate suites of records for nonlinear dynamic analysis of new or existing structures.</p>
          <a href="https://www.seismosoft.com/seismomatch-prod">
            <md-button class="md-raised md-primary">More Details</md-button>
          </a>
        </div>
      </div>

      <div class="software">
        <img src="/images/sspect.png" alt />
        <div class="details">
          <h3>SeismoSpect</h3>
          <p>allows users to create their own library of ground motion records and save them all in a single file making it easy to handle and share large numbers of records. This application is then capable of applying several filter types, perform baseline-correction, computing the mean spectral response of a collection of accelerograms and to compare these results with a target spectrum. A number of strong-motion parameters can also be calculated.</p>
          <a href="https://www.seismosoft.com/seismospect-prod">
            <md-button class="md-raised md-primary">More Details</md-button>
          </a>
        </div>
      </div>

      <div class="software">
        <img src="/images/sattif.png" alt />
        <div class="details">
          <h3>SeismoArtif</h3>
          <p>is an application capable of generating artificial earthquake accelerograms matched to a specific target response spectrum using different calculation methods and varied assumptions. It is noted that the use of real accelerograms and spectrum matching techniques (i.e. SeismoMatch), together with records selection tools, tends to be recommended for the derivation of suites of records for use in nonlinear dynamic analysis of structures. However, in those cases where access to real accelerograms is, for whatever reason, challenging or inappropriate, then a tool such as SeismoArtif will be of pertinence and usefulness.</p>
          <a href="https://www.seismosoft.com/seismoartif-prod">
            <md-button class="md-raised md-primary">More Details</md-button>
          </a>
        </div>
      </div>
      <div class="software">
        <img src="/images/select.jpg" alt />
        <div class="details">
          <h3>SeismoSelect</h3>
          <p>
            SeismoSelect is an easy and efficient way to search, select, scale and download ground motion data from different strong motion databases that are available on-line. Different criteria may be employed as the parameters of interest, with which to carry out the searches. These include a target response spectrum, different ground motion parameters (e.g. PGA, PGV, Arias or Housner Intensity), information regarding the event (e.g. magnitude, faulting style location, date) or the recording site (e.g. Vs30, epicentral distance).
            Based on these parameters the software carries out searches for sets of compatible records and provides ways to easily download the selected records.
          </p>
          <a href="https://seismosoft.com/products/seismoselect/">
            <md-button class="md-raised md-primary">More Details</md-button>
          </a>
        </div>
      </div>

      <section>
        <youtube-video url="https://www.youtube.com/watch?v=bi4TPzo-Jrs&t=4s" />
      </section>

      <p>
        In order to watch educational videos of SeismoStruct, visit
        <nuxt-link to="/node/20320">this page</nuxt-link>.
      </p>

      <section class="about-founder">
        <h3>About Co-Founder</h3>

        <nuxt-link to="/user/530" class="stelious">
          <div class="card card-profile card-plain">
            <div class="card-header card-avatar">
              <a>
                <img class="img" :src="profile.picture" />
              </a>
            </div>
            <div class="card-body">
              <h4 class="card-title">{{ profile.full_name }}</h4>
              <h6 class="card-category text-muted">{{ profile.job }}</h6>
              <p class="card-description">{{ profile.about }}</p>
            </div>
            <!--<div class="card-footer justify-content-center">-->
            <!--<a href="#pablo" class="btn btn-just-icon btn-link btn-twitter"><i class="fa fa-twitter"></i></a>-->
            <!--<a href="#pablo" class="btn btn-just-icon btn-link btn-dribbble"><i class="fa fa-dribbble"></i></a>-->
            <!--<a href="#pablo" class="btn btn-just-icon btn-link btn-linkedin"><i class="fa fa-linkedin"></i></a>-->
            <!--</div>-->
          </div>
        </nuxt-link>
      </section>
    </div>
    <contact-form
      :component="true"
      altText="If you have any question or require any further information, please contact us."
    />
  </div>
</template>

<script>
import ContactForm from "@/components/fields/ContactForm";
import YoutubeVideo from "@/components/fields/YoutubeVideo";
import axios from "axios";

export default {
  name: "SeismoSoft",
  layout: "static",
  components: { YoutubeVideo, ContactForm },
  data() {
    return {
      title: "SeismoSoft: Earthquake Engineering Solutions",
      profile: {}
    };
  },
  head() {
    return {
      links: [
        { rel: "canonical", href: "https://ed808.com/seismosoft" },
        {
          rel: "alternate",
          href: "https://ed808.com/seismosoft",
          hreflang: "en"
        },
        { rel: "shortlink", href: "https://ed808.com/seismosoft" }
      ],
      title: "SeismoSoft: Earthquake Engineering Solutions",
      meta: [
        // OpenGraph data (Most widely used)
        { property: "og:title", content: this.title, hid: "og:title" },
        {
          property: "og:url",
          content: "https://ed808.com/seismosoft",
          hid: "og:url"
        },
        {
          property: "og:image",
          content: "/images/laptop.jpg",
          hid: "og:image"
        },

        // Twitter card
        { name: "twitter:title", content: this.title, hid: "twitter:title" },
        {
          name: "twitter:image:src",
          content: "/images/laptop.jpg",
          hid: "twitter:image:src"
        },

        // Google / Schema.org markup:
        { itemprop: "name", content: this.title, hid: "name" },
        { itemprop: "image", content: "/images/laptop.jpg", hid: "image" }
      ]
    };
  },
  mounted() {
    this.$store.state.backGround = "/images/laptop.jpg";
    axios.get("https://ed808.com:92/latin/user/530").then(data => {
      this.profile = data.data;
    });
  }
};
</script>

<style lang="scss" scoped>
@import "@/assets/scss/vars.scss";
.service-details {
  .page-header {
    height: 40vh;
    position: relative;
    h2 {
      font-family: "roboto slab";
      z-index: 1;
      color: #fff;
      margin: 0;
      position: absolute;
      top: calc(50% - 43.5px);
      width: 100%;
    }
  }

  .container-raised {
    // margin-top: 40vh !important;
    min-height: 100vh;
    .software {
      margin: 100px 0;
    }
    .software:nth-child(even) {
      display: flex;
      direction: rtl;
      flex-wrap: wrap;
      img {
        width: 60%;
        height: 100%;
        margin: auto;
        @media #{$x960} {
          width: 40%;
        }
        @media #{$x720} {
          width: 100%;
        }
      }
      .details {
        width: 40%;
        margin: auto;
        padding: 15px;
        @media #{$x960} {
          width: 60%;
        }
        @media #{$x720} {
          width: 100%;
        }
        h3 {
          direction: ltr;
          text-align: left;
          font-family: "roboto slab";
          &:after {
            content: "";
            border-bottom: 4px solid #673ab7;
            width: 20px;
            display: block;
            margin-top: 10px;
          }
        }
        p {
          direction: ltr;
          text-align: justify;
        }
      }
    }
    .software:nth-child(odd) {
      display: flex;
      direction: ltr;
      flex-wrap: wrap;
      img {
        width: 60%;
        height: 100%;
        margin: auto;
        @media #{$x960} {
          width: 40%;
        }
        @media #{$x720} {
          width: 100%;
        }
      }
      .details {
        width: 40%;
        padding: 15px;
        margin: auto;
        @media #{$x960} {
          width: 60%;
        }
        @media #{$x720} {
          width: 100%;
        }
        h3 {
          text-align: right;
          font-family: "roboto slab";
          direction: rtl;
          &:after {
            content: "";
            border-bottom: 5px solid #673ab7;
            width: 30px;
            display: block;
            margin-top: 10px;
          }
        }
        p {
          text-align: justify;
        }
      }
    }
  }
  .about-founder {
    margin-top: 80px;
    h3 {
      font-family: "roboto slab";
      &:after {
        content: "";
        border-bottom: 5px solid #673ab7;
        width: 30px;
        display: block;
        margin: 10px auto;
      }
    }
    .stelious {
      display: block;
      width: 50%;
      margin: auto;
      min-width: 300px;
    }
    .card-plain {
      background: transparent;
      box-shadow: none;
    }

    .card .card-body,
    .card .card-footer {
      padding: 0.9375rem 1.875rem;
    }

    .card-profile,
    .card-testimonial {
      margin-top: 30px;
      text-align: center;
    }

    .card-profile .card-avatar + .card-body,
    .card-testimonial .card-avatar + .card-body {
      margin-top: 15px;
    }

    .card-plain .card-body {
      padding-left: 5px;
      padding-right: 5px;
    }

    .card .card-title {
      margin-top: 0.625rem;
    }

    .card-title,
    .card-title a,
    .footer-big h4,
    .footer-big h4 a,
    .footer-big h5,
    .footer-big h5 a,
    .footer-brand,
    .footer-brand a,
    .info-title,
    .info-title a,
    .media .media-heading,
    .media .media-heading a,
    .title,
    .title a {
      color: #3c4858;
      text-decoration: none;
      font-weight: 700;
      font-size: 1.125rem;
      line-height: 1.5em;
    }

    .bmd-help,
    .text-muted {
      color: #6c757d !important;
      text-transform: uppercase;
      font-weight: 500;
      font-size: 0.75rem;
    }

    .card-profile .card-avatar {
      width: 130px;
      max-width: 130px;
      max-height: 130px;
    }

    p {
      color: #999;
      font-size: 14px;
    }

    .card-profile .card-avatar,
    .card-testimonial .card-avatar {
      margin: -50px auto 0;
      border-radius: 50%;
      overflow: hidden;
      padding: 0;
      box-shadow: 0 16px 38px -12px rgba(0, 0, 0, 0.56),
        0 4px 25px 0 rgba(0, 0, 0, 0.12), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
    }

    .card-profile .card-avatar img,
    .card-testimonial .card-avatar img {
      width: 100%;
      height: auto;
    }
    .card-plain.card-profile .card-avatar,
    .card-plain.card-testimonial .card-avatar {
      margin-top: 0;
    }
    .card-profile .card-header:not([class*="card-header-"]) {
      background: transparent;
    }
    .card .card-header:not([class*="header-"]) {
      box-shadow: 0 16px 38px -12px rgba(0, 0, 0, 0.56),
        0 4px 25px 0 rgba(0, 0, 0, 0.12), 0 8px 10px -5px rgba(0, 0, 0, 0.2);
    }
  }
}
</style>
