const { spawn } = require('child_process')
const path = require('path')

/**
 * Starts the server process and resolves the promise once it binds to given port
 *
 * The server will listen on the specified port for given time; but it does not accept any connections.
 * Actual implementation is in server.c.
 * @param {number} port
 * @param {number} sleepSeconds - seconds to sleep before server exits
 * @returns {Promise}
 */
function startTcpServerNoAccept (port, sleepSeconds) {
  return new Promise((resolve, reject) => {
    const serverProc = spawn('./server.sh', [port, sleepSeconds], {
      shell: true,
      cwd: path.resolve(__dirname)
    })
    console.log('TcpServerNoAccept process started. pid: ', serverProc.pid)
    serverProc.stderr.on('data', (data) => {
      console.log('server: ' + data.toString())
      if (data.toString().indexOf('Listening on port') > -1) {
        resolve()
      }
    })
    serverProc.on('close', reject)
  })
}
module.exports = { startTcpServerNoAccept }
