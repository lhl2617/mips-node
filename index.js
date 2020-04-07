const addon = require('bindings')('mips-parser');
exports.format = addon.formatMIPS;


try {
    console.log(`run!`);
    const out = (exports.format("lw $t0, 4($gp) mult $t0, $t0"));
    console.log(out);
} 
catch (err) {
    console.log(err.message);
}