const addon = require('bindings')('mips-parser');
exports.format = addon.formatMIPS;
exports.compile = addon.compileMIPS;
exports.playground = addon.playground;


try {
    // const res = exports.format("ree: \nlw      $t0,    4($gp) #lolsdafasdfdasfasdfadsfdasds dsf asdf fasd \nmult    $t0,    $t0   \n #reee #a$a\n  # 232 ")
    const res = exports.playground("#reeee reee ree");
    console.log(res);
}
catch (err) {
    console.error(err.message);
}