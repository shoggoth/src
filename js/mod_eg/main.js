import { create, createReportList } from './modules/canvas.mjs'
import { draw, foo, bar } from './modules/square.mjs'

let myCanvas = create('myCanvas', document.body, 480, 320)
let square1 = draw(myCanvas.ctx, 50, 50, 100, 'blue')
foo()
bar()