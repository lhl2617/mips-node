const addon = require('bindings')('mips-parser');
exports.format = addon.formatMIPS;
exports.parse = addon.parseMIPS;

try {
    const res = exports.parse("lw      $t0,    4($gp)  mult    $t0,    $t0     ")
    console.log(res);
}
catch (err) {
    console.error(err.message);
}