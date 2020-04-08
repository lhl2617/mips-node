const addon = require('bindings')('mips-parser');
exports.format = addon.formatMIPS;
exports.compile = addon.compileMIPS;


try {
    const res = exports.format("lw      $t0,    4($gp) # lol \nmult    $t0,    $t0   \n #reee #a$a\n  # 232 ")
    // const res = exports.format("#reeee reee ree");
    console.log(res);
}
catch (err) {
    console.error(err.message);
}