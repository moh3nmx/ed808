<template>
  <div class="header">
    <div class="progress-container">
      <div class="progress-bar" id="myBar"></div>
    </div>
  </div>
</template>

<script>
export default {
  mounted() {
    window.onscroll = function() {
      myFunction(this.scrollY);
    };

    function myFunction(y) {
      if(document.getElementsByClassName('node-body').length > 0) {
        let winScroll = y - document.getElementsByClassName('node-body')[0].offsetTop + 50
        let height =
          document.getElementById('node_body').scrollHeight
        // document.documentElement.clientHeight;
        let scrolled = (winScroll / height) * 100;
        if(winScroll < 0) {
          scrolled = 0
        }if (winScroll > height) {
          scrolled = 100
        }
        document.getElementById("myBar").style.width = scrolled + "%";
      }
    }
  }
};
</script>

<style lang="scss" scoped>
.header {
  position: fixed;
  top: 0;
  z-index: 10;
  width: 100%;
  background-color: transparent;
}

.header h2 {
  text-align: center;
}

.progress-container {
  width: 100%;
  height: 2px;
  background: transparent;
}

.progress-bar {
  height: 2px;
  background: #92278f;
  background: var(--md-theme-default-primary-on-background, #92278f);
  width: 0%;
}
</style>
