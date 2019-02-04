const express = require('express')
const udp = require('dgram')
const app = express()
const bodyparser = require('body-parser')
let anterior = ''

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
    //if (anterior != `${req.query.fw}${req.query.bw}${req.query.lt}${req.query.rt}`) {
    //    anterior = `${req.query.fw}${req.query.bw}${req.query.lt}${req.query.rt}`
    backend.send(Buffer.from(`p=${req.query.fw}${req.query.bw}${req.query.lt}${req.query.rt}`), 1112, '192.168.1.110', function (error) {
        if (error) {
            client.close();
        } else {
            console.log(`p=${req.query.fw}${req.query.bw}${req.query.lt}${req.query.rt}`)
        }
    });
    //}
})

app.listen(3003)