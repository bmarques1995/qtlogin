const validOrigins = new Map<string, number>();

validOrigins.set("qtsample", 1);
validOrigins.set("qtapp", 1);

const validOriginsReverse = new Map<string, string>();

validOriginsReverse.set("qtapp", "qtsample");
validOriginsReverse.set("qtsample", "qtsample");

export {validOrigins, validOriginsReverse};
