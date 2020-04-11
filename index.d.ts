import mipsnode = require('mips-node');

declare function format(rawCode: string): string[];
declare function compile(rawCode: string): number[];
declare function intellisense(rawCode: string): string;
declare function getPickledRunInfo(rawCode: string): string;
declare function stepCode(pickledRunInfo: string, steps: number): 
    { pickledRunInfo: string; 
        vscodePayload: {
            highlightLineNo: number;
            highlightClassName: string;
            notificationMsg: string;
            notificationType: string
        }; 
        uiPayload: {
            regs: Record<string, number>;
            mem: Record<number, number>;
            stepsDone: number;
            statusBarMsg: string;
            statusBarClassName: string;
        }
    };