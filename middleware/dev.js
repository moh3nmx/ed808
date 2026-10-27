export default function ({ store, redirect }) {
  if (store.getters.getUid) {
    if (!store.state.user.hasOwnProperty('role') || store.state.user.role != '3'){
      redirect('https://ed808.com')
    }
  }else {
    // redirect('https://ed808.com')
  }
}
