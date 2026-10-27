<template>
  <div>
    <contact-form :component='false' />
  </div>
</template>

<script>
import ContactForm from '@/components/fields/ContactForm'
export default {
  name: 'ContactUs',
  layout: 'static',
  scrollToTop: true,
  components: {ContactForm},
  head() {
    return {
      links: [
        { rel: "canonical", href: "https://ed808.com/contactus" },
        { rel: "alternate", href: "https://ed808.com/contactus", hreflang: "en" },
        { rel: "shortlink", href: "https://ed808.com/contactus" }
      ],
      title: "Contact Us",
      meta: [
        // OpenGraph data (Most widely used)
        { property: "og:title", content: "Contact Us", hid: "og:title" },
        {
          property: "og:url",
          content: "https://ed808.com/contactus",
          hid: "og:url"
        },
        {
          property: "og:image",
          content: '/images/295.jpg',
          hid: "og:image"
        },

        // Twitter card
        { name: "twitter:title", content: "Contact Us", hid: "twitter:title" },
        {
          name: "twitter:image:src",
          content:'/images/295.jpg',
          hid: "twitter:image:src"
        },

        // Google / Schema.org markup:
        { itemprop: "name", content: "Contact Us", hid: "name" },
        { itemprop: "image", content: '/images/295'.jpg, hid: "image" }
      ]
    };
  },
  mounted(){
      this.$store.state.backGround = '/images/295.jpg'
  }

}
</script>
