#!/usr/local/bin/node
const fs = require("fs");

let [leaving_at, buses] = fs.readFileSync(0).toString().split("\n");
leaving_at = Number(leaving_at);

let [x, m] = buses.split(",").reduce((ans, x) => {
	if(!isNaN(x = Number(x))) {
		let m = leaving_at % x;
		if(m > 0) m = x - m;

		if(m < ans[1]) {
			ans = [x, m];
		}
	}
	return ans;
}, [Infinity, Infinity]);

let ans = x * m;
console.log(`${x}: ${m} (answer: ${ans})`);
