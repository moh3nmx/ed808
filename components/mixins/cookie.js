import Cookies from 'js-cookie';

export const cookie = {
  methods:{
    getAllCookies(){
      return (Cookies.get())
    },
    getCookie(name) {
      if (process.client) {
        var nameEQ = name + "="
        var ca = document.cookie.split(';')
        for (var i = 0; i < ca.length; i++) {
          var c = ca[i]
          //console.log(c)
          while (c.charAt(0) == ' ') c = c.substring(1, c.length)
          if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length, c.length)
        }
        return null
      }
    },
    setCookie(name, value, days){
      if (process.client) {
        var cookie = name + "=" + value + ";"
        if (days) {
          var expires = new Date()
          //this times is only for setting token
          expires = new Date(expires.getUTCFullYear(),expires.getUTCMonth(),expires.getUTCDate()+days,expires.getUTCHours()+3,expires.getUTCMinutes()+33,expires.getUTCSeconds()+20,expires.getUTCMilliseconds())
          cookie += "expires=" + expires + ";"
        }
        document.cookie = cookie + expires + "; path=/"
      }
    },
    eraseCookie(name) {this.setCookie(name, "", -1);},
    getUserCookie(name){
      var obj = JSON.parse(this.getCookie("user_cookie"))
      if (name){
        return obj[name]
      }
      return null
    },
    set_user(uid){
      this.$store.commit('SET_USER',uid);
    }
  }
}
