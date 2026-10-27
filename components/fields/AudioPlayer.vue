<template>
  <div class="relative">
    <!--<span>Total duration: {{ duration }} seconds</span>-->
    <!--<span>Progress: {{ (progress * 100) }}%</span>-->
    <!--<button @click="togglePlayback">{{ playing ? 'Pause' : 'Play' }}</button>-->
    <!--<button @click="stop">Stop</button>-->


    <div class="sound-icon disabled">
      <span class="currentTime">{{ current }}</span>
      <span class="duration">{{ duration }}</span>
      <div :class="playing ? 'sound-wave' : 'sound-wave2'">
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
        <i class='bar'></i>
      </div>

      <div class="controls">


        <a class="play-btn pause" @click="backward()">
          <i class="mdi mdi-step-backward"></i>
          <md-tooltip md-direction="top">Backward 30 Seconds</md-tooltip>
        </a>



        <a class="play-btn" @click="play();playing=true" v-if="!playing">
          <i class="mdi mdi-play"></i>
          <md-tooltip md-direction="top">Play</md-tooltip>
        </a>

        <a class="play-btn pause" @click="playing=false;pause()" v-else>
          <i class="mdi mdi-pause"></i>
          <md-tooltip md-direction="top">Pause</md-tooltip>
        </a>

        <a class="play-btn pause" @click="playing=false;stop()">
          <i class="mdi mdi-stop"></i>
          <md-tooltip md-direction="top">Stop</md-tooltip>
        </a>

        <a class="play-btn pause" @click="forward()">
          <i class="mdi mdi-step-forward"></i>
          <md-tooltip md-direction="top">Forward 30 Seconds</md-tooltip>

        </a>


      </div>


    </div>




    <audio id="podcast-audio" style="height: 100px">
      <source :src="source" type="audio/mpeg">
    </audio>

  </div>
</template>

<script>
  export default {
    name: "HowlerPlayer",
    // mixins: [VueHowler],
    props :['source'],
    data(){
      return {
        playing: false,
        current: '',
        duration:''
      }
    },
    computed: {

    },

    mounted(){
      setInterval(()=>{this.currentTime()},1000)
    },

    methods: {
      play() {
        document.getElementById("podcast-audio").play();
      },
      pause() {
        document.getElementById("podcast-audio").pause();
      },
      stop() {
        document.getElementById("podcast-audio").currentTime = 0;
        document.getElementById("podcast-audio").pause();
      },
      forward(){
        document.getElementById("podcast-audio").currentTime = document.getElementById("podcast-audio").currentTime + 30
      },
      backward(){
        document.getElementById("podcast-audio").currentTime = document.getElementById("podcast-audio").currentTime - 30

      },
      currentTime() {
        this.current = parseInt(document.getElementById("podcast-audio").currentTime)
        this.current = this.timeConvertor(this.current)
        this.duration = parseInt(document.getElementById("podcast-audio").duration)
        this.duration = this.timeConvertor(this.duration)

      },
      timeConvertor(time){
        let sec = parseInt(time%60);
        sec = sec < 10 ? '0'+sec : sec;
        let min = parseInt((time%3600)/60);
        min = min < 10 ? '0'+ min : min;
        let hrs = parseInt(time/3600);
        hrs = hrs < 10 ? '0'+ hrs : hrs;
        return `${hrs}:${min}:${sec}`
      }
    },

  }
</script>

<style lang="scss" scoped>

  .sound-icon {
    display: flex;
    flex-direction: column;
    align-items: center;
    position: relative;
    padding: 10px;
    background: #212121;
    .duration {
      position: absolute;
      right: 10px;
      top: 10px;
      font-weight: 500;
      color: #eeeeee;
    }
    .currentTime {
      position: absolute;
      left: 10px;
      top: 10px;
      color: #eeeeee;

      font-weight: 500;
    }
    .controls{
      position: absolute;
      top: 40px;
      text-align: center;

    }
    .play-btn {
      display: inline-block;
      width: 36px;
      height: 36px;
      line-height: 36px;
      border-radius: 50%;
      background-color: #424242;
      &:hover {
        text-decoration: none;
      }
      &.pause {
        opacity: 0.6;
        transition-duration: 0.5s;
        &:hover {
          opacity: 1;
        }
      }
      &:hover {
        cursor: pointer;
      }
      .mdi {
        font-size: 24px;
        color: #fff;
        display: block;
        line-height: 36px;
        height: 36px;
      }
    }
  }

  .sound-wave {
    width: 50%;
    height: 100px;
    display: flex;
    justify-content: space-around;
    align-items: center;
    .bar:nth-child(1) {
      height: 2px;
      animation-duration: 474ms;
    }

    .bar:nth-child(2) {
      height: 10px;
      animation-duration: 433ms;
    }

    .bar:nth-child(3) {
      height: 18px;
      animation-duration: 407ms;
    }

    .bar:nth-child(4) {
      height: 26px;
      animation-duration: 458ms;
    }

    .bar:nth-child(5) {
      height: 30px;
      animation-duration: 400ms;
    }

    .bar:nth-child(6) {
      height: 32px;
      animation-duration: 427ms;
    }

    .bar:nth-child(7) {
      height: 34px;
      animation-duration: 441ms;
    }

    .bar:nth-child(8) {
      height: 36px;
      animation-duration: 419ms;
    }

    .bar:nth-child(9) {
      height: 40px;
      animation-duration: 487ms;
    }

    .bar:nth-child(10) {
      height: 46px;
      animation-duration: 442ms;
    }

    .bar:nth-child(11) {
      height: 2px;
      animation-duration: 474ms;
    }

    .bar:nth-child(12) {
      height: 10px;
      animation-duration: 433ms;
    }

    .bar:nth-child(13) {
      height: 18px;
      animation-duration: 407ms;
    }

    .bar:nth-child(14) {
      height: 26px;
      animation-duration: 458ms;
    }

    .bar:nth-child(15) {
      height: 30px;
      animation-duration: 400ms;
    }

    .bar:nth-child(16) {
      height: 32px;
      animation-duration: 427ms;
    }

    .bar:nth-child(17) {
      height: 34px;
      animation-duration: 441ms;
    }

    .bar:nth-child(18) {
      height: 36px;
      animation-duration: 419ms;
    }

    .bar:nth-child(19) {
      height: 40px;
      animation-duration: 487ms;
    }

    .bar:nth-child(20) {
      height: 46px;
      animation-duration: 442ms;
    }
  }
  .sound-wave2{
    width: 50%;
    height: 100px;
    display: flex;
    justify-content: space-around;
    align-items: center;
  }
  .sound-wave2 .bar {
    display: block;
    width: 4px;
    margin-right: 2px;
    height: 2px;
    background: #7C93BF;
  }
  .sound-wave .bar {
    display: block;
    width: 4px;
    margin-right: 2px;
    height: 90px;
    background: #7C93BF;
    animation: sound 0ms -800ms linear infinite alternate;
    transition: height 0.8s;
  }

  .sound-wave .bar2{
    display: block;
    width: 4px;
    margin-right: 2px;
    height: 120px;
    background: #7C93BF;
    animation: sound2 0ms -800ms linear infinite alternate;
    transition: height 0.8s;
  }

  @keyframes sound {
    0% {
      opacity: .35;
      height: 6px;
    }
    100% {
      opacity: 1;
      height: 46px;
    }
  }

  @keyframes sound2 {
    0% {
      opacity: .35;
      height: 6px;
    }
    100% {
      opacity: 1;
      height: 120px;
    }
  }

</style>
