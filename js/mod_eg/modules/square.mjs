export const name = "square"

function draw(ctx, len, x, y, colour) {
    ctx.fillStyle = colour
    ctx.fillRect(x, y, len, colour)

    return {x, y, len, colour }
}

function foo() {
    console.log('Entred foo')
}

function bar() {
    console.log('Entered bar')
}

export { draw, foo, bar }