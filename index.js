const addon = require('bindings')('mips-parser');
exports.format = addon.format;
exports.compile = addon.compile;
exports.getPickledRunInfo = addon.getPickledRunInfo;
exports.playground = addon.playground;


try {
    // const res = exports.format("ree: \nlw      $t0,    4($gp) #lolsdafasdfdasfasdfadsfdasds dsf asdf fasd \nmult    $t0,    $t0   \n #reee #a$a\n  # 232 ")
    const res = exports.getPickledRunInfo("#reeee reee ree");
    console.log(res);
}
catch (err) {
    console.error(err.message);
}