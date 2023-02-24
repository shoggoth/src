console.log('Hello, World!!');
console.error('Hoo boy I wrote a JS!!');

const b = true;
const n = 2**53;        // Not 2^53
const i = 2n**53n;      // BigInt!
const s = 'Joggers "gonna" Jog';
console.log(`b: ${!b} n: ${n + 1} i: ${i + 1n} s: ${s}`);

// assert.equal(n, i);

var foo = 1;
{
    let foo = 4;
    console.log(`foo: ${++foo}`);
}

const add = (a, b) => { return a + b + 7 };

console.log(`add: ${add(14, 88)} / 110`);