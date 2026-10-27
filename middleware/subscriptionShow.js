const cookieparser = require('cookieparser')

export default function ({ isServer, store, res, req, query} ) {
  // const token = query.token
  let visitCount = 0;
  if (store.getters.getUid) {
    if (process.server) {
      res.setHeader('Set-Cookie', ['cookie=visitCount']); // Server-side
      req.headers.cookie
    } else {
      document.cookie = 'cookie=visitCount' // Client-side
    }

  }
}
