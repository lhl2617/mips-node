const addon = require('bindings')('mips-parser');
exports.format = addon.format;
exports.compile = addon.compile;
exports.intellisense = addon.intellisense;
exports.getPickledRunInfo = addon.getPickledRunInfo;
exports.stepCode = addon.stepCode;
// exports.playground = addon.playground;


// try {
//     const prog = ("  add     $0,     $1,     $1      \n        ori     $1,     $0,     11232131232 \n  add     $0,     $1,     $1      \nadd     $1,     $1,     $1      ");
//     const bin = exports.compile(prog);
//     console.log('----')
//     console.log(bin);
//     // // const ri = exports.getPickledRunInfo("ree: \nlw      $t0,    4($gp) #lolsdafasdfdasfasdfadsfdasds dsf asdf fasd \nmult    $t0,    $t0   \n #reee #a$a\n  # 232 ")
//     // const ri = exports.getPickledRunInfo(prog);
//     // console.log(ri);
//     // const res = exports.stepCode(ri, 1);
//     // console.log(res)
// }
// catch (err) {
//     console.error(err.message);
// }