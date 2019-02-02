const express = require('express')
const udp = require('dgram')
const app = express()
const bodyparser = require('body-parser')

const backend = udp.createSocket('udp4')
backend.bind(2222);

const jsonpcallback = ({})

app.use(bodyparser.json())
app.use(bodyparser.urlencoded({
    extended: true
}))

app.get('/data', (req, resp) => {
    resp.send(jsonpcallback);
    //console.log(req.query)
    //console.log(`fw=${req.body.fw},bw=${req.body.bw},lt=${req.body.bw},rt=${req.body.bw}`)
    backend.send(Buffer.from(`fw=${req.query.fw},bw=${req.query.bw},lt=${req.query.lt},rt=${req.query.rt}`), 1112, '192.168.1.110', function (error) {
        if (error) {
            client.close();
        } else {
            console.log(`fw=${req.query.fw},bw=${req.query.bw},lt=${req.query.lt},rt=${req.query.rt}`)
        }
    });
})

app.listen(3003)